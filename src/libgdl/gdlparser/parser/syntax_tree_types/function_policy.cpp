/**
 * @file function_policy.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation of FunctionPolicy.
 */
#include "function_policy.hpp"

#include <libgdl/core.hpp>
#include <libgdl/core/data_types/location.hpp>
#include <libgdl/core/symbol_table/symbol_table.hpp>

#include <libgdl/gdlparser/parser/kif_driver.hpp>

using namespace std;
using namespace libgdl;
using namespace libgdl::core;
using namespace libgdl::gdlparser::parser;

bool FunctionPolicy::CodeGen(Argument*& out,
                             KIFDriver& driver,
                             std::string* command,
                             std::list<Term*>& terms,
                             StrVarMap& v_map,
                             const Location& command_loc)
{
  if((*command)[0] == '?')
  {
    StrVarMap::const_iterator it = v_map.find(*command);
    if(it == v_map.end())
    {
      out = new Argument;
      out->val = *command;
      out->t = Argument::Var;

      v_map[*command] = out;
    }
    else out = it->second;
    return true;
  }

  SymbolTable symbol_table = driver.GetSymbolTable();
  Symbol* sym;
  size_t id = symbol_table->CheckEntry(*command, terms.size(), false, sym);

  if(sym == NULL)
    id = symbol_table->AddEntry(*command, command_loc, terms.size(), false);

  out = new Argument();
  out->t = Argument::Function;
  out->value = id;

  for(list<Term*>::const_iterator it = terms.begin();it != terms.end();it++)
  {
    Argument* temp;
    if(!(*it)->CodeGen(temp, driver, v_map))
    {
      delete out;
      return false;
    }
    out->args.push_back(temp);
  }

  return true;
}

bool FunctionPolicy::CodeGen(KIFDriver& driver,
                             std::string* command,
                             std::list<Term*>& terms,
                             StrVarMap& v_map,
                             const Location& command_loc)
{
  MARK_USED(driver);
  MARK_USED(command);
  MARK_USED(terms);
  MARK_USED(v_map);
  MARK_USED(command_loc);
  exit(1);
  return false;
}

