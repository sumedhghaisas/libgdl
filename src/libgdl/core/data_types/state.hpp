/**
 * @file state.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of State.
 */
#ifndef _LIBGDL_GDL_STATES_HPP_INCLUDED
#define _LIBGDL_GDL_STATES_HPP_INCLUDED

#include <list>
#include <string>
#include <boost/unordered_map.hpp>

#include <libgdl/core/data_types/argument.hpp>

namespace libgdl
{

/**
 * Represents a state of the game. This representation of the state stores the
 * base propositions of the game true in this current state. This representation
 * can be applied to the knowledge base faster but can be a little memory
 * overhead.
 *
 * @see KnowledgeBase
 */
struct State
{
  //! Generates a state out of a existing vector of facts
  //! if isSorted is false facts are sorted before adding
  //! if isSorted is true vector is assumed to be sorted
  State (const std::list<Argument*> & facts,
         const boost::unordered_map<std::string, size_t>& id_map);

  State(const State& s);

  ~State()
  {
    for(std::list<Argument*>::const_iterator it = facts.begin();it != facts.end();it++)
      delete *it;
  }

  //! Returns the hash value o this state
  //! States with identical set of true base proposition will have
  //! same value of hash.
  const size_t& GetHash() const { return hash; }

  //! comparison operators for State class
  bool operator==(const State& s) const
  {
    if (hash != s.GetHash()) return false;
    return true;
  }
  bool operator!=(const State& s) const {return !(*this == s); }

  //! Returns true base propositions
  const std::list<Argument*>& Facts() const { return facts; }

  //! computes and stores hash
  void calcHash(const boost::unordered_map<std::string, size_t>& id_map);

  //! Hash value of the state
  size_t hash;
  //! list of base propositions (sorted)
  std::list<Argument*> facts;
}; // struct State

}; // namespace libgdl

inline std::ostream& operator<<(std::ostream & ss, const libgdl::State& s)
{
  ss << "State: ";
  for(std::list<libgdl::Argument*>::const_iterator it = s.facts.begin();
                                                    it != s.facts.end();it++)
  {
    ss << "\t" << **it << std::endl;
  }

  ss << "\tHash = " << std::hex << s.hash << std::dec;
  return ss;
}

#endif // _LIBGDL_GDL_STATES_HPP_INCLUDED
