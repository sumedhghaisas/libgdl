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

#include "argument.hpp"
#include "location.hpp"

namespace libgdl
{

/**
 * Represents a Clause.
 * Clause is stored as Head and its arguments(body).
 * Text representation is also stored for easy printing.
 *
 * @see KIFDriver
 */
struct Clause
{
  typedef gdlparser::parser::yy::location location_type;
  typedef gdlparser::parser::TokenValue TokenValue;

  //! empty constructor
  Clause() : head(NULL), isLocation(false) {}
  //! copy constructor
  Clause(const Clause& c);
  //! move constructor
  Clause(Clause&& c) noexcept
    : head(NULL) { swap(*this, c); }

  //! constructs clause from scanner token
  Clause(const TokenValue& tok, const size_t id);
  //! construct clause from string
  Clause(const std::string& str);

  //! destructor
  ~Clause();

  //! swap function
  friend void swap(Clause& c1, Clause& c2)
  {
    using std::swap;

    swap(c1.head, c2.head);
    swap(c1.premisses, c2.premisses);
    swap(c1.id, c2.id);
    swap(c1.isLocation, c2.isLocation);
    swap(c1.loc, c2.loc);
  }

  //! copy-assignment operator
  Clause& operator=(const Clause& c);
  //! move-assignment operator
  Clause& operator=(Clause&& c) { swap(*this, c); return *this; }

  void AddLocation(const location_type& l)
  {
    isLocation = true;
    loc = Location(l);
  }

  bool IsGround();

  //! Head of the clause
  Argument* head;
  //! body
  std::vector<Argument*> premisses;
  //! to assign unique id
  size_t id;

  bool isLocation;
  Location loc;
}; // class Clause

}; // namespace libgdl

inline std::ostream& operator<<(std::ostream& o, const libgdl::Clause& clause)
{
  o << "(<= ";
  o << *clause.head << " ";
  for(size_t i = 0;i < clause.premisses.size();i++)
  o << *clause.premisses[i] << " ";
  o << ")";
  return o;
}



#endif // _LIBGDL_CORE_DATATYPES_CLAUSE_HPP_INCLUDED
