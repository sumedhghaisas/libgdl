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
#include <atomic>
#include <boost/intrusive_ptr.hpp>
#include <boost/unordered_map.hpp>

#include <libgdl/core/util/log.hpp>
#include <libgdl/core/util/to_string.hpp>
#include <libgdl/core/data_types/location.hpp>

#include "symbols.hpp"

namespace libgdl
{
namespace core
{

class RawSymbolTable
{
  typedef boost::unordered_map<std::string, size_t> IDMap;
  typedef boost::unordered_map<size_t, Symbol*> SymbolMap;
 public:
  RawSymbolTable();

  ~RawSymbolTable();

  size_t AddEntry(const std::string& name,
                  const Location& loc,
                  size_t arity = 0,
                  bool isRelation = false);

  size_t CheckEntry(const std::string& name, Symbol*& symbol);

  std::string GetCommandName(size_t id) const;

  bool AddDefined(size_t id, const Location& loc);

  Log& GetLog() { return log; }

  friend std::ostream& operator<<(std::ostream& s,
                                  const RawSymbolTable& symbol_table)
  {
    for(RawSymbolTable::IDMap::const_iterator it = symbol_table.id_table.begin();
                                          it != symbol_table.id_table.end();it++)
    {
      s << it->second << " -> " << it->second;
      RawSymbolTable::SymbolMap::const_iterator it2 =
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

  //! Refernce count stored in atomic variable (useful for multi-threading)
  std::atomic_size_t count;

 private:
  SymbolMap symbol_table;
  IDMap id_table;

  size_t index;

  mutable Log log;
}; // class RawSymbolTable

//! Intrusive pointer release function for RawSymbolTable
//! Decrements reference count of RawSymbolTable object
//!
//! \param p object to decrement
//!
//!
inline void intrusive_ptr_release(RawSymbolTable* p)
{
   if (--p->count == 0u)
        delete p;
}

//! Intrusive pointer add function for RawSymbolTable
//! Increments reference count
//!
//! \param p object to increment
//!
//!
inline void intrusive_ptr_add_ref(RawSymbolTable* p)
{
  ++p->count;
}

class SymbolTable : public boost::intrusive_ptr<RawSymbolTable>
{
  typedef boost::unordered_map<std::string, size_t> IDMap;
  typedef boost::unordered_map<size_t, Symbol*> SymbolMap;
 public:
  SymbolTable()
    : boost::intrusive_ptr<RawSymbolTable>(new RawSymbolTable()) {}

  SymbolTable(RawSymbolTable* st)
    : boost::intrusive_ptr<RawSymbolTable>(st) {}

  size_t AddEntry(const std::string& name,
                  const Location& loc,
                  size_t arity = 0,
                  bool isRelation = false)
  {
    return get()->AddEntry(name, loc, arity, isRelation);
  }

  size_t CheckEntry(const std::string& name, Symbol*& symbol)
  {
    return get()->CheckEntry(name, symbol);
  }

  std::string GetCommandName(size_t id) const
  {
    return get()->GetCommandName(id);
  }

  bool AddDefined(size_t id, const Location& loc)
  {
    return get()->AddDefined(id, loc);
  }

  Log& GetLog() { return get()->GetLog(); }

  friend std::ostream& operator<<(std::ostream& s,
                                  const SymbolTable& symbol_table)
  {
    s << *symbol_table.get();
    return s;
  }

  const SymbolMap& GetSymbolMap() const
  {
    return get()->GetSymbolMap();
  }
  const IDMap& GetIDMap() const
  {
    return get()->GetIDMap();
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
};

}; // namespace core
}; // namespace libgdl

#endif // _LIBGDL_CORE_SYMBOL_TABLE_HPP_INCLUDED
