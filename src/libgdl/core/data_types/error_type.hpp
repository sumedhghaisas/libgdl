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
namespace core
{

/**
 * Represents Error and Warning message created by KIF parser.
 *
 * @see KIFDriver, KIFScanner, KIFParser
 */
struct ErrorType
{
  /**
   * Error msg can contain mutiple entries with different locations.
   * This structure helps to keep them organized.
   */
  struct Entry
  {
    //! Construct entry from text and location
    //!
    //! \param text text of the entry
    //! \param loc location assigned to the entry
    //!
    //!
    Entry(const std::string& text, const Location& loc)
      : text(text), loc(loc) {}

    //! Get string representation of the entry
    //!
    //! \param tabs number tabs to add between location and text
    //! \return std::string
    //!
    //!
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

    //! text of the entry
    std::string text;
    //! location assigned to the entry
    Location loc;
  }; // struct Entry


  //! Add entry to this Error
  //!
  //! \param text Text of the entry
  //! \param loc Location Assigned to the entry
  //! \return void
  //!
  //!
  void AddEntry(const std::string& text, const Location& loc)
  {
    entries.push_back(Entry(text, loc));
  }

  //! Entries of the error
  std::list<Entry> entries;
}; // struct ErrorType

inline std::ostream& operator<<(std::ostream& s,
                                const ErrorType& error)
{
  bool isFirst = true;
  for(auto it = error.entries.begin(); it!= error.entries.end();it++)
  {
    if(isFirst)
    {
      s << it->GetString(0);
      isFirst = false;
    }
    else s << it->GetString(1);

    if(it != (--error.entries.end()))
      s << std::endl;
  }
  return s;
}

}; // namespace core
}; // namespace libgdl

//! create Arity difference error
#define ARITY_ERROR(NAME, VAR, UARITY, DARITY, ULOC, DLOC)                    \
libgdl::core::ErrorType NAME;                                                 \
error.AddEntry("Trying to use " + VAR + " with arity " +                      \
               libgdl::ToString(UARITY) +                                     \
               " which is previously defined with arity " +                   \
               libgdl::ToString(DARITY), ULOC);                               \
error.AddEntry(VAR + " previously used here.", DLOC);

//! create 'relation', 'function' difference error
#define RF_ERROR(NAME, VAR, USED, DEF, ULOC, DLOC)                            \
libgdl::core::ErrorType NAME;                                                 \
error.AddEntry("Trying to use " + VAR + " as " + USED +                       \
               " which is previously used as " + DEF, ULOC);                  \
error.AddEntry(VAR + " previously used here.", DLOC);

//! create single entry error with text and location
#define Q_ERROR(NAME, TEXT, LOC)                                              \
libgdl::core::ErrorType NAME;                                                 \
error.AddEntry(TEXT, LOC);

//! create error which represents change in predefined arity
#define PR_ARITY_ERROR(NAME, VAR, EXP, PROV, LOC)                             \
libgdl::core::ErrorType NAME;                                                 \
error.AddEntry("Relation " + VAR + " expects " + libgdl::ToString(EXP)        \
               + " arguments.", LOC);                                         \
error.AddEntry(libgdl::ToString(PROV) + " provided.", LOC);

//! create simple error with text
#define SIMPLE_ERROR(NAME, TEXT)                                              \
libgdl::core::ErrorType NAME;                                                 \
error.AddEntry(TEXT, libgdl::core::Location());

//! create invalid dependency error
#define INVALID_DEP_ERROR(NAME, VAR1, VAR2)                                   \
libgdl::core::ErrorType NAME;                                                 \
error.AddEntry("Invalid dependency", libgdl::core::Location());               \
error.AddEntry("Relation " + libgdl::ToString(VAR1) +                         \
               " is dependent on " + libgdl::ToString(VAR2),                  \
               libgdl::core::Location());

#endif // _LIBGDL_GDLPARSER_PARSER_ERROR_TYPE_HPP_INCLUDED
