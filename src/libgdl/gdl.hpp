/**
 * @file gdl.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of GDL class.
 */
#ifndef _LIBGDL_GDL_HPP_INCLUDED
#define _LIBGDL_GDL_HPP_INCLUDED

#include <vector>
#include <string>
#include <set>
#include <boost/unordered_map.hpp>
#include <iostream>

#include <libgdl/core.hpp>
#include <libgdl/core/cache/lru_cache.hpp>
#include <libgdl/gdlparser/kif.hpp>
#include <libgdl/gdlreasoner/knowledgebase.hpp>

namespace libgdl
{

class GDL
{
  template<typename T>
  using intrusive_ptr = boost::intrusive_ptr<T>;

  typedef core::Argument Argument;
  typedef core::Fact Fact;
  typedef core::Clause Clause;
  typedef core::SymbolTable SymbolTable;

 public:
  GDL(const std::string& filename,
      size_t state_cache_capacity = 1024,
      const Log& log = Log());

  //! Constructs a GDL object from KIF.
  //!
  //! \param kif& KIF object
  //! \param state_cache_capacity cache capacity for all caches
  //!
  //!
  GDL(gdlparser::KIF& kif,
      size_t state_cache_capacity = 1024,
      const Log& log = Log());


  ~GDL()
  {
    delete init;
    for(std::list<Argument*>::const_iterator it = roles.begin();
                                                        it != roles.end();it++)
      delete *it;
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
                     const Move& moves,
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

  //! Returns all the legal move combinations possible in the given state
  //! if you have 3 roles; first role has 4 possibility (ABCD), second role 3
  //! possibilites (012), third has 2 possibilities (XY) then function returns
  //! vector with following Move A0X, A0Y, A1X, A1Y, A2X, A2Y, B0X, B0Y, B1X,
  //! B1Y, B2X, B2Y ... D0X, D0Y, D1X, D1Y, D2X, D2Y (size=24)
	//! If useCache is true function returns the answer from cache if it exists
  //!
  //! \param state const State&
  //! \param true bool useCache
  //! \return std::list<Move>
  //!
  //!
  MoveList GetLegalMoves(const State& state, bool useCache = true);


  //! Returns goal value associated with the given state for given role
  //! If useCache is true function returns the answer from cache if it exists
  //!
  //! \param state const State&
  //! \param role const std::string&
  //! \param true bool useCache
  //! \return int
  //!
  //!
  size_t GetGoal(const State& state, const size_t role, bool useCache = true);

  //! returns the initial state of the GDL
  const State& InitState() const { return *init; }

  const SymbolTable* GetSymbolTable() const
  {
    return base_rules.GetSymbolTable();
  }
  SymbolTable*& GetSymbolTable()
  {
    return base_rules.GetSymbolTable();
  }

  Log& GetLog() { return log; }

private:
  State* cached_GetNextState(const State& state,
                             const Move& moves);
  size_t StateMoveHash(const State& state,
                       const Move& moves) const;

  bool* cached_IsTerminal(const State& state);

  MoveList* cached_getLegalMoves(const State& state);

  size_t* cached_getGoal(const State& state, const size_t rid);
  size_t StateRoleHash(const State& state, const size_t role) const;

  inline void ApplyState(const State& state);
  inline void ApplyActions(const Move& moves);
  inline void RemoveState();
  inline void RemoveActions();

  std::list<Argument*> roles;

  //! knowledge base representing the base rules of GDL
  mutable gdlreasoner::KnowledgeBase base_rules;

  //! initial state of the GDL
  State* init;

  size_t next_state_cache_capacity;
  cache::LRUCache<State, State> next_state_cache;

  size_t isTerminal_cache_capacity;
  cache::LRUCache<State, bool> isTerminal_cache;

  size_t getLegalMoves_cache_capacity;
  cache::LRUCache<State, MoveList> getLegalMoves_cache;

  size_t getGoal_cache_capacity;
  cache::LRUCache<State, size_t> getGoal_cache;

  mutable Log log;
};

inline void GDL::ApplyState(const State& state)
{
  const std::list<Argument*>& facts = state.Facts();
  for(std::list<Argument*>::const_iterator it = facts.begin();it != facts.end();
                                                                          it++)
  {
    Argument *temp = new Argument;
    temp->value = SymbolTable::TrueID;
    temp->t = Argument::Relation;
    temp->args.push_back(*it);

    Fact f;
    f.arg = temp;
    base_rules.m_facts[SymbolTable::TrueID].push_back(std::move(f));
  }
}

inline void GDL::ApplyActions(const Move& moves)
{
  size_t r_index = 0;
  for(std::list<Argument*>::const_iterator it = roles.begin();
                                                        it != roles.end();it++)
  {
    Argument *temp = new Argument;
    temp->value = SymbolTable::DoesID;
    temp->t = Argument::Relation;
    temp->args.push_back(*it);
    temp->args.push_back(moves.moves[r_index]);

    Fact f;
    f.arg = temp;
    base_rules.m_facts[SymbolTable::DoesID].push_back(std::move(f));
  }
}

inline void GDL::RemoveState()
{
  gdlreasoner::KnowledgeBase::FactMap::iterator
                            m_it = base_rules.m_facts.find(SymbolTable::TrueID);
  gdlreasoner::KnowledgeBase::FactList& fvec = m_it->second;
  for(gdlreasoner::KnowledgeBase::FactList::iterator it = fvec.begin();
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
                            m_it = base_rules.m_facts.find(SymbolTable::DoesID);
  gdlreasoner::KnowledgeBase::FactList& fvec2 = m_it->second;
  for(gdlreasoner::KnowledgeBase::FactList::iterator it = fvec2.begin();
                                                        it != fvec2.end();it++)
  {
    Fact& f = *it;
    f.arg->args.clear();
  }
  base_rules.m_facts.erase(m_it);
}

};

#endif // _EPICURUS_GDL_HPP_INCLUDED
