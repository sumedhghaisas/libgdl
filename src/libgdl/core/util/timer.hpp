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

//! Returns the time in microseconds
//!
//! \return uint64_t Time in microseconds
//!
//!
inline uint64_t microtimer()
{
  timeval t;
  gettimeofday (&t, NULL);
  return t.tv_sec * 1000000 + t.tv_usec;
}

}; // namespace libgdl

#endif // LIBGDL_CORE_UTIL_TIMER_HPP_INCLUDED
