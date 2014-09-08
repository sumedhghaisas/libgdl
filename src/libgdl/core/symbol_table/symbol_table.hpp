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
namespace core
{

class SymbolTable
{
  typedef boost::unordered_map<std::string, size_t> IDMap;
  typedef boost::unordered_map<size_t, Symbol*> SymbolMap;
 public:
  SymbolTable();

  ~SymbolTable();

  size_t AddEntry(const std::string& name,
                  const Location& loc,
                  size_t arity = 0,
                  bool isRelation = false);

  size_t CheckEntry(const std::string& name, Symbol*& symbol);

  std::string GetCommandName(size_t id) const;

  bool AddDefined(size_t id, const Location& loc);

  Log& GetLog() { return log; }

  friend std::ostream& operator<<(std::ostream& s,
                                  const SymbolTable& symbol_table)
  {
    for(SymbolTable::IDMap::const_iterator it = symbol_table.id_table.begin();
                                          it != symbol_table.id_table.end();it++)
    {
      s << it->second << " -> " << it->second;
      SymbolTable::SymbolMap::const_iterator it2 =
                                    symbol_table.symbol_table.find(it->second);
      s << " -> " << *it2->second << std::endl;
    }
    return s;
  }

  const SymbolMap& GetSymbolMap() const
  {
    return symbol_table;
  }
  const IDMap& GetIDMap() const
  {
    return id_table;
  }

  enum IDS {NotID,
            OrID,
            DistinctID,
            RoleID,
            BaseID,
            InputID,
            InitID,
            LegalID,
            NextID,
            DoesID,
            TrueID,
            GoalID,
            TerminalID};

 private:
  SymbolMap symbol_table;
  IDMap id_table;

  size_t index;

  mutable Log log;
}; // class SymbolTable

}; // namespace core
}; // namespace libgdl

#endif // _LIBGDL_CORE_SYMBOL_TABLE_HPP_INCLUDED
