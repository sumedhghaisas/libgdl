/**
 * @file symbol_table.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of SymbolTable.
 */
#ifndef _LIBGDL_CORE_SYMBOL_TABLE_HPP_INCLUDED
#define _LIBGDL_CORE_SYMBOL_TABLE_HPP_INCLUDED

#include <string>
#include <list>
#include <map>
#include <boost/unordered_map.hpp>

#include "symbols.hpp"

#include <libgdl/core/data_types/location.hpp>

namespace libgdl
{

class SymbolTable
{
  typedef boost::unordered_map<std::string, size_t> IDMap;
  typedef boost::unordered_map<size_t, Symbol*> SymbolMap;
 public:
  SymbolTable() : index(0) {}

  ~SymbolTable()
  {
    for(SymbolMap::iterator it = symbol_tables.begin();
                                                it != symbol_tables.end();it++)
      delete it->second;
  }

  size_t AddEntry(const std::string& name,
                  const Location& loc,
                  size_t arity = 0,
                  bool isRelation = false)
  {
    Symbol* sym;
    if(isRelation)
      sym = new RelationSymbol(name, arity, loc);
    else sym = new FunctionSymbol(name, arity, loc);

    id_table[name] = index;
    symbol_tables[index] = sym;
    index++;
    return index - 1;
  }

  size_t CheckEntry(const std::string& name, Symbol*& symbol)
  {
    IDMap::const_iterator it = id_table.find(name);
    if(it == id_table.end())
    {
      symbol = NULL;
      return 0;
    }
    symbol = symbol_tables[it->second];
    return it->second;
  }

 private:
  SymbolMap symbol_tables;
  IDMap id_table;

  size_t index;
}; // class SymbolTable

}; // namespace libgdl

#endif // _LIBGDL_CORE_SYMBOL_TABLE_HPP_INCLUDED
