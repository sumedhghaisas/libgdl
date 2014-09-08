/**
 * @file relation_policy.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation of RelationPolicy.
 */
#include "relation_policy.hpp"

#include <libgdl/core.hpp>
#include <libgdl/core/data_types/error_type.hpp>
#include <libgdl/core/symbol_table/symbol_table.hpp>
#include <libgdl/core/data_types/str_var_map.hpp>

#include <libgdl/gdlparser/parser/kif_driver.hpp>

using namespace std;
using namespace libgdl;
using namespace libgdl::gdlparser::parser;

bool RelationPolicy::CodeGen(Argument*& out,
                             KIFDriver& driver,
                             std::string* command,
                             std::list<Term*>& terms,
                             StrVarMap& v_map,
                             const Location& command_loc)
{
  SymbolTable* symbol_table = driver.GetSymbolTable();
  Symbol* sym;
  size_t id = symbol_table->CheckEntry(*command, sym);
  if(sym != NULL)
  {
    if(sym->Arity() != terms.size())
    {
      ARITY_ERROR(error,
                  *command, terms.size(),
                  sym->Arity(),
                  command_loc,
                  sym->GetLocation());
      driver.Error(error);
      return false;
    }
    else if(sym->SymbolType() != Symbol::RELATION)
    {
      RF_ERROR(error,
               *command,
               "Relation", "Function",
               command_loc,
               sym->GetLocation());
      driver.Error(error);
      return false;
    }
  }
  else id = symbol_table->AddEntry(*command, command_loc, terms.size(), true);

  Argument* arg = new Argument();
  arg->t = Argument::Relation;
  arg->value = id;

  for(std::list<Term*>::const_iterator it = terms.begin();it != terms.end();it++)
  {
    Argument* temp;
    if(!(*it)->CodeGen(temp, driver, v_map))
    {
      delete arg;
      return false;
    }
    arg->args.push_back(temp);
  }

  out = arg;

  return true;
}

bool RelationPolicy::CodeGen(KIFDriver& driver,
                             std::string* command,
                             std::list<Term*>& terms,
                             StrVarMap& v_map,
                             const Location& command_loc)
{
  if(*command == "does" || *command == "true")
  {
    Q_ERROR(error,
            "Relation " + *command + " cannot appear as a fact.",
            command_loc);
    driver.Error(error);
  }
  else if(*command == "role" && terms.size() != 1)
  {
    PR_ARITY_ERROR(error, *command, 1, terms.size(), command_loc);
    driver.Error(error);
    return false;
  }

  SymbolTable* symbol_table = driver.GetSymbolTable();
  Symbol* sym;
  size_t id = symbol_table->CheckEntry(*command, sym);
  if(sym != NULL)
  {
    if(sym->Arity() != terms.size())
    {
      ARITY_ERROR(error,
                  *command, terms.size(),
                  sym->Arity(),
                  command_loc,
                  sym->GetLocation());
      driver.Error(error);
      return false;
    }
    else if(sym->SymbolType() != Symbol::RELATION)
    {
      RF_ERROR(error,
               *command,
               "Relation", "Function",
               command_loc,
               sym->GetLocation());
      driver.Error(error);
      return false;
    }
  }
  else id = symbol_table->AddEntry(*command, command_loc, terms.size(), true);

  symbol_table->AddDefined(id, command_loc);

  Argument* arg = new Argument();
  arg->t = Argument::Relation;
  arg->value = id;

  for(std::list<Term*>::const_iterator it = terms.begin();it != terms.end();it++)
  {
    Argument* temp;
    if(!(*it)->CodeGen(temp, driver, v_map))
    {
      delete arg;
      return false;
    }
    else if(temp->HasVariables())
    {
      core::ErrorType error;
      error.AddEntry("Variables cannot appear in facts.", command_loc);
      driver.Error(error);
    }
    arg->args.push_back(temp);
  }

  Fact f;
  f.isLocation = true;
  f.loc = command_loc;
  f.arg = arg;
  driver.AddFact(std::move(f));

  driver.GetDGraph()->AddNode(id);

  return true;
}

bool RelationPolicy::CodeGen(Argument*& out,
                             KIFDriver& driver,
                             std::string* command,
                             std::list<Sentence*>& sentences,
                             StrVarMap& v_map,
                             const Location& command_loc)
{
  MARK_USED(command_loc);

  if(*command == "")
  {
    if(!(*sentences.begin())->CodeGen(out, driver, v_map))
      return false;
    return true;
  }

  SymbolTable* symbol_table = driver.GetSymbolTable();
  Symbol* sym;

  size_t id = symbol_table->CheckEntry(*command, sym);

  Argument* arg = new Argument();
  arg->t = Argument::Relation;
  arg->value = id;

  for(std::list<Sentence*>::const_iterator it = sentences.begin();
                                              it != sentences.end();it++)
  {
    Argument* temp;
    if(!(*it)->CodeGen(temp, driver, v_map))
    {
      delete arg;
      return false;
    }
    arg->args.push_back(temp);
  }

  out = arg;

  return true;
}

bool RelationPolicy::CodeGen(KIFDriver& driver,
                             std::string* command,
                             std::list<Sentence*>& sentences,
                             StrVarMap& v_map,
                             const Location& command_loc)
{
  MARK_USED(driver);
  MARK_USED(command);
  MARK_USED(sentences);
  MARK_USED(v_map);
  MARK_USED(command_loc);
  cout << "lol2" << endl;
  exit(1);
  return false;
}
