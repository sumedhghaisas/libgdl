#ifndef LIBGDL_CORE_DATA_TYPES_AMOVE_HPP_INCLUDED
#define LIBGDL_CORE_DATA_TYPES_AMOVE_HPP_INCLUDED

#include <iostream>
#include <string>
#include <atomic>
#include <list>
#include <set>
#include <boost/intrusive_ptr.hpp>

#include <libgdl/core/util/logid.hpp>

#include "a_state.hpp"

namespace libgdl
{
namespace core
{

struct RawAMove
{
  RawAMove()
    : moves(new size_t[n_roles]), count(0u) {}

  ~RawAMove()
  {
    delete[] moves;
  }

  RawAMove(const RawAMove& rm)
    : moves(new size_t[n_roles]), count(0u)
  {
    for(size_t i = 0;i < n_roles;i++)
    {
      moves[i] = rm.moves[i];
    }
  }

  RawAMove(const std::list<size_t>& m)
    : moves(new size_t[n_roles]), count(0u)
  {
    auto it = m.begin();
    for(size_t i = 0;i < n_roles;i++)
    {
      moves[i] = *it;
      it++;
    }
  }

  RawAMove(const std::list<std::string>& s_moves);

  RawAMove(size_t i)
    : moves(new size_t[n_roles]), count(0u)
  {
    moves[0] = i;
  }

  inline void Get(size_t r_id, size_t in_id, bool& out) const
  {
    if(moves[r_id] == in_id) out = true;
    else out = false;
  }

  inline void Set(size_t r_id, size_t m_id)
  {
    moves[r_id] = m_id;
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

} // namespace core

struct AMove : public boost::intrusive_ptr<core::RawAMove>
{
  typedef core::RawAMove RawType;

  AMove(core::RawAMove* rm = NULL) : boost::intrusive_ptr<core::RawAMove>(rm) {}

  AMove(const std::string&) : boost::intrusive_ptr<core::RawAMove>(new core::RawAMove()) {}

  AMove(const std::list<size_t>& l)
    : boost::intrusive_ptr<core::RawAMove>(new core::RawAMove(l)) {}

  AMove(const std::list<std::string>& s_moves)
    : boost::intrusive_ptr<core::RawAMove>(new core::RawAMove(s_moves)) {}

  AMove(size_t i)
    : boost::intrusive_ptr<core::RawAMove>(new core::RawAMove(i)) {}

  AMove Clone() const
  {
    return AMove(new core::RawAMove(*get()));
  }

  void Get(size_t r_id, size_t in_id, bool& out) const
  {
    return get()->Get(r_id, in_id, out);
  }

  size_t GetMove(size_t r_id) const
  {
    return get()->GetMove(r_id);
  }

  template<typename NodeType>
  void UpdateNodes(AState& base, AState& top, AMove& m, NodeType*** input_nodes, std::set<size_t>* m_set, size_t* goals) const
  {
    for(size_t i = 0;i < get()->n_roles;i++)
    {
      if(get()->moves[i] != m->moves[i])
      {
        input_nodes[i][m->moves[i]]->Update(false, base, top, m, m_set, goals);
        input_nodes[i][get()->moves[i]]->Update(true, base, top, m, m_set, goals);
      }
    }
  }

  void Set(size_t r_id, size_t m_id)
  {
    get()->Set(r_id, m_id);
  }

  static void InitializeCreateMove(AMove (*cm)(const std::list<std::string>&))
  {
    CreateMove = cm;
    isCreateMoveInitialized = true;
  }

  static void InitializePrint(void (*p)(std::ostream&, const AMove&))
  {
    Print = p;
    isPrintInitialized = true;
  }

  static void PrintMove(std::ostream&, const AMove& move);

  static AMove ParseString(const std::string& str_moves);

  static bool isCreateMoveInitialized;
  static AMove(*CreateMove)(const std::list<std::string>&);

  static bool isPrintInitialized;
  static void (*Print)(std::ostream&, const AMove&);
};

inline std::ostream& operator<<(std::ostream& stream, const AMove& m)
{
  AMove::PrintMove(stream, m);
  return stream;
}

} //namespace libgdl
#endif //LIBGDL_CORE_DATA_TYPES_AMOVE_HPP_INCLUDED
