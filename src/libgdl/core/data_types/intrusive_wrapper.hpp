/**
 * @file intrusive_wrapper.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of IntrusiveList.
 */
#ifndef _LIBGDL_CORE_DATATYPES_INTRUSIVE_WRAPPER_HPP_INCLUDED
#define _LIBGDL_CORE_DATATYPES_INTRUSIVE_WRAPPER_HPP_INCLUDED

#include <list>
#include <atomic>

namespace libgdl
{
namespace core
{

template<typename T>
class IntrusiveWrapper : public T
{
 public:
  //! Constructor which assigns reference count to 0
  //!
  //!
  IntrusiveWrapper()
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


template<typename T>
inline void intrusive_ptr_release(IntrusiveWrapper<T>* p)
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
inline void intrusive_ptr_add_ref(IntrusiveWrapper<T>* p)
{
  ++p->count;
}

}; // namespace core
}; // namespace libgdl

#endif // _LIBGDL_CORE_DATATYPES_INTRUSIVE_WRAPPER_HPP_INCLUDED
