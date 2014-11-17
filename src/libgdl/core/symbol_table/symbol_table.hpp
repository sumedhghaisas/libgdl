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
#include <libgdl/core/data_types/error_type.hpp>

#include "symbols.hpp"

namespace libgdl
{
namespace core
{

/**
 * Symbol table implementation which supports intrusive pointers. For more
 * information see SymbolTable.
 *
 * @see SymbolTable
 */
class RawSymbolTable
{
  //! Map from string to ID number
  typedef boost::unordered_map<std::string, size_t> IDMap;
  //! Map from ID number to symbol entry
  typedef boost::unordered_map<size_t, Symbol*> SymbolMap;
 public:
  //! Empty constructor
  RawSymbolTable(const Log& log = GLOBAL_LOG);

  //! Destructor
  ~RawSymbolTable();

  //! Add entry of the given symbol with given properties in the SymbolTable
  //!
  //! \param name Name of the symbol
  //! \param loc Location of occurrence
  //! \param arity Arity of the symbol
  //! \param isRelation Is symbol a predicate
  //! \return size_t ID of the symbol added
  //!
  //!
  size_t AddEntry(const std::string& name,
                  const Location& loc,
                  size_t arity = 0,
                  bool isRelation = false);

  //! Checks if the given symbol is already registered in the SymbolTable
  //! If yes then returns Symbol object corresponding to the name as the second
  //! argument, else returns NULL.
  //! Returns ID of the symbol if found else returns 0.
  //!
  //! \param name Name of the symbol
  //! \param arity Arity associated with the symbol
  //! \param isRelation Is the symbol associated with relation or function
  //! \param symbol Symbol object corresponding to the entry
  //! \return size_t ID of the entry
  //!
  //!
  size_t CheckEntry(const std::string& name,
                    size_t arity,
                    bool isRelation,
                    Symbol*& symbol);

  //! Returns symbol name(command name) given ID.
  //!
  //! \param id ID of the symbol
  //! \return std::string String representation of the symbol
  //!
  //!
  std::string GetCommandName(size_t id) const;

  //! Adds definition location to given symbol.
  //!
  //! \param id ID of the symbol
  //! \param loc Location of definition
  //! \return bool If the symbol is not found returns false.
  //!
  //!
  bool AddDefined(size_t id, const Location& loc);

  //! Adds usage location to the given symbol
  //!
  //! \param id ID of the symbol
  //! \param loc Location of usage
  //! \return bool If the symbol is not found returns false.
  //!
  //!
  bool AddUsed(size_t id, const Location& loc);

  //! Generate warnings for unused and undefined relations
  //!
  //! \param out List of warnings
  //! \return void
  //!
  //!
  void GenerateWarnings(std::list<ErrorType>& out) const;

  //! Get-Set logging stream
  Log& GetLog() { return log; }

  //! Friend Operator<< for SymbolTable
  //!
  //! \param s Output stream
  //! \param symbol_table SymbolTable to print
  //! \return std::ostream&
  //!
  //!
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

  //! Get symbol mapping
  const SymbolMap& GetSymbolMap() const
  {
    return symbol_table;
  }
  //! Get ID mapping
  const IDMap& GetIDMap() const
  {
    return id_table;
  }

  static std::string GetMangledName(const std::string& name,
                             size_t arity,
                             bool isRelation);

  static std::string UnmangleName(const std::string& uname,
                           size_t* arity = NULL,
                           bool* isRelation = NULL);

  //! Represents GDL predefined IDs.
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

  //! ID to symbol entry mapping
  SymbolMap symbol_table;
  //! String to ID mapping
  IDMap id_table;

  //! Next ID to be used for new entry
  size_t index;

