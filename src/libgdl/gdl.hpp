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
#include <libgdl/core/data_types/b_state.hpp>
#include <libgdl/core/cache/lru_cache.hpp>
#include <libgdl/gdlparser/kif.hpp>
#include <libgdl/reasoners/gdlreasoner/kif_flattener.hpp>
#include <libgdl/reasoners/gdlreasoner/gdlreasoner.hpp>

namespace libgdl
{

/**
 * GDL class acts as the state machine for general game player. This class is
 * templatized so that to supports many reasoners. Current version of libGDL
 * only implements one reasoner namely GDLReasoner but user can user their own
 * reasoners with this state machine.
 *
 * \tparam Reasoner Reasoner to be used for operations
 *
 * @see GDLReasoner
 */
template<class Reasoner = gdlreasoner::GDLReasoner>
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
      const Log& log = GLOBAL_LOG);

  //! Constructs a GDL object from KIF.
  //!
  //! \param kif KIF object
  //! \param state_cache_capacity cache capacity for all caches
  //! \param log Logging stream
  //!
  //!
  GDL(gdlparser::KIF& kif,
      size_t state_cache_capacity = 1024,
      const Log& log = GLOBAL_LOG);

  //! Constructs a GDL object from flattened knowledge.
  //!
  //! \param kf KIFFlattener object
  //! \param state_cache_capacity cache capacity for all caches
  //! \param log Logging stream
  //!
  //!
  GDL(gdlreasoner::KIFFlattener& kf,
      size_t state_cache_capacity = 1024,
      const Log& log = GLOBAL_LOG);

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
  BState GetNextState(const BState& state,
                      const BMove& moves,
                      bool useCache = true);

  //! Returns if the given state is terminal.
  //! If useCache is true function returns the answer from cache if it exists
  //!
  //! \param state State to check for termination
  //! \param useCache To use cache
  //! \return bool
  //!
  //!
  bool IsTerminal(const BState& state, bool useCache = true);

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
  MoveList<BMove> GetLegalMoves(const BState& state, bool useCache = true);


  //! Returns goal value associated with the given state for given role
  //! If useCache is true function returns the answer from cache if it exists
  //!
  //! \param state State to process
  //! \param role Role of the given player
  //! \param useCache To use cache
  //! \return int
  //!
  //!
  size_t GetGoal(const BState& state, const size_t role, bool useCache = true);

  //! Returns the initial state of the game
  const BState& InitState() const { return reasoner.InitState(); }

  //! Get the symbol table
  SymbolTable GetSymbolTable() const
  {
    return reasoner.GetSymbolTable();
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
  BState* cached_GetNextState(const BState& state,
                              const BMove& moves);

  //! Returns the combined Hash value of given State along with the actions
  //!
  //! \param state State of the game
  //! \param moves Moves to perform
  //! \return size_t
  //!
  //!
  size_t StateMoveHash(const BState& state,
                       const BMove& moves) const;

  //! The function to be called by cache when IsTerminal produces cache miss.
  //!
  //! \param state State of the game
  //! \return bool*
  //!
  //!
  bool* cached_IsTerminal(const BState& state);

  //! The function to be called by cache when GetLegalMoves produces cache miss.
  //!
  //! \param state State of the game
  //! \return MoveList*
  //!
  //!
  MoveList<BMove>* cached_getLegalMoves(const BState& state);

  //! The function to be called by cach when GetGoal produces cache miss.
  //!
  //! \param state State of the game
  //! \param rid Role ID of the given role
  //! \return size_t*
  //!
  //!
  size_t* cached_getGoal(const BState& state, const size_t rid);

  //! Returns the combined hash value of State along with the role ID.
  //!
  //! \param state State of the game
  //! \param role Role ID of the given role
  //! \return size_t
  //!
  //!
  size_t StateRoleHash(const BState& state, const size_t role) const;

  size_t StateHash(const BState& state) const
  {
    return state.GetHash();
  }

  //! Knowledge base representing the rules of the game
  Reasoner reasoner;

  //! Cache capacity of cache associated with function GetNextState
  size_t next_state_cache_capacity;
  //! Cache associated with function GetNextState
  cache::LRUCache<BState, BState, BMove> next_state_cache;

  //! Cache capacity of cache associated with function IsTerminal
  size_t isTerminal_cache_capacity;
  //! Cache associated with function IsTerminal
  cache::LRUCache<bool, BState> isTerminal_cache;

  //! Cache capacity of cache associated with functions GetLegalMoves
  size_t getLegalMoves_cache_capacity;
  //! Cache associated with function GetLegalMoves
  cache::LRUCache<MoveList<BMove>, BState> getLegalMoves_cache;

  //! Cache capacity of cache associated with function GetGoal
  size_t getGoal_cache_capacity;
  //! Cache associated with function GetGoal
  cache::LRUCache<size_t, BState, size_t> getGoal_cache;

  //! Logging stream
  mutable Log log;
}; // class GDL

}; // namespace libgdl

#include "gdl_impl.hpp"

#endif // _LIBGDL_GDL_HPP_INCLUDED
