/**
 * @file location.hpp
 * @author Sumedh Ghaisas
 *
 * Data structure to store location of any identifier.
 */
#ifndef _LIBGDL_GDLPARSER_LOCATION_HPP_INCLUDED
#define _LIBGDL_GDLPARSER_LOCATION_HPP_INCLUDED

#include <string>

#include "location.hh"

namespace gdlparser
{
namespace parser
{

/**
 * This structure represents location of the identifier. It stores the filename
 * and lineNo for tracking. Location is used in debugging KnowledgeBase. Location
 * is used in place of location_type (implicitly defined by gdlparser) so
 * that filename (the string variable) will be destroyed properly. location_type
 * class holds pointer to filename (string) which sometimes create problems.
 */
struct Location
{
  //! some useful typedefs
  typedef parser::yy::location location_type;

  //! empty constructor
  Location() {}

  //! constructs location object from location_type of parser
  Location(const location_type& loc)
  {
    filename = *loc.begin.filename;
    lineNo = loc.begin.line;
  }

  //! holds filename
  std::string filename;
  //! holds line number
  size_t lineNo;
}; // struct Location

}; // namespace parser
}; // namespace gdlparser

inline std::ostream& operator<<(std::ostream& o,
                                const gdlparser::parser::Location& loc)
{
  o << loc.lineNo << " \"" << loc.filename << "\"";
  return o;
}

#endif // _LIBGDL_GDLPARSER_LOCATION_HPP_INCLUDED
