/**
 * @file clause_policy.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation of ClausePolicy.
 */
#include "clause_policy.hpp"

#include <vector>

#include <libgdl/core.hpp>
#include <libgdl/core/data_types/error_type.hpp>
#include <libgdl/core/util/setop.hpp>
#include <libgdl/core/symbol_table/symbol_table.hpp>
#include <libgdl/core/data_types/str_var_map.hpp>

#include <libgdl/gdlparser/parser/kif_driver.hpp>

using namespace std;
using namespace libgdl::core;
using namespace libgdl::gdlparser::parser;

bool ClausePolicy::CodeGen(Argument*& out,
                           KIFDriver& driver,
                           Sentence* command,
                           std::list<Premiss*>& terms,
                           StrVarMap& v_map,
                           const Location& command_loc)
{
  MARK_USED(out);
  MARK_USED(driver);
  MARK_USED(command);
  MARK_USED(terms);
  MARK_USED(v_map);
  MARK_USED(command_loc);
  cout << "lol3" << endl;
  exit(1);
  return false;
}

bool ClausePolicy::CodeGen(KIFDriver& driver,
                           Sentence* command,
                           std::list<Premiss*>& terms,
                           StrVarMap& v_map,
                           const Location& command_loc)
{
  SymbolTable symbol_table = driver.GetSymbolTable();

  Argument* head;
  if(!command->CodeGen(head, driver, v_map))
    return false;

  const std::string& hcmd = symbol_table->GetCommandName(head->value);

  if(hcmd == "does" || hcmd == "true" || hcmd == "distinct" || hcmd == "role")
  {
    core::ErrorType error;
    error.AddEntry("Relation " + hcmd + " cannot appear as head in the clause.",
                   command_loc);
    driver.Error(error);
  }

  symbol_table->AddDefined(head->value, command_loc);

  vector<Argument*> args;

  for(std::list<Premiss*>::iterator it = terms.begin();it != terms.end();it++)
  {
    Argument* temp;
    if(!(*it)->CodeGen(temp, driver, v_map))
    {
      for(size_t i = 0;i < args.size();i++)
        delete args[i];
      delete head;
      return false;
    }

    symbol_table->AddUsed(temp->value, command_loc);

    args.push_back(temp);
  }

  set<const Argument*> head_vars = head->GetVariables();

  set<const Argument*> neg_vars;
  set<const Argument*> pos_vars;

  for(size_t i = 0;i < args.size();i++)
  {
    if(args[i]->IsNegation())
      neg_vars = util::setop::setUnion(neg_vars, args[i]->GetVariables());
    else if(args[i]->IsOr())
    {
      set<const Argument*> temp;
      bool isFirst = true;
      for(size_t j = 0;j < args[i]->args.size();j++)
      {
        if(isFirst)
        {
          temp = args[i]->args[j]->GetVariables();
          isFirst = false;
        }
        else temp = util::setop::setIntersection(temp,
                                                 args[i]->args[j]->GetVariables());
      }
      pos_vars = util::setop::setUnion(pos_vars, temp);
    }
    else pos_vars = util::setop::setUnion(pos_vars, args[i]->GetVariables());
  }

  head_vars = util::setop::setDifference(head_vars, pos_vars);
  neg_vars = util::setop::setDifference(neg_vars, pos_vars);

  for(set<const Argument*>::iterator it = head_vars.begin();it != head_vars.end();
                                                                        it++)
  {
    core::ErrorType error;
    error.AddEntry("Variable " + (*it)->val + " appearing in the head does not \
appear in any positive body.", command_loc);
    driver.Error(error);
  }

  for(set<const Argument*>::iterator it = neg_vars.begin();it != neg_vars.end();
                                                                        it++)
  {
    core::ErrorType error;
    error.AddEntry("Variable " + (*it)->val + " appearing in the negative body \
does not appear in any positive body.", command_loc);
    driver.Error(error);
  }

  Clause c;
  c.head = head;
  c.premisses = args;
  c.loc = command_loc;
  c.isLocation = true;

  const Clause& cl = driver.AddClause(std::move(c));

  core::DGraph& graph = driver.GetDGraph();
  graph.AddNode(head->value);

  for(size_t i = 0;i < cl.premisses.size();i++)
  {
    if(cl.premisses[i]->IsNegation())
    {
      graph.AddNode(cl.premisses[i]->args[0]->value);
      graph.AddDependency(head->value, cl.premisses[i]->args[0]->value,
                          &cl, cl.premisses[i]->args[0], true);
    }
    else if(cl.premisses[i]->IsOr())
    {
      for(size_t j = 0;j < cl.premisses[i]->args.size();j++)
      {
        graph.AddNode(cl.premisses[i]->args[j]->value);
        graph.AddDependency(head->value, cl.premisses[i]->args[j]->value,
                            &cl, cl.premisses[i]->args[0]);
      }
    }
    else
    {
      graph.AddNode(cl.premisses[i]->value);
      graph.AddDependency(head->value, cl.premisses[i]->value,
                          &cl, cl.premisses[i]);
    }
  }

  return true;
}
