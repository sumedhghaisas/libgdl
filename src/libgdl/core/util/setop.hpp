/**
 * @file setop.hpp
 * @author Sumedh Ghaisas
 *
 * Set operation functions.
 */
#ifndef _LIBGDL_CORE_UTIL_SETOP_HPP_INCLUDED
#define _LIBGDL_CORE_UTIL_SETOP_HPP_INCLUDED

#include <set>
#include <string>
#include <algorithm>

namespace libgdl
{
namespace util
{

/**
 * This class implements the useful set operations.
 */
class setop
{
 public:
  //! Returns the intersection of set a and b
  //!
  //! \param a const set&
  //! \param b const set&
  //! \return intersection of a and b
  //!
  //!
  template<typename T>
  static std::set<T> setIntersection(const std::set<T>& a, const std::set<T>& b)
  {
    std::set<T> intersection;
    std::set_intersection(a.begin(), a.end(), b.begin(), b.end(),
                          std::inserter(intersection, intersection.begin()));
    return intersection;
  }

  //! Returns the union of set a and b
  //!
  //! \param a const set&
  //! \param b const set&
  //! \return set union
  //!
  //!
  template<typename T>
  static std::set<T> setUnion(const std::set<T>& a, const std::set<T>& b)
  {
    std::set<T> u;
    std::set_union(a.begin(), a.end(), b.begin(), b.end(),
                   std::inserter(u, u.begin()));
    return u;
  }

  //! checks whether a is a subset of b
  //!
  //! \param a const set&
  //! \param b const set&
  //! \return bool
  //!
  //!
  template<typename T>
  static bool isSubset(const std::set<T>& a, const std::set<T>& b)
  {
    return std::includes(b.begin(), b.end(), a.begin(), a.end());
  }

  //! Returns set difference
  //!
  //! \param a const set&
  //! \param b const set&
  //! \return set difference
  //!
  //!
  template<typename T>
  static std::set<T> setDifference(const std::set<T>a, const std::set<T>& b)
  {
    std::set<T> difference;
    std::set_difference(a.begin(),a.end(),b.begin(),b.end(),
                        std::inserter(difference,difference.begin()));
    return difference;
  }
}; // class SetOp

}; // namespace util
}; // namespace libgdl

#endif // _LIBGDL_CORE_UTIL_SETOP_HPP_INCLUDED