  //! Logging stream
  mutable Log log;
}; // class RawSymbolTable

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

/**
 * Represents symbol table which is created while parsing KIF. This table
 * stores all the predicates and functions by assigning them unique IDs. These
 * IDs are used while storing all the facts and clauses. Storing IDs rather than
 * string increases unification speed and saves space. Symbol table also keeps
 * track of location where certain symbol is defined and used. This helps in
 * generating verbose errors. Predefined symbols of GDL are given predefined
 * IDs for faster access. SymbolTable is used by many modules for different
 * purposes but most of the time the same symbol table is passed between them.
 * As deep copying SymbolTable is an expensive computation, SymbolTable uses
 * intrusive_ptr to wrap RawSymbolTable. This way copying SymbolTable is always
 * a shallow copy. This wrapper is structured in such a way as to abstract the
 * part of RawSymbolTable from the user.
 *
 * @see RawSymbolTable, Symbol
 */
class SymbolTable : public boost::intrusive_ptr<RawSymbolTable>
{
  //! Map from string to ID number
  typedef boost::unordered_map<std::string, size_t> IDMap;
  //! Map from ID number to symbol entry
  typedef boost::unordered_map<size_t, Symbol*> SymbolMap;

 public:
  //! Creates empty SymbolTable
  //!
  //!
  SymbolTable(const Log& log = GLOBAL_LOG)
    : boost::intrusive_ptr<RawSymbolTable>(new RawSymbolTable(log)) {}

  //! Wraps the given RawSymbolTable
  //!
  //! \param st The RawSymbolTable pointer
  //!
  //!
  SymbolTable(RawSymbolTable* st)
    : boost::intrusive_ptr<RawSymbolTable>(st) {}

  //! Add entry of the given symbol with given properties in the SymbolTable
  //!
  //! \param name Name of the symbol
  //! \param loc Location of occurrence
  //! \param arity Arity of the symbol
  //! \param isRelation Is symbol a predicate
  //! \return size_t ID of the symbol added
  //!
  //!
  size_t AddEntry(const std::string& name,
                  const Location& loc,
                  size_t arity = 0,
                  bool isRelation = false)
  {
    return get()->AddEntry(name, loc, arity, isRelation);
  }

  //! Checks if the given symbol is already registered in the SymbolTable
  //! If yes then returns Symbol object corresponding to the name as the second
  //! argument, else returns NULL.
  //! Returns ID of the symbol if found else returns 0.
  //!
  //! \param name Name of the symbol
  //! \param arity Arity associated with the symbol
  //! \param isRelation Is the symbol associated with relation or function
  //! \param symbol Symbol object corresponding to the entry
  //! \return size_t ID of the entry
  //!
  //!
  size_t CheckEntry(const std::string& name,
                    size_t arity,
                    bool isRelation,
                    Symbol*& symbol)
  {
    return get()->CheckEntry(name, arity, isRelation, symbol);
  }

  //! Returns symbol name(command name) given ID.
  //!
  //! \param id ID of the symbol
  //! \return std::string String representation of the symbol
  //!
  //!
  std::string GetCommandName(size_t id) const
  {
    return get()->GetCommandName(id);
  }

  //! Adds definition location to given symbol.
  //!
  //! \param id ID of the symbol
  //! \param loc Location of definition
  //! \return bool If the symbol is not found returns false.
  //!
  //!
  bool AddDefined(size_t id, const Location& loc)
  {
    return get()->AddDefined(id, loc);
  }

  //! Returns logging stream(Get-Set)
  Log& GetLog() { return get()->GetLog(); }

  //! Friend Operator<< for SymbolTable
  //!
  //! \param s Output stream
  //! \param symbol_table SymbolTable to print
  //! \return std::ostream&
  //!
  //!
  friend std::ostream& operator<<(std::ostream& s,
                                  const SymbolTable& symbol_table)
  {
    s << *symbol_table.get();
    return s;
  }

  //! Get symbol mapping
  const SymbolMap& GetSymbolMap() const
  {
    return get()->GetSymbolMap();
  }
  //! Get ID mapping
  const IDMap& GetIDMap() const
  {
    return get()->GetIDMap();
  }

  //! Represents GDL predefined IDs.
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
