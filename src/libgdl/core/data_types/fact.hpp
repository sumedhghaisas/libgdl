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
 * Text representation is also stored for easy printing.
 *
 * @see KIFDriver
 */
struct Fact
{
  //! some useful typedefs
  typedef gdlparser::parser::yy::location location_type;

  //! empty constructor
  Fact() : arg(NULL), isLocation(false) {}
  //! copy constructor
  Fact(const Fact& f)
    : arg(new Argument(*f.arg)), isLocation(f.isLocation), loc(f.loc) {}
  Fact(Fact&& f) noexcept
  {
    arg = NULL;
    swap(*this, f);
  }

  //! construct a fact fro argument
  //! does not check if argument has variables or not
  Fact(const Argument& arg) : arg(new Argument(arg)) {}
  //! construct fact from string
  Fact(const std::string& str,
       SymbolTable& symbol_table,
       Log log = std::cerr);

  //! desctructor
  ~Fact() { delete arg; }

  //! copy-assignment operator
  Fact& operator=(const Fact& f);
  //! move-assignment operator
  Fact& operator=(Fact&& f) { swap(*this, f); return *this; }


  friend void swap(Fact& f1, Fact& f2)
  {
    using std::swap;

    swap(f1.arg, f2.arg);
    swap(f1.isLocation, f2.isLocation);
    swap(f1.loc, f2.loc);
  }

  //! Assign location to this fact
  void AddLocation(const location_type& l)
  {
    isLocation = true;
    loc = Location(l);
  }

  //! get command name of this fact
  const std::string& Command() const { return arg->val; }

  //! get arguments of this fact
  const std::vector<Argument*>& Arguments() const { return arg->args; }

  //! comparison operators
  bool operator==(const Fact& fact) const;
  bool operator!=(const Fact& fact) const { return !(*this == fact); }

  std::string DecodeToString(const SymbolTable& symbol_table) const
  {
    return arg->DecodeToString(symbol_table);
  }

  //! fact as argument
  Argument* arg;

  //! is location assigned
  bool isLocation;
  //! location value
  Location loc;
}; // class Fact

}; // namespace core
}; // namespace libgdl

inline std::ostream& operator<<(std::ostream& o,
                                const libgdl::core::Fact& f)
{
  o << *f.arg;
  return o;
}


#endif // _LIBGDL_CORE_FACT_HPP_INCLUDED
