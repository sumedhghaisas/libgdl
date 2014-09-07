/**
 * @file location.hpp
 * @author Sumedh Ghaisas
 *
 * Data structure to store location of any identifier.
 */
#ifndef _LIBGDL_CORE_DATATYPES_HPP_INCLUDED
#define _LIBGDL_CORE_DATATYPES_HPP_INCLUDED

#include <string>

#include <libgdl/gdlparser/parser/location.hh>

namespace libgdl
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
  typedef gdlparser::parser::yy::location location_type;
  typedef gdlparser::parser::yy::position position;

  //! empty constructor
  Location() : begin(&filename), end(&filename) {}
  //! copy constructor
  Location(const Location& loc)
    : filename(loc.filename),
      begin(&filename, loc.begin.line, loc.begin.column),
      end(&filename, loc.end.line, loc.end.column)
  {
    // empty constructor
  }
  //! constructs location object from location_type of parser
  Location(const location_type& loc)
    : filename(*loc.begin.filename),
      begin(&filename, loc.begin.line, loc.begin.column),
      end(&filename, loc.end.line, loc.end.column)
  {
    // empty constructor
  }

  friend void swap(Location& loc1, Location& loc2)
  {
    using std::swap;

    swap(loc1.filename, loc2.filename);
    swap(loc1.begin, loc2.end);
    swap(loc1.end, loc2.end);
  }

  Location& operator=(Location loc)
  {
    swap(*this, loc);
    return *this;
  }

  //! Comparison operators
  bool operator==(const Location& loc) const
  {
    if(filename != loc.filename) return false;
    if(begin != loc.begin) return false;
    if(end != loc.end) return false;
    return true;
  }
  bool operator!=(const Location& loc) const { return !(*this == loc); }

  //! holds filename
  std::string filename;

  position begin;
  position end;
}; // struct Location

}; // namespace libgdl


inline std::ostream& operator<<(std::ostream& ostr, const libgdl::Location& loc)
{
  unsigned int end_col = 0 < loc.end.column ? loc.end.column - 1 : 0;

  ostr << loc.begin;

  if (loc.end.filename
      && (!loc.begin.filename
          || *loc.begin.filename != *loc.end.filename))
  {
    ostr << '-' << loc.end.filename << ':' << loc.end.line << '.' << end_col;
  }
  else if (loc.begin.line < loc.end.line)
    ostr << '-' << loc.end.line << '.' << end_col;
  else if (loc.begin.column < end_col)
    ostr << '-' << end_col;
  return ostr;
}

#endif // _LIBGDL_CORE_HPP_INCLUDED
