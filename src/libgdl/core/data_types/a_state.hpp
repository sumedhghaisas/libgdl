#ifndef LIBGDL_CORE_DATA_TYPES_STATE_HPP_INCLUDED
#define LIBGDL_CORE_DATA_TYPES_STATE_HPP_INCLUDED

#include <iostream>
#include <atomic>
#include <cmath>
#include <boost/intrusive_ptr.hpp>

namespace libgdl
{
namespace core
{

struct RawAState
{
  RawAState()
    : s(new char[arr_size]), count(0u)
  {
    for(size_t i = 0;i < arr_size;i++)
    {
      s[i] = 0;
    }
  };

  ~RawAState()
  {
    delete s;
  }

  RawAState(const RawAState& state)
    : s(new char[arr_size]), count(0u)
  {
    for(size_t i = 0;i < arr_size;i++)
    {
      s[i] = state.s[i];
    }
  }

  //! Get function
  void Get(size_t i, bool& out) const
  {
    size_t buff = i / 8;
    size_t p = pow(2, i % 8);

    char temp = s[buff] & p;

    if(temp != 0) out = true;
    else out = false;
  }

  //! Set function
  void Set(size_t i, bool val)
  {
    size_t buff = i / 8;
    size_t p = pow(2, i % 8);
    size_t q = 255 & ~p;

    if(val) s[buff] = s[buff] | p;
    else s[buff] = s[buff] & q;
  }

  void Print(std::ostream& stream) const;

  char *s;

  static size_t arr_size;

  size_t ref_count() { return count; }
  std::atomic_size_t count;
};

inline void intrusive_ptr_release(RawAState* p)
{
  if(--p->count == 0u) delete p;
}

inline void intrusive_ptr_add_ref(RawAState* p)
{
  ++p->count;
}

inline std::ostream& operator<<(std::ostream& stream, const RawAState& state)
{
  state.Print(stream);
  return stream;
}

} // namespace core

struct AState : public boost::intrusive_ptr<core::RawAState>
{
  AState() : boost::intrusive_ptr<core::RawAState>(NULL) {}
  AState(core::RawAState* state) : boost::intrusive_ptr<core::RawAState>(state) {}

  AState Clone() const
  {
    return AState(new core::RawAState(*get()));
  }
}; // struct AState

inline std::ostream& operator<<(std::ostream& stream, const AState& s)
{
  stream << *s;
  return stream;
}

}; // namespace libgdl
#endif //LIBGDL_CORE_DATA_TYPES_STATE_HPP_INCLUDED
