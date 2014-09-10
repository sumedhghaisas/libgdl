#ifndef _LIBGDL_CORE_DATATYPES_INTRUSIVE_LIST_HPP_INCLUDED
#define _LIBGDL_CORE_DATATYPES_INTRUSIVE_LIST_HPP_INCLUDED

#include <list>
#include <atomic>

namespace libgdl
{
namespace core
{

template<typename T>
class IntrusiveList : public std::list<T>
{
public:
  IntrusiveList() : count(0u) {}
  size_t ref_count() { return count; }

  std::atomic_size_t count;
};

template<typename T>
inline void intrusive_ptr_release(IntrusiveList<T>* p)
{
   if (--p->count == 0u)
        delete p;
}

template<typename T>
inline void intrusive_ptr_add_ref(IntrusiveList<T>* p)
{
  ++p->count;
}

}; // namespace core
}; // namespace libgdl

#endif // _LIBGDL_CORE_DATATYPES_INTRUSIVE_LIST_HPP_INCLUDED
