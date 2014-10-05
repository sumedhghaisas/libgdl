/**
 * @file clause.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of Clause data structure.
 */
#ifndef _LIBGDL_CORE_DATATYPES_CLAUSE_HPP_INCLUDED
#define _LIBGDL_CORE_DATATYPES_CLAUSE_HPP_INCLUDED

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
 * Represents a Clause.
 * Clause is stored as Head and its arguments(body).
 *
 * @see Argument, Location
 */
struct Clause
{
  //! Empty constructor
  //!
  //!
  //!
  Clause()
    : head(NULL), isLocation(false) {}

  //! Copy constructor (deep copy)
  //! No exception is thrown, can be used by stl wrappers
  //!
  //! \param clause Clause to copy
  //!
  //!
  Clause(const Clause& clause) noexcept;

  //! Move constructor
  //! No exception is thrown, can be used by stl wrappers
  //!
  //! \param clause Clause to move from
  //!
  //!
  Clause(Clause&& clause) noexcept
    : head(NULL) { swap(*this, clause); }

  //! Construct clause from string
  //!
  //! \param str string representation of the clause
  //! \param symbol_table SymbolTable to encode
  //! \param log Logging stream
  //!
  //!
  Clause(const std::string& str,
         SymbolTable& symbol_table,
         Log log = std::cerr);

  //! destructor
  ~Clause();

  //! Swap function
  //!
  //! \param c1
  //! \param c2
  //! \return void
  //!
  //!
  friend void swap(Clause& c1, Clause& c2)
  {
    using std::swap;

    swap(c1.head, c2.head);
    swap(c1.premisses, c2.premisses);
    swap(c1.id, c2.id);
    swap(c1.isLocation, c2.isLocation);
    swap(c1.loc, c2.loc);
  }

  //! Copy-assignment operator (deep copy)
  //!
  //! \param clause Clause to copy from
  //! \return Clause&
  //!
  //!
  Clause& operator=(const Clause& clause);

  //! Move-assignment operator
  //!
  //! \param clause Clause to move from
  //! \return Clause&
  //!
  //!
  Clause& operator=(Clause&& clause) { swap(*this, clause); return *this; }

  //! Add Location to this clause
  //!
  //! \param l Location object to add
  //! \return void
  //!
  //!
  void AddLocation(const Location& l)
  {
    isLocation = true;
    loc = l;
  }

  //! Returns true if clause is ground (contains no variables)
  //!
  //! \return bool
  //!
  //!
  bool IsGround();

  void Reorder();


  //! Returns string representation of this clause using the symbol table
  //! This function is used by SymbolDecodeStream to print clause
  //!
  //! \param symbol_table const SymbolTable&
  //! \return std::string
  //!
  //! @see SymbolDecodeStream
  //!
  std::string DecodeToString(const SymbolTable& symbol_table) const;

  //! Head of the clause
  Argument* head;
  //! Body of the clause
  std::vector<Argument*> premisses;
  //! Used to assign unique ID
  size_t id;
  //! is location info present for this clause
  bool isLocation;
  //! represents location info for this clause
  Location loc;
}; // class Clause

}; // namespace core
}; // namespace libgdl

inline std::ostream& operator<<(std::ostream& o,
                                const libgdl::core::Clause& clause)
{
  o << "(<= ";
  o << *clause.head << " ";
  for(size_t i = 0;i < clause.premisses.size();i++)
  o << *clause.premisses[i] << " ";
  o << ")";
  return o;
}

#endif // _LIBGDL_CORE_DATATYPES_CLAUSE_HPP_INCLUDED
