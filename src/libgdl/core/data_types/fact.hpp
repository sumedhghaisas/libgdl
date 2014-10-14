/**
 * @file fact.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of Fact data type.
 */
#ifndef _LIBGDL_CORE_DATATYPES_FACT_HPP_INCLUDED
#define _LIBGDL_CORE_DATATYPES_FACT_HPP_INCLUDED

#include <string>
#include <vector>

#include <libgdl/core/symbol_table/symbol_table.hpp>

#include "argument.hpp"
#include "location.hpp"

namespace libgdl
{
namespace core
{

/**
 * Represents a fact.
 * Fact is stored as command and its arguments.
 *
 * @see Argument, KIFDriver
 */
struct Fact
{
  //! Empty constructor
  //!
  //!
  //!
  Fact()
    : arg(NULL), isLocation(false) {}

  //! Copy constructor (deep constructor)
  //! Throws no exception, can be used by STL wrappers
  //!
  //! \param fact Fact to copy from
  //!
  //!
  Fact(const Fact& fact) noexcept
    : arg(new Argument(*fact.arg)), isLocation(fact.isLocation), loc(fact.loc) {}

  //! Move constructor
  //!
  //! \param fact Fact to move from
  //!
  //!
  Fact(Fact&& fact) noexcept
  {
    arg = NULL;
    swap(*this, fact);
  }

  //! Construct a fact from argument (deep copy)
  //! Does not check if argument has variables or not
  //!
  //! \param arg Argument to copy
  //!
  //!
  Fact(const Argument& arg)
    : arg(new Argument(arg)) {}

  //! Construct fact from string
  //!
  //! \param str string representation of fact
  //! \param symbol_table SymbolTable to encode
  //! \param log Logging stream
  //! \return
  //!
  //!
  Fact(const std::string& str,
       SymbolTable& symbol_table,
       Log log = GLOBAL_LOG);

  //! destructor
  ~Fact() { delete arg; }

  //! copy-assignment operator
  Fact& operator=(const Fact& f);
  //! move-assignment operator
  Fact& operator=(Fact&& f) { swap(*this, f); return *this; }

  //! Swap function
  //!
  //! \param f1
  //! \param f2
  //! \return void
  //!
  //!
  friend void swap(Fact& f1, Fact& f2)
  {
    using std::swap;

    swap(f1.arg, f2.arg);
    swap(f1.isLocation, f2.isLocation);
    swap(f1.loc, f2.loc);
  }

  //! Assign location to this fact
  //!
  //! \param l Location object to assign
  //! \return void
  //!
  //!
  void AddLocation(const Location& l)
  {
    isLocation = true;
    loc = Location(l);
  }

  //! Get arguments of this fact
  //!
  //! \return const std::vector<Argument*>&
  //!
  //!
  const std::vector<Argument*>& Arguments() const { return arg->args; }

  //! comparison operators
  bool operator==(const Fact& fact) const;
  //! comparison operators
  bool operator!=(const Fact& fact) const { return !(*this == fact); }

  //! Returns string representation of this fact using the symbol table
  //! This function is used by SymbolDecodeStream to print Fact
  //!
  //! \param symbol_table const SymbolTable&
  //! \return std::string
  //!
  //! @see SymbolDecodeStream
  //!
  std::string DecodeToString(const SymbolTable& symbol_table) const
  {
    return arg->DecodeToString(symbol_table);
  }

  //! fact is represented as argument
  Argument* arg;
  //! is location assigned
  bool isLocation;
  //! location value
  Location loc;
}; // struct Fact

}; // namespace core
}; // namespace libgdl

inline std::ostream& operator<<(std::ostream& o,
                                const libgdl::core::Fact& f)
{
  o << *f.arg;
  return o;
}


#endif // _LIBGDL_CORE_FACT_HPP_INCLUDED
