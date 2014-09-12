/**
 * @file intrusive_list.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of IntrusiveList.
 */
#ifndef _LIBGDL_CORE_DATATYPES_INTRUSIVE_LIST_HPP_INCLUDED
#define _LIBGDL_CORE_DATATYPES_INTRUSIVE_LIST_HPP_INCLUDED

#include <list>
#include <atomic>

namespace libgdl
{
namespace core
{

/**
 * IntrusiveList represents list which supports intrusive pointer.
 * It inherits all the basic functionality from std::list
 */
template<typename T>
class IntrusiveList : public std::list<T>
{
 public:
  //! Constructor which assigns reference count to 0
  //!
  //!
  IntrusiveList()
    : count(0u) {}

  //! Get reference count
  //!
  //! \return size_t
  //!
  //!
  size_t ref_count() { return count; }

  //! Refernce count stored in atomic variable (useful for multi-threading)
  std::atomic_size_t count;
}; // class IntrusiveList


//! Intrusive pointer release function for IntrusiveList
//! Decrements reference count of IntrusiveList object
//!
//! \param p object to decrement
//!
//!
template<typename T>
inline void intrusive_ptr_release(IntrusiveList<T>* p)
{
   if (--p->count == 0u)
        delete p;
}

//! Intrusive pointer add function for IntrusiveList
//! Increments reference count
//!
//! \param p object to increment
//!
//!
template<typename T>
inline void intrusive_ptr_add_ref(IntrusiveList<T>* p)
{
  ++p->count;
}

}; // namespace core
}; // namespace libgdl

#endif // _LIBGDL_CORE_DATATYPES_INTRUSIVE_LIST_HPP_INCLUDED
