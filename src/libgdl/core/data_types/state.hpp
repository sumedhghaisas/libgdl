/**
 * @file state.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of State which is used to represent the state of the game.
 */
#ifndef _LIBGDL_GDL_STATES_HPP_INCLUDED
#define _LIBGDL_GDL_STATES_HPP_INCLUDED

#include <atomic>
#include <list>
#include <string>
#include <boost/unordered_map.hpp>
#include <boost/intrusive_ptr.hpp>

#include <libgdl/core/data_types/argument.hpp>

namespace libgdl
{
namespace core
{

/**
 * Represents a state of the game. This representation of the state stores the
 * base propositions of the game true in this current state. This representation
 * can be applied to the knowledge base faster but can be a little memory
 * overhead.
 *
 * @see KnowledgeBase
 */
struct RawState
{
  //! Generates a state out of a existing vector of facts
  //! if isSorted is false facts are sorted before adding
  //! if isSorted is true vector is assumed to be sorted
  RawState (const std::list<Argument*> & facts,
         const boost::unordered_map<std::string, size_t>& id_map);

  RawState(const RawState& s);

  RawState& operator=(const RawState& state);

  ~RawState()
  {
    for(std::list<Argument*>::const_iterator it = facts.begin();it != facts.end();it++)
      delete *it;
  }

  //! Returns the hash value of this state
  //! States with identical set of true base proposition will have
  //! same value of hash.
  const size_t& GetHash() const { return hash; }

  //! comparison operators for State class
  bool operator==(const RawState& s) const
  {
    if (hash != s.GetHash()) return false;
    return true;
  }
  bool operator!=(const RawState& s) const {return !(*this == s); }

  //! Returns true base propositions
  const std::list<Argument*>& Facts() const { return facts; }

  //! computes and stores hash
  void calcHash(const boost::unordered_map<std::string, size_t>& id_map);

  //! Hash value of the state
  size_t hash;
  //! list of base propositions (sorted)
  std::list<Argument*> facts;

  size_t ref_count() { return count; }

  std::atomic_size_t count;
}; // struct State

inline void intrusive_ptr_release(core::RawState* p)
{
   if (--p->count == 0u)
        delete p;
}

inline void intrusive_ptr_add_ref(core::RawState* p)
{
  ++p->count;
}

}; // namespace core

struct State : public boost::intrusive_ptr<core::RawState>
{
  State(core::RawState* s)
    : boost::intrusive_ptr<core::RawState>(s) {}

  const size_t& GetHash() const { return get()->GetHash(); }

  const std::list<core::Argument*>& Facts() const { return get()->facts; }
  std::list<core::Argument*>& Facts() { return get()->facts; }

  bool operator==(const State& s) { return *get() == *s.get(); }
  bool operator!=(const State& s) { return *get() != *s.get(); }
};

}; // namespace libgdl

inline std::ostream& operator<<(std::ostream & ss,
                                const libgdl::core::RawState& s)
{
  ss << "State: ";
  for(std::list<libgdl::core::Argument*>::const_iterator it = s.facts.begin();
                                                    it != s.facts.end();it++)
  {
    ss << "\t" << **it << std::endl;
  }

  ss << "\tHash = " << std::hex << s.hash << std::dec;
  return ss;
}

inline std::ostream& operator<<(std::ostream& ss, const libgdl::State& s)
{
  ss << *s << std::endl;
  return ss;
}

#endif // _LIBGDL_GDL_STATES_HPP_INCLUDED
