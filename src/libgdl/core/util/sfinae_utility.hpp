/**
 * @file sfinae_utility.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of some  useful detection functions.
 */
#ifndef _LIBGDL_CORE_UTIL_SFINAE_UTILITY_HPP_INCLUDED
#define _LIBGDL_CORE_UTIL_SFINAE_UTILITY_HPP_INCLUDED

namespace libgdl
{
namespace sfinae
{

template <class T>
struct SupportsStringOperator
{
  template <class U>
  static auto string_test(const U* u) -> decltype(std::string(*u), char(0))
  { }

  static std::array<char, 2> string_test(...) { return std::array<char, 2>(); }

  static const bool value = (sizeof(string_test((T*)0)) == 1);
};

}
}


#endif // _LIBGDL_CORE_UTIL_SFINAE_UTILITY_HPP_INCLUDED
