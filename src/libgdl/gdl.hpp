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
#include <libgdl/gdlreasoner/kif_flattener.hpp>

namespace libgdl
{

class GDL
{
  //! For simplicity
  typedef core::Argument Argument;
  typedef core::Fact Fact;
  typedef core::Clause Clause;
  typedef core::SymbolTable SymbolTable;

 public:
  //! Constructs GDL object from KIF file
  //!
  //! \param filename Filename of the file to load
  //! \param state_cache_capacity Capacity of the state cache
  //! \param log Logging stream
  //!
  //!
  GDL(const std::string& filename,
      size_t state_cache_capacity = 1024,
      const Log& log = Log());

  //! Constructs a GDL object from KIF.
  //!
  //! \param kif KIF object
  //! \param state_cache_capacity cache capacity for all caches
  //! \param log Logging stream
  //!
  //!
  GDL(gdlparser::KIF& kif,
      size_t state_cache_capacity = 1024,
      const Log& log = Log());

  //! Constructs a GDL object from flattened knowledge.
  //!
  //! \param kf KIFFlattener object
  //! \param state_cache_capacity cache capacity for all caches
  //! \param log Logging stream
  //!
  //!
  GDL(gdlreasoner::KIFFlattener& kf,
      size_t state_cache_capacity = 1024,
      const Log& log = Log());

  //! Destructor
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
  //! \param state State on which to perform the action
  //! \param moves Action to perform
  //! \param useCache To use cache
  //! \return State
  //!
  //!
  State GetNextState(const State& state,
                     const Move& moves,
                     bool useCache = true);

  //! Returns if the given state is terminal.
  //! If useCache is true function returns the answer from cache if it exists
  //!
  //! \param state State to check for termination
  //! \param useCache To use cache
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
  //! \param state State to process
  //! \param useCache To use cache
  //! \return std::list<Move>
  //!
  //!
  MoveList GetLegalMoves(const State& state, bool useCache = true);


  //! Returns goal value associated with the given state for given role
  //! If useCache is true function returns the answer from cache if it exists
  //!
  //! \param state State to process
  //! \param role Role of the given player
  //! \param useCache To use cache
  //! \return int
  //!
  //!
  size_t GetGoal(const State& state, const size_t role, bool useCache = true);

  //! Returns the initial state of the game
  const State& InitState() const { return *init; }

  //! Get the symbol table
  SymbolTable GetSymbolTable() const
  {
    return base_rules.GetSymbolTable();
  }

  //! Get-Set the logging stream
  Log& GetLog() { return log; }

private:
  //! The function to be called by cache when GetNextState produces cache miss
  //!
  //! \param state State on which to perform the action
  //! \param moves Action to perform
  //! \return State*
  //!
  //!
  State* cached_GetNextState(const State& state,
                             const Move& moves);

  //! Returns the combined Hash value of given State along with the actions
  //!
  //! \param state State of the game
  //! \param moves Moves to perform
  //! \return size_t
  //!
  //!
  size_t StateMoveHash(const State& state,
                       const Move& moves) const;

  //! The function to be called by cache when IsTerminal produces cache miss.
  //!
  //! \param state State of the game
  //! \return bool*
  //!
  //!
  bool* cached_IsTerminal(const State& state);

  //! The function to be called by cache when GetLegalMoves produces cache miss.
  //!
  //! \param state State of the game
  //! \return MoveList*
  //!
  //!
  MoveList* cached_getLegalMoves(const State& state);

  //! The function to be called by cach when GetGoal produces cache miss.
  //!
  //! \param state State of the game
  //! \param rid Role ID of the given role
  //! \return size_t*
  //!
  //!
  size_t* cached_getGoal(const State& state, const size_t rid);

  //! Returns the combined hash value of State along with the role ID.
  //!
  //! \param state State of the game
  //! \param role Role ID of the given role
  //! \return size_t
  //!
  //!
  size_t StateRoleHash(const State& state, const size_t role) const;

  //! Apply the given state to knowledge base
  //!
  //! \param state State to apply
  //! \return void
  //!
  //!
  inline void ApplyState(const State& state);

  //! Apply the given actions to knowledge base
  //!
  //! \param moves Actions to apply
  //! \return void
  //!
  //!
  inline void ApplyActions(const Move& moves);


  //! Remove the state representation from the knowledge base
  //!
  //! \return void
  //!
  //!
  inline void RemoveState();

  //! Remove the knowledge of actions from knowledge base
  //!
  //! \return void
  //!
  //!
  inline void RemoveActions();

  //! Roles in the playing game
  std::list<Argument*> roles;

  //! Knowledge base representing the rules of the game
  mutable gdlreasoner::KnowledgeBase base_rules;

  //! Initial state of the game
  State* init;

  //! Cache capacity of cache associated with function GetNextState
  size_t next_state_cache_capacity;
  //! Cache associated with function GetNextState
  cache::LRUCache<State, State> next_state_cache;

  //! Cache capacity of cache associated with function IsTerminal
  size_t isTerminal_cache_capacity;
  //! Cache associated with function IsTerminal
  cache::LRUCache<State, bool> isTerminal_cache;

  //! Cache capacity of cache associated with functions GetLegalMoves
  size_t getLegalMoves_cache_capacity;
  //! Cache associated with function GetLegalMoves
  cache::LRUCache<State, MoveList> getLegalMoves_cache;

  //! Cache capacity of cache associated with function GetGoal
  size_t getGoal_cache_capacity;
  //! Cache associated with function GetGoal
  cache::LRUCache<State, size_t> getGoal_cache;

  //! Logging stream
  mutable Log log;
}; // class GDL

inline void GDL::ApplyState(const State& state)
{
  const std::list<Argument*>& facts = state.GetProps();
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

}; // namespace libgdl

#endif // _LIBGDL_GDL_HPP_INCLUDED
