/**
 * @file nulloutstream.hpp
 * @author Ryan Curtin
 * @author Matthew Amidon
 * @author Sumedh Ghaisas
 *
 * Definition of the NullOutStream class.
 */
#ifndef __LIBGDL_CORE_UTIL_NULLOUTSTREAM_HPP
#define __LIBGDL_CORE_UTIL_NULLOUTSTREAM_HPP

#include <iostream>
#include <streambuf>

namespace libgdl {
namespace util {

/**
 * Used for Log::Debug when not compiled with debugging symbols.  This class
 * does nothing and should be optimized out entirely by the compiler.
 *
 * @see Log
 */
class NullOutStream
{
 public:
  /**
   * Does nothing.
   */
  NullOutStream() { }

  /**
   * Does nothing.
   */
  NullOutStream(const NullOutStream& /* other */) { }

  //! Does nothing.
  NullOutStream& operator<<(std::streambuf*) { return *this; }
  //! Does nothing.
  NullOutStream& operator<<(std::ostream& (*) (std::ostream&)) { return *this; }
  //! Does nothing.
  NullOutStream& operator<<(std::ios& (*) (std::ios&)) { return *this; }
  //! Does nothing.
  NullOutStream& operator<<(std::ios_base& (*) (std::ios_base&))
  { return *this; }

  //! Does nothing.
  template<typename T>
  NullOutStream& operator<<(const T&) { return *this; }
};

} // namespace util
} // namespace libgdl

#endif
