#ifndef LIBGDL_CORE_DATA_TYPES_AMOVE_HPP_INCLUDED
#define LIBGDL_CORE_DATA_TYPES_AMOVE_HPP_INCLUDED

#include <iostream>
#include <atomic>
#include <list>
#include <boost/intrusive_ptr.hpp>

namespace libgdl
{
namespace core
{

struct RawAMove
{
  RawAMove()
    : moves(new size_t[n_roles] {}

  ~RawMove()
  {
    delete moves;
  }

  RawMove(const RawMove& rm)
    : moves(new size_t[n_roles]
  {
    for(size_t i = 0;i < n_roles;i++)
    {
      moves[i] = rm.moves[i];
    }
  }

  RawMove(const std::list<size_t>& m)
    : moves(new size_t[n_roles])
  {
    auto it = m.begin();
    for(size_t i = 0;i < n_roles;i++)
    {
      moves[i] = *it;
      it++;
    }
  }

  inline void Get(size_t r_id, size_t in_id, bool& out) const
  {
    if(moves[r_id] == in_id) out = true;
    else out = false;
  }

  size_t GetMove(size_t r_id) const
  {
    return moves[r_id];
  }

  void SetMove(size_t r_id, size_t m_id)
  {
    moves[r_id] = m_id;
  }

  size_t* moves;

  static size_t n_roles;

  size_t ref_count() { return count; }
  std::atomic_size_t count;
}; // struct RawAMove

inline void intrusive_ptr_release(RawAMove* p)
{
  if(--p->count == 0u) delete p;
}

inline void intrusive_ptr_add_ref(RawAMove* p)
{
  ++p->count;
}

std::ostream& operator<<(std::ostream& stream, const RawMove& move);

} // namespace core

struct AMove : public boost::intrusive_ptr<core::RawAMove>
{
  AMove(RawMove* rm = NULL) : boost::intrusive_ptr<RawAMove>(rm) {}

  AMove(const std::list<size_t>& l)
    : boost::intrusive_ptr<RawAMove>(new RawAMove(l)) {}

  AMove Clone() const
  {
    return Move(new RawMove(*get()));
  }

  void Get(size_t r_id, size_t in_id, bool& out)
  {
    return get()->Get(r_id, in_id, out);
  }

  size_t GetMove(size_t r_id) const
  {
    return get()->GetMove(r_id);
  }

  void SetMove(size_t r_id, size_t m_id)
  {
    get()->SetMove(r_id, m_id);
  }
};

std::ostream& operator<<(std::ostream& stream, const Move& m)
{
  stream << *m;
  return stream;
}

} //namespace libgdl
#endif //LIBGDL_CORE_DATA_TYPES_AMOVE_HPP_INCLUDED
