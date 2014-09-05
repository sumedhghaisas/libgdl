/**
 * @file relation_policy.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation of RelationPolicy.
 */
#include "relation_policy.hpp"

#include <libgdl/core.hpp>
#include <libgdl/core/symbol_table/symbol_table.hpp>
#include <libgdl/core/data_types/variable_map.hpp>

#include <libgdl/gdlparser/parser/kif_driver.hpp>
#include <libgdl/gdlparser/parser/error_type.hpp>

using namespace std;
using namespace libgdl;
using namespace libgdl::gdlparser::parser;

bool RelationPolicy::CodeGen(Argument*& out,
                             KIFDriver& driver,
                             std::string* command,
                             std::list<Term*>& terms,
                             VariableMap& v_map)
{

}

bool RelationPolicy::CodeGen(KIFDriver& driver,
                             std::string* command,
                             std::list<Term*>& terms,
                             VariableMap& v_map,
                             const Location& command_loc)
{
  SymbolTable* symbol_table = driver.GetSymbolTable();
  Symbol* sym;
  size_t id = symbol_table->CheckEntry(*command, sym);
  if(sym != NULL)
  {
    if(sym->Arity() != terms.size())
    {
      ARITY_ERROR(error, *command, terms.size(), sym->Arity(), command_loc, sym->GetLocation());
      driver.Error(error);
      return false;
    }
    else if(sym->SymbolType() != Symbol::RELATION)
    {
      RF_ERROR(error, *command, "Relation", "Function", command_loc, sym->GetLocation());
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

  Fact f;
  f.isLocation = true;
  f.loc = command_loc;
  f.arg = arg;
  driver.AddFact(std::move(f));

  return true;
}
