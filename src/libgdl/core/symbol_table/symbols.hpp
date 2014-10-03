/**
 * @file symbols.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of Symbol.
 */
#ifndef _LIBGDL_CORE_SYMBOLS_HPP_INCLUDED
#define _LIBGDL_CORE_SYMBOLS_HPP_INCLUDED

#include <iostream>
#include <string>
#include <list>

#include <libgdl/core/data_types/location.hpp>
#include <libgdl/core/util/to_string.hpp>

namespace libgdl
{
namespace core
{

/**
 * Represents single entry of the SymbolTable. This class stores all the related
 * information about the entry and provides better abstraction. Location
 * information is stored for verbose output. There are 2 types of symbol entries.
 * One is FunctionSymbol and other is RelationSymbol.
 *
 * @see SymbolTable
 */
struct Symbol
{
  //! Type of the Symbol
  enum Type {FUNCTION, RELATION};

  //! Create symbol with given information
  Symbol(const Type& t, const std::string& name, const Location& loc)
    : t(t), name(name), loc(loc)
  {
    isUsed = false;
    isDef = false;
  }

  //! virtual destructor
  virtual ~Symbol() {}

  //! Adds usage location to the symbol
  //!
  //! \param loc Location
  //! \return void
  //!
  //!
  void AddUsed(const Location& loc)
  {
    if(!isUsed)
    {
      isUsed = true;
      loc_used = loc;
    }
  }

  //! Adds definition location to the symbol
  //!
  //! \param loc Location
  //! \return void
  //!
  //!
  void AddDef(const Location& loc)
  {
    if(!isDef)
    {
      isDef = true;
      loc_def = loc;
    }
  }

  //! Get type of the symbol
  const Type& SymbolType() const { return t; }

  //! Get location information about the symbol
  const Location& GetLocation() const { return loc; }

  //! Get name of the symbol
  const std::string& Name() const { return name; }

  //! Get arity support of the symbol
  size_t Arity() const { return arity; }

  //! Returns if the symbol is used
  bool IsDef() const { return isDef; }
  //! Returns if symbol is defined
  bool IsUsed() const { return isUsed; }

  //! Type of the symbol
  Type t;
  //! Name of the symbol
  std::string name;
  //! Location of first occurrence
  Location loc;

  //! Is symbol used
  bool isUsed;
  //! Location where the symbol is first used
  Location loc_used;

  //! Is symbol defined
  bool isDef;
  //! Location where the symbol is first defined
  Location loc_def;

  //! Arity associated with the symbol
  size_t arity;
}; // class Symbol

/**
 * Type of Symbol representing functions in GDL. This class provides simplicity
 * in usage while producing no overhead(static inheritance).
 *
 * @see Symbol
 */
struct FunctionSymbol : public Symbol
{
  //! Creates symbol representing Given function
  //!
  //! \param name Name of the function
  //! \param arity Arity associated with the function
  //! \param loc Location of first occurrence
  //!
  //!
  FunctionSymbol(const std::string& name, size_t arity, const Location& loc)
    : Symbol(Symbol::FUNCTION, name, loc)
  {
    this->arity = arity;
  }
}; // class FunctionSymbol

/**
 * Type of Symbol representing relations(predicates) in GDL. This class provides
 * simplicity in usage while producing no overhead(static inheritance).
 *
 * @see Symbol
 */
struct RelationSymbol : public Symbol
{
  //! Creates symbol representing Given relation(predicate)
  //!
  //! \param name Name of the relation(predicate)
  //! \param arity Arity associated with the relation
  //! \param loc Location of first occurrence
  //!
  //!
  RelationSymbol(const std::string& name, size_t arity, const Location& loc)
    : Symbol(Symbol::RELATION, name, loc)
  {
    this->arity = arity;
  }
}; // RelationSymbol

}; // namespace core
}; // namespace libgdl

inline std::ostream& operator<<(std::ostream& s,
                                const libgdl::core::Symbol& sym)
{
  if(sym.SymbolType() == libgdl::core::Symbol::RELATION)
    s << "Relation: ";
  else
    s << "Function: ";
  s << sym.Name() << "/" << libgdl::ToString(sym.Arity());
  return s;
}

#endif // SYMBOLS_HPP_INCLUDED
