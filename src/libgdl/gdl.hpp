#ifndef _LIBGDL_GDL_HPP_INCLUDED
#define _LIBGDL_GDL_HPP_INCLUDED

#include <vector>
#include <string>
#include <set>
#include <boost/unordered_map.hpp>

#include <libgdl/core.hpp>
#include <libgdl/core/cache/lru_cache.hpp>
#include <libgdl/gdlparser/kif.hpp>
#include <libgdl/gdlreasoner/knowledgebase.hpp>

namespace libgdl
{

class GDL
{
  typedef std::vector<std::string> StringVec;
 public:
  //! Constructs a GDL object from KIF.
  //!
  //! \param kif& KIF object
  //! \param state_cache_capacity cache capacity for all caches
  //!
  //!
  GDL(gdlparser::KIF& kif, unsigned short state_cache_capacity = 1024);

  ~GDL()
  {
    delete init;
    for(std::list<Argument*>::const_iterator it = roles.begin();it != roles.end();it++)
      delete *it;
    delete id_map;
  }

  //! Returns the state of the game after performing given moves on the given
  //! state
  //! If useCache is true function returns the answer from cache if it exists
  //!
  //! \param state const State&
  //! \param moves const StringVec&
  //! \param true bool useCache=
  //! \return State
  //!
  //!
  State GetNextState(const State& state,
                     const std::vector<Argument*>& moves,
                     bool useCache = true);

  //! Returns if the given state is terminal.
  //! If useCache is true function returns the answer from cache if it exists
  //!
  //! \param state const State&
  //! \param true bool useCache
  //! \return bool
  //!
  //!
  bool IsTerminal(const State& state, bool useCache = true);
//
//  //! Returns all the legal move combinations possible in the given state
//  //! if you have 3 roles; first role has 4 possibility (ABCD), second role 3 possibilites (012),
//  //! third has 2 possibilities (XY) then function returns vector with following string vectors
//	//! A0X, A0Y, A1X, A1Y, A2X, A2Y, B0X, B0Y, B1X, B1Y, B2X, B2Y ... D0X, D0Y, D1X, D1Y, D2X, D2Y (size=24)
//	//! If useCache is true function returns the answer from cache if it exists
//  //!
//  //! \param state const State&
//  //! \param true bool useCache
//  //! \return std::vector<StringVec>
//  //!
//  //!
//  std::vector<StringVec> GetLegalJointMoves(const State& state, bool useCache = true) const;
//
//  //! Returns legal moves for the given role and given state
//  //! This function does not use cache
//  //!
//  //! \param state const State&
//  //! \param role const std::string&
//  //! \return StringVec
//  //!
//  //!
//  StringVec GetLegalMoves(const State& state, const std::string& role) const;
//
//    //! Returns goal value associated with the given state for given role
//    //! If useCache is true function returns the answer from cache if it exists
//    //!
//    //! \param state const State&
//    //! \param role const std::string&
//    //! \param true bool useCache
//    //! \return int
//    //!
//    //!
//    int GetGoal(const State& state, const std::string& role, bool useCache = true) const;
//
//    //! Returns goal value associated with the given state for given role id
//    //! If useCache is true function returns the answer from cache if it exists
//    //!
//    //! \param state const State&
//    //! \param rid size_t
//    //! \param true bool useCache=
//    //! \return int
//    //!
//    //!
//    int GetGoal(const State& state, size_t rid, bool useCache = true) const;
//
//    //! Returns a random move combination.
//    //!
//    //! \param state const State&
//    //! \param ran RandomNumberGenerator&
//    //! \return StringVec
//    //!
//    //!
//    StringVec GetRandomJointMove(const State& state, RandomNumberGenerator& ran, bool useCache = true) const;
//
//    //! Returns a random next state.
//    //!
//    //! \param state const State&
//    //! \param ran RandomNumberGenerator&
//    //! \return State
//    //!
//    //!
//    State GetRandomNextState(const State& state, RandomNumberGenerator& ran, bool useCache = true) const;
//
//    //! Returns the terminal state by applying random legal moves to the given state
//    //! till termination.
//    //!
//    //! \param state const State&
//    //! \param ran RandomNumberGenerator&
//    //! \param false bool useCache=
//    //! \return State
//    //!
//    //!
//    State PerformRandomDepthCharge(const State& state, RandomNumberGenerator& ran, bool useCache = true) const;
//
//    KIF GetFlattenedKIF() const;
//
//    //! Get Compressed string if compression is on
//    //! else it returns the passed string
//    //!
//    //! \param str const std::string&
//    //! \return std::string
//    //!
//    //!
//    std::string Compress(const std::string& str) const;
//
  //! returns the initial state of the GDL
  const State& InitState() const { return *init; }
//
//    //! returns roles involved in this GDL
//    const StringVec& GetRoles() const { return roles; }
//
//    static int hitCount;
//
//    static int sim_cache_capacity;



private:
  State* cached_GetNextState(const State& state,
                             const std::vector<Argument*>& moves);
  bool* cached_IsTerminal(const State& state);

