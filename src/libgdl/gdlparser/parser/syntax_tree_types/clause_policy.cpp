/**
 * @file basic_construct.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of BasicConstruct class.
 */
#include "clause_policy.hpp"

#include <vector>

#include <libgdl/core.hpp>
#include <libgdl/core/symbol_table/symbol_table.hpp>
#include <libgdl/core/data_types/variable_map.hpp>

#include <libgdl/gdlparser/parser/kif_driver.hpp>
#include <libgdl/gdlparser/parser/error_type.hpp>

using namespace std;
using namespace libgdl::gdlparser::parser;

bool ClausePolicy::CodeGen(Argument*& out,
                           KIFDriver& driver,
                           Sentence* command,
                           std::list<Premiss*>& terms,
                           VariableMap& v_map,
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
                           VariableMap& v_map,
                           const Location& command_loc)
{
  SymbolTable* symbol_table = driver.GetSymbolTable();

  Argument* head;
  if(!command->CodeGen(head, driver, v_map))
    return false;

  const std::string& hcmd = symbol_table->GetCommandName(head->value);

  if(hcmd == "does")
  {
    ErrorType error;
    error.AddEntry("Relation " + hcmd + " cannot appear as head in the clause.",
                   command_loc);
    driver.Error(error);
  }

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
    args.push_back(temp);
  }

  Clause c;
  c.head = head;
  c.premisses = args;
  c.loc = command_loc;
  c.isLocation = true;

  driver.AddClause(std::move(c));

  return true;
}
