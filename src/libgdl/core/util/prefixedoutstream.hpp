/**
 * @file prefixedoutstream.hpp
 * @author Ryan Curtin
 * @author Matthew Amidon
 * @author Sumedh Ghaisas
 *
 * Declaration of the PrefixedOutStream class.
 */
#ifndef __LIBGDL_CORE_UTIL_PREFIXEDOUTSTREAM_HPP
#define __LIBGDL_CORE_UTIL_PREFIXEDOUTSTREAM_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include <streambuf>

namespace libgdl
{
namespace util
{

/**
 * Allows us to output to an ostream with a prefix at the beginning of each
 * line, in the same way we would output to cout or cerr.  The prefix is
 * specified in the constructor (as well as the destination ostream).  A newline
 * must be passed to the stream, and then the prefix will be prepended to the
 * next line.  For example,
 *
 * @code
 * PrefixedOutStream outstr(std::cout, "[TEST] ");
 * outstr << "Hello world I like " << 7.5;
 * outstr << "...Continue" << std::endl;
 * outstr << "After the CR\n" << std::endl;
 * @endcode
 *
 * would give, on std::cout,
 *
 * @code
 * [TEST] Hello world I like 7.5...Continue
 * [TEST] After the CR
 * [TEST]
 * @endcode
 *
 * These objects are used for the mlpack::Log levels (DEBUG, INFO, WARN, and
 * FATAL).
 */
class PrefixedOutStream
{
 public:
  //! Set up the PrefixedOutStream.
  //! If ignoreInput is true, the input won't be printed to the stream
  //!
  //! \param destination Output stream which receives output from this object.
  //! \param prefix The prefix to prepend to each line.
  //! \param ignoreInput To ignore the input povided
  //!
  PrefixedOutStream(std::ostream& destination,
                    const std::string& prefix = "",
                    bool ignoreInput = false) :
      destination(&destination),
      ignoreInput(ignoreInput),
      prefix(prefix),
      // We want the first call to operator<< to prefix the prefix so we set
      // carriageReturned to true.
      carriageReturned(true)
    { /* nothing to do */ }

  //! Write a streambuf to the stream.
  PrefixedOutStream& operator<<(std::streambuf* sb);
  //! Write an ostream manipulator function to the stream.
  PrefixedOutStream& operator<<(std::ostream& (*pf)(std::ostream&));
  //! Write an ios manipulator function to the stream.
  PrefixedOutStream& operator<<(std::ios& (*pf)(std::ios&));
  //! Write an ios_base manipulator function to the stream.
  PrefixedOutStream& operator<<(std::ios_base& (*pf)(std::ios_base&));

  //! Write anything else to the stream.
  template<typename T>
  PrefixedOutStream& operator<<(const T& s);

  //! Get output stream
  const std::ostream& Stream() { return *destination; }
  //! Set output stream
  std::ostream& Stream() const { return *destination; }
  //! Set output stream
  void SetStream(std::ostream& stream) { destination = &stream; }

 private:
   //! The output stream that all data is to be sent too; example: std::cout.
  std::ostream* destination;

  //! Discards input, prints nothing if true.
  bool ignoreInput;

  //! Conducts the base logic required in all the operator << overloads.
  //! Mostly just a good idea to reduce copy-pasta.
  //!
  //! @tparam T The type of the data to output.
  //! @param val The data to be output.
  //!
  template<typename T>
  void BaseLogic(const T& val);

  //! Output the prefix, but only if we need to and if we are allowed to.
  inline void PrefixIfNeeded();

  //! Contains the prefix we must prepend to each line.
  std::string prefix;

  //! If true, the previous call to operator<< encountered a CR, and a prefix
  //! will be necessary.
  bool carriageReturned;
}; // class PrefixedOutStream

}; // namespace util
}; // namespace libgdl

// Template definitions.
#include "prefixedoutstream_impl.hpp"

#endif
