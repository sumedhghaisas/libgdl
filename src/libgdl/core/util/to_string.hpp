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

//! This version of ToString is activated only when the class of the value(which
//! is deduced through template substitution) implements std::string() operator.
//!
//! \tparam T Type of the value to be printed
//! \param s Value to be printed
//! \return std::string
//!
//!
template<typename T> std::string
ToString(const T& s,
         typename boost::enable_if<sfinae::SupportsStringOperator<T> >::type* = 0)
{
  return std::string(s);
}

//! This version of ToString is activated only when the class of the value(which
//! is deduced through template substitution) DOES NOT implements
//! std::string() operator.
//!
//! \tparam T Type of the value to be printed
//! \param s Value to be printed
//! \return std::string
//!
//!
template<typename T> std::string
ToString(const T& s,
         typename boost::disable_if<sfinae::SupportsStringOperator<T> >::type* = 0)
{
  std::stringstream stream;
  stream << s;
  return stream.str();
}

}; // namespace libgdl

#endif // _LIBGDL_CORE_UTIL_TO_STRING_HPP_INCLUDED
