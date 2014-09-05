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

#include <libgdl/core/util/log.hpp>
#include <libgdl/core/util/to_string.hpp>
#include <libgdl/core/data_types/location.hpp>

#include "symbols.hpp"

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
    for(SymbolMap::iterator it = symbol_table.begin();
                                                it != symbol_table.end();it++)
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
    symbol_table[index] = sym;
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
    symbol = symbol_table[it->second];
    return it->second;
  }

  std::string GetCommandName(size_t id) const
  {
    const SymbolMap::const_iterator it = symbol_table.find(id);
    if(it != symbol_table.end()) return (it->second)->Name();
    else
    {
      log.Warn << "Identifier " << ToString(id) << " does not exist" << std::endl;
      return "";
    }
  }

  Log& GetLog() { return log; }

  friend std::ostream& operator<<(std::ostream& s,
                                  const libgdl::SymbolTable& symbol_table)
  {
    for(libgdl::SymbolTable::IDMap::const_iterator it = symbol_table.id_table.begin();
                                          it != symbol_table.id_table.end();it++)
    {
      s << it->second << " -> " << it->second;
      libgdl::SymbolTable::SymbolMap::const_iterator it2 =
                                        symbol_table.symbol_table.find(it->second);
      s << " -> " << *it2->second << std::endl;
    }
    return s;
  }

 private:
  SymbolMap symbol_table;
  IDMap id_table;

  size_t index;

  mutable Log log;
}; // class SymbolTable

}; // namespace libgdl

#endif // _LIBGDL_CORE_SYMBOL_TABLE_HPP_INCLUDED
