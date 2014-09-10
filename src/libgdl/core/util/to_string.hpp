/**
 * @file to_string.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration for libgdl::ToString.
 */
#ifndef _LIBGDL_CORE_UTIL_TO_STRING_HPP_INCLUDED
#define _LIBGDL_CORE_UTIL_TO_STRING_HPP_INCLUDED

#include <sstream>
#include <string>

#include "sfinae_utility.hpp"

namespace libgdl
{

//! Call std::string() if implemented
template<typename T> std::string
ToString(const T& s,
         typename boost::enable_if<sfinae::SupportsStringOperator<T> >::type* = 0)
{
  return std::string(s);
}

template<typename T> std::string
ToString(const T& s,
         typename boost::disable_if<sfinae::SupportsStringOperator<T> >::type* = 0)
{
  std::stringstream stream;
  stream << s;
  return stream.str();
}

}


#endif // _LIBGDL_CORE_UTIL_TO_STRING_HPP_INCLUDED
