/**
 * @file sfinae_utility.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of some  useful detection functions.
 */
#ifndef _LIBGDL_CORE_UTIL_SFINAE_UTILITY_HPP_INCLUDED
#define _LIBGDL_CORE_UTIL_SFINAE_UTILITY_HPP_INCLUDED

#include <array>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>

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

#define HAS_MEM_FUNC(FUNC, NAME)                                               \
template<typename T, typename sig>                                             \
struct NAME {                                                                  \
  typedef char yes[1];                                                         \
  typedef char no [2];                                                         \
  template<typename U, U> struct type_check;                                   \
  template<typename _1> static yes &chk(type_check<sig, &_1::FUNC> *);         \
  template<typename   > static no  &chk(...);                                  \
  static bool const value = sizeof(chk<T>(0)) == sizeof(yes);                  \
};


#endif // _LIBGDL_CORE_UTIL_SFINAE_UTILITY_HPP_INCLUDED
