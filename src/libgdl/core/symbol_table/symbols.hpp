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
  //! Create symbol with given information
  Symbol(const std::string& uname,
         const std::string& name,
         const Location& loc)
    : uname(uname), name(name), loc(loc)
  {
    isUsed = false;
    isDef = false;
  }

  //! virtual destructor
  ~Symbol() {}

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

  //! Get location information about the symbol
  const Location& GetLocation() const { return loc; }

  //! Get unique name
  const std::string& UName() const
  {
    return uname;
  }

  //! Get name of the symbol
  const std::string& Name() const { return name; }

  //! Returns if the symbol is used
  bool IsDef() const { return isDef; }
  //! Returns if symbol is defined
  bool IsUsed() const { return isUsed; }

  //! Unique Name of the symbol
  std::string uname;
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
}; // class Symbol


inline std::ostream& operator<<(std::ostream& s,
                                const libgdl::core::Symbol& sym)
{
  s << sym.Name() << "/" << sym.UName();
  return s;
}


}; // namespace core
}; // namespace libgdl

#endif // SYMBOLS_HPP_INCLUDED
