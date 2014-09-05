/**
 * @file error_type.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration ErrorType.
 */
#ifndef _LIBGDL_GDLPARSER_PARSER_ERROR_TYPE_HPP_INCLUDED
#define _LIBGDL_GDLPARSER_PARSER_ERROR_TYPE_HPP_INCLUDED

#include <iostream>
#include <string>

#include <libgdl/core/data_types/location.hpp>
#include <libgdl/core/util/to_string.hpp>

namespace libgdl
{
namespace gdlparser
{
namespace parser
{

struct ErrorType
{
  struct Entry
  {
    Entry(const std::string& text, const Location& loc)
      : text(text), loc(loc) {}

    std::string text;
    Location loc;
  };

  void AddEntry(const std::string& text, const Location& loc)
  {
    entries.push_back(Entry(text, loc));
  }

  std::list<Entry> entries;
};

}
}
}

inline std::ostream& operator<<(std::ostream& s,
                                const libgdl::gdlparser::parser::ErrorType::Entry& entry)
{
  s << entry.loc << ": " << entry.text;
  return s;
}

inline std::ostream& operator<<(std::ostream& s,
                                const libgdl::gdlparser::parser::ErrorType& error)
{
  for(std::list<libgdl::gdlparser::parser::ErrorType::Entry>::const_iterator
      it = error.entries.begin(); it!= error.entries.end();it++)
  {
    s << *it << std::endl;
  }
  return s;
}

#define ARITY_ERROR(NAME, VAR, UARITY, DARITY, ULOC, DLOC)                    \
libgdl::gdlparser::parser::ErrorType NAME;                                    \
error.AddEntry("Trying to use " + VAR + " with arity " +                      \
               libgdl::ToString(UARITY) +                                     \
               " which is previously defined with arity " +                   \
               libgdl::ToString(DARITY), ULOC);                               \
error.AddEntry(VAR + " previously used here.", DLOC);

#define RF_ERROR(NAME, VAR, USED, DEF, ULOC, DLOC)                            \
libgdl::gdlparser::parser::ErrorType NAME;                                    \
error.AddEntry("Trying to use " + VAR + " as " + USED +                       \
               " which is previously used as " + DEF, ULOC);                  \
error.AddEntry(VAR + " previously used here.", DLOC);

#endif // _LIBGDL_GDLPARSER_PARSER_ERROR_TYPE_HPP_INCLUDED
