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
 * PrefixedOutStream support addition of time-stamp to every line. Time-stamp
 * will be prepended to every line including the prefix.
 * To enable timestamp -
 * @code
 * PrefixedOutStream stream(std::cout, "prefix ", false, true);
 * @endcode
 *
 * These objects are used for the libgdl::Log levels (DEBUG, INFO, WARN, and
 * FATAL).
 *
 * @see Timer
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
  //! \param timestamp To enable addition of time-stamp
  //!
  PrefixedOutStream(std::ostream& destination = std::cout,
                    const std::string& prefix = "",
                    bool ignoreInput = false,
                    bool timestamp = false) :
    destination(&destination),
    ignoreInput(ignoreInput),
    prefix(prefix),
    // We want the first call to operator<< to prefix the prefix so we set
    // carriageReturned to true.
    carriageReturned(true),
    timestamp(timestamp)
  {}

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

  //! Set output stream
  std::ostream& Stream() const { return *destination; }
  //! Get destination
  std::ostream* Destination() const { return destination; }
  //! Set output stream
  void SetStream(std::ostream& stream) { destination = &stream; }
  //! Get ignoreInput
  bool IgnoreInput() const { return ignoreInput; }
  //! Get prefix
  std::string Prefix() const { return prefix; }
  //! Get bool timestamp
  bool TimeStamp() const { return timestamp; }

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

  //! If timestamp needs to be printed before the prefix
  bool timestamp;
}; // class PrefixedOutStream

}; // namespace util
}; // namespace libgdl

// Template definitions.
#include "prefixedoutstream_impl.hpp"

#endif
