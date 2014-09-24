/**
 * @file logid.hpp
 * @author Sumedh Ghaisas
 *
 * LOGID implementation
 */
#ifndef __LIBGDL_CORE_UTIL_LOGID_HPP_INCLUDED
#define __LIBGDL_CORE_UTIL_LOGID_HPP_INCLUDED

#include <string>
#include <sstream>

//! Caller for format_long
#define LOGIDXL                                                               \
    libgdl::util::Logid::format_long(__FILE__, __LINE__, __PRETTY_FUNCTION__)
//! Caller for format_short
#define LOGID                                                                 \
    libgdl::util::Logid::format_short(__FILE__, __LINE__, __PRETTY_FUNCTION__)

namespace libgdl
{
namespace util
{

/**
 * Implements function to print file name along with line number which helps
 * in debugging.
 *
 * @code
 * std::cout << LOGID << " something went wrong here" << std::endl;
 * @endcode
 *
 */
class Logid
{
 public:
  //! retruns a format like
  //! /path/test.cpp:47 TestFun
  //!
  //! \param filename Pointer to the filename
  //! \param line Line number
  //! \return std::string
  //!
  //!
  static std::string format_long(const char * filename,
                                 int line,
                                 const char * function)
  {
    std::ostringstream os;
    os << filename << ":" << line << " " << function << " ";
    return os.str ();
  }

  //! Throws away filename/line number
  //! Returns format like
  //! TestFun
  //!
  //! \param filename Pointer to the filename
  //! \param line Line number
  //! \return std::string
  //!
  //!
  static std::string format_short(const char * filename,
                                  int line,
                                  const char * function)
  {
    (void)function; // to remove unused warning
    std::ostringstream os;
    os << filename << "(" << line << "): ";
    return os.str ();
  }
}; // class Logid

}; // namespace util
}; // namespace libgdl

#endif