  size_t StateMoveHash(const State& state,
                       const std::vector<Argument*>& moves);

  inline void ApplyState(const State& state);
  inline void ApplyActions(const std::vector<Argument*>& moves);
  inline void RemoveState();
  inline void RemoveActions();

  boost::unordered_map<std::string, size_t>* id_map;

  std::list<Argument*> roles;

  //! knowledge base representing the base rules of GDL
  mutable gdlreasoner::KnowledgeBase base_rules;

  //! initial state of the GDL
  State* init;

  size_t next_state_cache_capacity;
  cache::LRUCache<State, State> next_state_cache;

  size_t isTerminal_cache_capacity;
  cache::LRUCache<State, bool> isTerminal_cache;
};

inline void GDL::ApplyState(const State& state)
{
  const std::list<Argument*>& facts = state.facts;
  for(std::list<Argument*>::const_iterator it = facts.begin();it != facts.end();
                                                                          it++)
  {
    Argument *temp = new Argument;
    temp->val = "true";
    temp->t = Argument::Relation;
    temp->args.push_back(*it);

    Fact f;
    f.arg = temp;
    base_rules.m_facts["true/1"].push_back(std::move(f));
  }
}

inline void GDL::ApplyActions(const std::vector<Argument*>& moves)
{
  size_t r_index = 0;
  for(std::list<Argument*>::const_iterator it = roles.begin();it != roles.end();it++)
  {
    Argument *temp = new Argument;
    temp->val = "does";
    temp->t = Argument::Relation;
    temp->args.push_back(*it);
    temp->args.push_back(moves[r_index]);

    Fact f;
    f.arg = temp;
    base_rules.m_facts["does/2"].push_back(std::move(f));
  }
}

inline void GDL::RemoveState()
{
  gdlreasoner::KnowledgeBase::FactMap::iterator
                                      m_it = base_rules.m_facts.find("true/1");
  gdlreasoner::KnowledgeBase::FactVec& fvec = m_it->second;
  for(gdlreasoner::KnowledgeBase::FactVec::iterator it = fvec.begin();
                                                        it != fvec.end();it++)
  {
    Fact& f = *it;
    f.arg->args.clear();
  }
  base_rules.m_facts.erase(m_it);
}

inline void GDL::RemoveActions()
{
  gdlreasoner::KnowledgeBase::FactMap::iterator
                                      m_it = base_rules.m_facts.find("does/2");
  gdlreasoner::KnowledgeBase::FactVec& fvec2 = m_it->second;
  for(gdlreasoner::KnowledgeBase::FactVec::iterator it = fvec2.begin();
                                                        it != fvec2.end();it++)
  {
    Fact& f = *it;
    f.arg->args.clear();
  }
  base_rules.m_facts.erase(m_it);
}

};

#endif // _EPICURUS_GDL_HPP_INCLUDED
