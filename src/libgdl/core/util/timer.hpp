/**
 * @file timer.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration and implementation of timer function.
 */

#ifndef LIBGDL_CORE_UTIL_TIMER_HPP_INCLUDED
#define LIBGDL_CORE_UTIL_TIMER_HPP_INCLUDED

#include <time.h>       // clock_gettime()
#include <sys/time.h>   // timeval, gettimeofday()
#include <inttypes.h>   // int types for uint64_t

namespace libgdl
{

/**
 * This class handles all the time related queries of libGDL.
 */
class Timer
{
 public:
  //! Returns the time in microseconds
  //!
  //! \return uint64_t Time in microseconds
  //!
  //!
  static uint64_t microtimer()
  {
    timeval t;
    gettimeofday (&t, NULL);
    return t.tv_sec * 1000000 + t.tv_usec;
  }

  //! Returns current time-stamp as string
  //!
  //! \return std::string
  //!
  //!
  static std::string getTimeStamp()
  {
    std::time_t tm;
    std::time(&tm);
    std::string timestamp = ctime(&tm);
    timestamp[strlen(timestamp.c_str())-1] = ' ';
    return timestamp;
  }
}; // class Timer

}; // namespace libgdl

#endif // LIBGDL_CORE_UTIL_TIMER_HPP_INCLUDED
