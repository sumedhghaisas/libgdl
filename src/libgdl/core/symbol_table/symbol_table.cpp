/**
 * @file symbol_table.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation of SymbolTable.
 */
#include "symbol_table.hpp"

using namespace std;
using namespace libgdl;
using namespace libgdl::core;
using namespace libgdl::gdlparser::parser;

RawSymbolTable::RawSymbolTable()
  : count(0u)
{
  index = 0;

  AddEntry("not", Location(), 0, true); // 0
  AddEntry("or", Location(), 0, true); // 1
  AddEntry("distinct", Location(), 2, true); // 2

  AddEntry("role", Location(), 1, true); // 3
  AddEntry("base", Location(), 1, true); // 4
  AddEntry("input", Location(), 2, true); // 5
  AddEntry("init", Location(), 1, true); // 6
  AddEntry("legal", Location(), 2, true); // 7
  AddEntry("next", Location(), 1, true); // 8
  AddEntry("does", Location(), 2, true); // 9
  AddEntry("true", Location(), 1, true); // 10
  AddEntry("goal", Location(), 2, true); // 11
  AddEntry("terminal", Location(), 0, true); // 12
}

RawSymbolTable::~RawSymbolTable()
{
  for(SymbolMap::iterator it = symbol_table.begin();it != symbol_table.end();it++)
    delete it->second;
}

size_t RawSymbolTable::AddEntry(const std::string& name,
                                const Location& loc,
                                size_t arity,
                                bool isRelation)
{
  Symbol* sym;
  if(isRelation)
    sym = new RelationSymbol(name, arity, loc);
  else sym = new FunctionSymbol(name, arity, loc);

  id_table[name] = index;
  symbol_table[index] = sym;
  index++;
  return index - 1;
}

size_t RawSymbolTable::CheckEntry(const std::string& name, Symbol*& symbol)
{
  IDMap::const_iterator it = id_table.find(name);
  if(it == id_table.end())
  {
    symbol = NULL;
    return 0;
  }
  symbol = symbol_table[it->second];
  return it->second;
}

bool RawSymbolTable::AddDefined(size_t id, const Location& loc)
{
  SymbolMap::iterator it = symbol_table.find(id);
  if(it == symbol_table.end()) return false;

  it->second->AddDef(loc);
  return true;
}

std::string RawSymbolTable::GetCommandName(size_t id) const
{
  const SymbolMap::const_iterator it = symbol_table.find(id);
  if(it != symbol_table.end()) return (it->second)->Name();
  else
  {
    log.Warn << "Identifier " << ToString(id) << " does not exist" << std::endl;
    return "";
  }
}
