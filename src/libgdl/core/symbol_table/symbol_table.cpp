#include "symbol_table.hpp"

using namespace std;
using namespace libgdl;
using namespace libgdl::gdlparser::parser;

SymbolTable::SymbolTable()
{
  index = 0;

  AddEntry("not", Location(), 0, true);
  AddEntry("or", Location(), 0, true);
  AddEntry("distinct", Location(), 2, true);
}

SymbolTable::~SymbolTable()
{
  for(SymbolMap::iterator it = symbol_table.begin();it != symbol_table.end();it++)
    delete it->second;
}

size_t SymbolTable::AddEntry(const std::string& name,
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

size_t SymbolTable::CheckEntry(const std::string& name, Symbol*& symbol)
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

bool SymbolTable::AddDefined(size_t id, const Location& loc)
{
  SymbolMap::iterator it = symbol_table.find(id);
  if(it == symbol_table.end()) return false;

  it->second->AddDef(loc);
  return true;
}

std::string SymbolTable::GetCommandName(size_t id) const
{
  const SymbolMap::const_iterator it = symbol_table.find(id);
  if(it != symbol_table.end()) return (it->second)->Name();
  else
  {
    log.Warn << "Identifier " << ToString(id) << " does not exist" << std::endl;
    return "";
  }
}
