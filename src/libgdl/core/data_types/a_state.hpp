#ifndef LIBGDL_CORE_DATA_TYPES_STATE_HPP_INCLUDED
#define LIBGDL_CORE_DATA_TYPES_STATE_HPP_INCLUDED

#include <iostream>
#include <atomic>
#include <cmath>
#include <set>
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
    delete[] s;
  }

  RawAState(const RawAState& state)
    : s(new char[arr_size]), count(0u)
  {
    for(size_t i = 0;i < arr_size;i++)
    {
      s[i] = state.s[i];
    }
  }

  void operator&(RawAState&& state) const
  {
    for(size_t i = 0;i < arr_size;i++)
      state.s[i] = state.s[i] & s[i];
  }

  //! Get function
  inline void Get(size_t i, bool& out) const
  {
    size_t buff = i / 8;
    size_t p = pow(2, i % 8);

    char temp = s[buff] & p;

    out = temp;
    //if(temp != 0) out = true;
    //else out = false;
  }

  //! Set function
  inline void Set(size_t i, bool val)
  {
    size_t buff = i / 8;
    size_t p = pow(2, i % 8);
    size_t q = 255 & ~p;

    if(val) s[buff] = s[buff] | p;
    else s[buff] = s[buff] & q;
  }

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

} // namespace core

struct AMove;

struct AState : public boost::intrusive_ptr<core::RawAState>
{
  typedef core::RawAState RawType;

  AState() : boost::intrusive_ptr<core::RawAState>(NULL) {}
  AState(const std::string&)
    : boost::intrusive_ptr<core::RawAState>(new core::RawAState()) {}
  AState(core::RawAState* state) : boost::intrusive_ptr<core::RawAState>(state) {}

  AState Clone() const
  {
    return AState(new core::RawAState(*get()));
  }

  inline void Equate(const AState& s)
  {
    for(size_t i = 0;i < core::RawAState::arr_size;i++)
      get()->s[i] = s->s[i];
  }

  inline AState operator&(AState&& state) const
  {
    get()->operator&(std::move(*state.get()));
    return state;
  }

  inline AState operator&(const AState& state) const
  {
    AState out = state.Clone();
    get()->operator&(std::move(*out.get()));
    return out;
  }

  inline operator bool() const
  {
    for(size_t i = 0;i < core::RawAState::arr_size;i++)
      if(get()->s[i] != 0)
        return true;
    return false;
  }

  AState operator~()
  {
    AState out("");
    for(size_t i = 0;i < core::RawAState::arr_size;i++)
    {
      out->s[i] = ~get()->s[i];
    }
    return out;
  }

  void Clear()
  {
    for(size_t i = 0;i < get()->arr_size;i++)
    {
      get()->s[i] = 0;
    }
  }

  //! Set function
  inline void Set(size_t i, bool val)
  {
    get()->Set(i, val);
  }

  //! Get function
  inline void Get(size_t i, bool& out) const
  {
    get()->Get(i, out);
  }

  template<typename NodeType>
  void UpdateNodes(AState& base, AState& top, AState& mask, AMove& m, NodeType** nodes, std::set<size_t>* m_set, size_t* goals) const
  {
    for(size_t i = 0;i < get()->arr_size;i++)
    {
      char x_or = get()->s[i] ^ base.get()->s[i];
      x_or = x_or & mask.get()->s[i];
      if(x_or)
      {
        char s_val = get()->s[i];
        if(x_or & 1)
          //if(nodes[8*i] != NULL)
            nodes[8*i]->Update((bool)(s_val & 1), base, top, m, m_set, goals);

        x_or = x_or >> 1;
        s_val = s_val >> 1;
        if(x_or & 1)
          //if(nodes[8*i + 1] != NULL)
            nodes[8*i + 1]->Update((bool)(s_val & 1), base, top, m, m_set, goals);

        x_or = x_or >> 1;
        s_val = s_val >> 1;
        if(x_or & 1)
          //if(nodes[8*i + 2] != NULL)
            nodes[8*i + 2]->Update((bool)(s_val & 1), base, top, m, m_set, goals);

        x_or = x_or >> 1;
        s_val = s_val >> 1;
        if(x_or & 1)
          //if(nodes[8*i + 3] != NULL)
            nodes[8*i + 3]->Update((bool)(s_val & 1), base, top, m, m_set, goals);

        x_or = x_or >> 1;
        s_val = s_val >> 1;
        if(x_or & 1)
          //if(nodes[8*i + 4] != NULL)
            nodes[8*i + 4]->Update((bool)(s_val & 1), base, top, m, m_set, goals);

        x_or = x_or >> 1;
        s_val = s_val >> 1;
        if(x_or & 1)
          //if(nodes[8*i + 5] != NULL)
            nodes[8*i + 5]->Update((bool)(s_val & 1), base, top, m, m_set, goals);

        x_or = x_or >> 1;
        s_val = s_val >> 1;
        if(x_or & 1)
          //if(nodes[8*i + 6] != NULL)
            nodes[8*i + 6]->Update((bool)(s_val & 1), base, top, m, m_set, goals);

        x_or = x_or >> 1;
        s_val = s_val >> 1;
        if(x_or & 1)
          //if(nodes[8*i + 7] != NULL)
            nodes[8*i + 7]->Update((bool)(s_val & 1), base, top, m, m_set, goals);
      }
    }
  }

  static void InitializePrint(void (*p)(std::ostream&, const AState&))
  {
    PrintAState = p;
    isPrintInitialized = true;
  }

  static void PrintState(std::ostream& stream, const AState& state);

  static bool isPrintInitialized;
  static void (*PrintAState)(std::ostream&, const AState&);
}; // struct AState

inline std::ostream& operator<<(std::ostream& stream, const AState& s)
{
  AState::PrintState(stream, s);
  return stream;
}

}; // namespace libgdl
#endif //LIBGDL_CORE_DATA_TYPES_STATE_HPP_INCLUDED
