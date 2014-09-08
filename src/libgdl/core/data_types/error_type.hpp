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
struct ErrorType
{
  struct Entry
  {
    Entry(const std::string& text, const Location& loc)
      : text(text), loc(loc) {}

    std::string GetString(size_t tabs) const
    {
      std::string out;
      if(loc != Location())
        out += ToString(loc) + ": ";
      for(size_t i = 0;i < tabs;i++)
        out += "\t";
      out += text;
      return out;
    }

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

inline std::ostream& operator<<(std::ostream& s,
                                const libgdl::ErrorType& error)
{
  bool isFirst = true;
  for(std::list<libgdl::ErrorType::Entry>::const_iterator
      it = error.entries.begin(); it!= error.entries.end();it++)
  {
    if(isFirst)
    {
      s << it->GetString(0) << std::endl;
      isFirst = false;
    }
    else s << it->GetString(1) << std::endl;
  }
  return s;
}

#define ARITY_ERROR(NAME, VAR, UARITY, DARITY, ULOC, DLOC)                    \
libgdl::ErrorType NAME;                                                       \
error.AddEntry("Trying to use " + VAR + " with arity " +                      \
               libgdl::ToString(UARITY) +                                     \
               " which is previously defined with arity " +                   \
               libgdl::ToString(DARITY), ULOC);                               \
error.AddEntry(VAR + " previously used here.", DLOC);

#define RF_ERROR(NAME, VAR, USED, DEF, ULOC, DLOC)                            \
libgdl::ErrorType NAME;                                                       \
error.AddEntry("Trying to use " + VAR + " as " + USED +                       \
               " which is previously used as " + DEF, ULOC);                  \
error.AddEntry(VAR + " previously used here.", DLOC);

#define Q_ERROR(NAME, TEXT, LOC)                                              \
libgdl::ErrorType NAME;                                                       \
error.AddEntry(TEXT, LOC);

#define PR_ARITY_ERROR(NAME, VAR, EXP, PROV, LOC)                             \
libgdl::ErrorType NAME;                                                       \
error.AddEntry("Relation " + VAR + " expects " + libgdl::ToString(EXP)        \
               + " arguments.", LOC);                                         \
error.AddEntry(libgdl::ToString(PROV) + " provided.", LOC);

#define SIMPLE_ERROR(NAME, TEXT)                                              \
libgdl::ErrorType NAME;                                                       \
error.AddEntry(TEXT, libgdl::Location());

#define INVALID_DEP_ERROR(NAME, VAR1, VAR2)                                   \
libgdl::ErrorType NAME;                                                       \
error.AddEntry("Invalid dependency", Location());                             \
error.AddEntry("Relation " + libgdl::ToString(VAR1) +                         \
               " is dependent on " + libgdl::ToString(VAR2), Location());

#endif // _LIBGDL_GDLPARSER_PARSER_ERROR_TYPE_HPP_INCLUDED
