#include <functional>

namespace libgdl
{

template<typename Reasoner>
GDL<Reasoner>::GDL(const std::string& filename,
                   size_t state_cache_capacity,
                   const Log& l)
  : next_state_cache_capacity(state_cache_capacity),
    next_state_cache(std::bind(&GDL::cached_GetNextState, this, std::placeholders::_1, std::placeholders:: _2),
                     std::bind(&GDL::StateMoveHash, this, std::placeholders::_1, std::placeholders::_2),
                     next_state_cache_capacity),
    isTerminal_cache_capacity(state_cache_capacity),
    isTerminal_cache(std::bind(&GDL::cached_IsTerminal, this, std::placeholders::_1),
                     std::bind(&GDL::StateHash, this, std::placeholders::_1),
                     isTerminal_cache_capacity),
    getLegalMoves_cache_capacity(state_cache_capacity),
    getLegalMoves_cache(std::bind(&GDL::cached_getLegalMoves, this, std::placeholders::_1),
                        std::bind(&GDL::StateHash, this, std::placeholders::_1),
                        getLegalMoves_cache_capacity),
    getGoal_cache_capacity(state_cache_capacity),
    getGoal_cache(std::bind(&GDL::cached_getGoal, this, std::placeholders::_1, std::placeholders::_2),
                  std::bind(&GDL::StateRoleHash, this, std::placeholders::_1, std::placeholders::_2),
                  getGoal_cache_capacity),
    log(l)
{
  gdlparser::KIF kif;
  kif.GetLog() = log;
  kif.AddFile(filename);
  if(!kif.Parse())
  {
    log.Fatal << "Could Not create a gdl object from file " << filename << std::endl;
    exit(1);
  }

  reasoner = Reasoner(kif, l);
}

template<typename Reasoner>
GDL<Reasoner>::GDL(gdlparser::KIF& kif,
                   size_t state_cache_capacity,
                   const Log& l)
  : reasoner(kif, l),
    next_state_cache_capacity(state_cache_capacity),
    next_state_cache(std::bind(&GDL::cached_GetNextState, this, std::placeholders::_1, std::placeholders::_2),
                     std::bind(&GDL::StateMoveHash, this, std::placeholders::_1, std::placeholders::_2),
                     next_state_cache_capacity),
    isTerminal_cache_capacity(state_cache_capacity),
    isTerminal_cache(std::bind(&GDL::cached_IsTerminal, this, std::placeholders::_1),
                     std::bind(&GDL::StateHash, this, std::placeholders::_1),
                     isTerminal_cache_capacity),
    getLegalMoves_cache_capacity(state_cache_capacity),
    getLegalMoves_cache(std::bind(&GDL::cached_getLegalMoves, this, std::placeholders::_1),
                        std::bind(&GDL::StateHash, this, std::placeholders::_1),
                        getLegalMoves_cache_capacity),
    getGoal_cache_capacity(state_cache_capacity),
    getGoal_cache(std::bind(&GDL::cached_getGoal, this, std::placeholders::_1, std::placeholders::_2),
                  std::bind(&GDL::StateRoleHash, this, std::placeholders::_1, std::placeholders::_2),
                  getGoal_cache_capacity),
    log(l)

{}

template<class Reasoner>
GDL<Reasoner>::GDL(gdlreasoner::KIFFlattener& kf,
                   size_t state_cache_capacity,
                   const Log& l)
  : reasoner(kf, l),
    next_state_cache_capacity(state_cache_capacity),
    next_state_cache(std::bind(&GDL::cached_GetNextState, this, std::placeholders::_1, std::placeholders::_2),
                     std::bind(&GDL::StateMoveHash, this, std::placeholders::_1, std::placeholders::_2),
                     next_state_cache_capacity),
    isTerminal_cache_capacity(state_cache_capacity),
    isTerminal_cache(std::bind(&GDL::cached_IsTerminal, this, std::placeholders::_1),
                     std::bind(&GDL::StateHash, this, std::placeholders::_1),
                     isTerminal_cache_capacity),
    getLegalMoves_cache_capacity(state_cache_capacity),
    getLegalMoves_cache(std::bind(&GDL::cached_getLegalMoves, this, std::placeholders::_1),
                        std::bind(&GDL::StateHash, this, std::placeholders::_1),
                        getLegalMoves_cache_capacity),
    getGoal_cache_capacity(state_cache_capacity),
    getGoal_cache(std::bind(&GDL::cached_getGoal, this, std::placeholders::_1, std::placeholders::_2),
                  std::bind(&GDL::StateRoleHash, this, std::placeholders::_1, std::placeholders::_2),
                  getGoal_cache_capacity),
    log(l)

{}

template<class Reasoner>
bool GDL<Reasoner>::IsTerminal(const BState& state, bool useCache)
{
  bool* out;
  if(useCache) out = isTerminal_cache.Get(state);
  else out = cached_IsTerminal(state);

  return *out;
}

template<class Reasoner>
bool* GDL<Reasoner>::cached_IsTerminal(const BState& state)
{
  reasoner.ApplyState(state);

  // check if terminal is satisfiable with current knowledge
  bool* result = reasoner.IsTerminal();

  reasoner.RemoveState();

  return result;
}

template<class Reasoner>
MoveList GDL<Reasoner>::GetLegalMoves(const BState& state, bool useCache)
{
  MoveList* out;
  if(useCache)
  {
    out = getLegalMoves_cache.Get(state);
  }
  else out = cached_getLegalMoves(state);

  return *out;
}

template<class Reasoner>
MoveList* GDL<Reasoner>::cached_getLegalMoves(const BState& state)
{
  reasoner.ApplyState(state);

  MoveList* out = reasoner.GetLegalMoves();

  reasoner.RemoveState();

  return out;
}

template<class Reasoner>
BState GDL<Reasoner>::GetNextState(const BState& state,
                                   const Move& moves,
                                   bool useCache)
{
  BState* out;

  if(useCache)
  {
    out = next_state_cache.Get(state, moves);
  }
  else out = cached_GetNextState(state, moves);

  return *out;
}

template<class Reasoner>
BState* GDL<Reasoner>::cached_GetNextState(const BState& state,
                                           const Move& moves)
{
  reasoner.ApplyState(state);
  reasoner.ApplyActions(moves);

  BState* out = reasoner.GetNextState();

  reasoner.RemoveState();
  reasoner.RemoveActions();

  return out;
}

template<class Reasoner>
size_t GDL<Reasoner>::StateMoveHash(const BState& state, const Move& moves) const
{
  size_t seed = state.GetHash();
  boost::hash_combine(seed, moves.Hash());
  return seed;
}

template<class Reasoner>
size_t GDL<Reasoner>::GetGoal(const BState& state, const size_t rid, bool useCache)
{
  size_t* out;
  if(useCache)
  {
    out = getGoal_cache.Get(state, rid);
  }
  else out = cached_getGoal(state, rid);

  return *out;
}

template<class Reasoner>
size_t* GDL<Reasoner>::cached_getGoal(const BState& state, const size_t rid)
{
  reasoner.ApplyState(state);

  size_t* out = reasoner.GetGoal(rid);

  reasoner.RemoveState();

  return out;
}

template<class Reasoner>
size_t GDL<Reasoner>::StateRoleHash(const BState& state, const size_t role) const
{
  size_t seed = role;
  boost::hash_combine(seed, state.GetHash());
  return seed;
}

}
