/**
 * @file log.hpp
 * @author Sumedh Ghaisas
 *
 * Definition of the Log class.
 */
#ifndef __LIBGDL_CORE_UTIL_LOG_HPP
#define __LIBGDL_CORE_UTIL_LOG_HPP

#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>

#include "prefixedoutstream.hpp"
#include "nulloutstream.hpp"

// Color code escape sequences
#define BASH_RED "\033[0;31m"
#define BASH_GREEN "\033[0;32m"
#define BASH_YELLOW "\033[0;33m"
#define BASH_CYAN "\033[0;36m"
#define BASH_CLEAR "\033[0m"

namespace libgdl
{
/**
 * Provides a convenient way to give formatted output.
 *
 * The Log class has four members which can be used in the same way ostreams can
 * be used:
 *
 *  - Log::Debug
 *  - Log::Info
 *  - Log::Warn
 *  - Log::Fatal
 *
 * Each of these will prefix a tag to the output (for easy filtering), and the
 * fatal output will terminate the program when a newline is encountered.  An
 * example is given below.
 *
 * @code
 * Log test;
 * test.Info << "Checking a condition." << std::endl;
 * if (!someCondition())
 *   Log::Warn << "someCondition() is not satisfied!" << std::endl;
 * test.Info << "Checking an important condition." << std::endl;
 * if (!someImportantCondition())
 * {
 *   Log.Fatal << "someImportantCondition() is not satisfied! Terminating.";
 *   Log.Fatal << std::endl;
 * }
 * @endcode
 *
 * Any messages sent to Log::Debug will not be shown when compiling in non-debug
 * mode.  Messages to Log::Info will only be shown when the --verbose flag is
 * given to the program.
 *
 * @see PrefixedOutStream, NullOutStream
 */
class Log
{
 public:
  Log(std::ostream& stream = std::cout)
    :
#ifdef DEBUG
      Debug(PrefixedOutStream(stream, BASH_CYAN "[DEBUG] " BASH_CLEAR)),
#else
      Debug(util::NullOutStream()),
#endif
      Info(util::PrefixedOutStream(stream,
                                   BASH_GREEN "[INFO ] " BASH_CLEAR,
                                   false /* unless --verbose */)),
      Warn(util::PrefixedOutStream(stream,
                                   BASH_YELLOW "[WARN ] " BASH_CLEAR,
                                   false)),
      Fatal(util::PrefixedOutStream(stream,
                                   BASH_RED "[FATAL] " BASH_CLEAR,
                                   false))
  {
  }

  /**
   * Checks if the specified condition is true.
   * If not, halts program execution and prints a custom error message.
   * Does nothing in non-debug mode.
   */
  static void Assert(bool condition,
                     const std::string& message = "Assert Failed.");

  // We only use PrefixedOutStream if the program is compiled with debug
  // symbols.
#ifdef DEBUG
  //! Prints debug output with the appropriate tag: [DEBUG].
  util::PrefixedOutStream Debug;
#else
  //! Dumps debug output into the bit nether regions.
  util::NullOutStream Debug;
#endif

  //! Prints informational messages if --verbose is specified, prefixed with
  //! [INFO ].
  util::PrefixedOutStream Info;

  //! Prints warning messages prefixed with [WARN ].
  util::PrefixedOutStream Warn;

  //! Prints fatal messages prefixed with [FATAL], then terminates the program.
  util::PrefixedOutStream Fatal;

  void SetStream(std::ostream& stream)
  {
#ifdef DEBUG
    Debug.Stream() = stream;
#endif
    Info.SetStream(stream);
    Warn.SetStream(stream);
    Fatal.SetStream(stream);
  }
};

}; //namespace libgdl

#endif
