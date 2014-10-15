namespace libgdl
{

template<typename Reasoner>
GDL<Reasoner>::GDL(const std::string& filename,
                   size_t state_cache_capacity,
                   const Log& l)
  : next_state_cache_capacity(state_cache_capacity),
    next_state_cache(boost::bind(&GDL::cached_GetNextState, this, _1, _2),
                     boost::bind(&GDL::StateMoveHash, this, _1, _2),
                     next_state_cache_capacity),
    isTerminal_cache_capacity(state_cache_capacity),
    isTerminal_cache(boost::bind(&GDL::cached_IsTerminal, this, _1),
                     boost::bind(&GDL::StateHash, this, _1),
                     isTerminal_cache_capacity),
    getLegalMoves_cache_capacity(state_cache_capacity),
    getLegalMoves_cache(boost::bind(&GDL::cached_getLegalMoves, this, _1),
                        boost::bind(&GDL::StateHash, this, _1),
                        getLegalMoves_cache_capacity),
    getGoal_cache_capacity(state_cache_capacity),
    getGoal_cache(boost::bind(&GDL::cached_getGoal, this, _1, _2),
                  boost::bind(&GDL::StateRoleHash, this, _1, _2),
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
    next_state_cache(boost::bind(&GDL::cached_GetNextState, this, _1, _2),
                     boost::bind(&GDL::StateMoveHash, this, _1, _2),
                     next_state_cache_capacity),
    isTerminal_cache_capacity(state_cache_capacity),
    isTerminal_cache(boost::bind(&GDL::cached_IsTerminal, this, _1),
                     boost::bind(&GDL::StateHash, this, _1),
                     isTerminal_cache_capacity),
    getLegalMoves_cache_capacity(state_cache_capacity),
    getLegalMoves_cache(boost::bind(&GDL::cached_getLegalMoves, this, _1),
                        boost::bind(&GDL::StateHash, this, _1),
                        getLegalMoves_cache_capacity),
    getGoal_cache_capacity(state_cache_capacity),
    getGoal_cache(boost::bind(&GDL::cached_getGoal, this, _1, _2),
                  boost::bind(&GDL::StateRoleHash, this, _1, _2),
                  getGoal_cache_capacity),
    log(l)

{}

template<class Reasoner>
GDL<Reasoner>::GDL(gdlreasoner::KIFFlattener& kf,
                   size_t state_cache_capacity,
                   const Log& l)
  : reasoner(kf, l),
    next_state_cache_capacity(state_cache_capacity),
    next_state_cache(boost::bind(&GDL::cached_GetNextState, this, _1, _2),
                     boost::bind(&GDL::StateMoveHash, this, _1, _2),
                     next_state_cache_capacity),
    isTerminal_cache_capacity(state_cache_capacity),
    isTerminal_cache(boost::bind(&GDL::cached_IsTerminal, this, _1),
                     boost::bind(&GDL::StateHash, this, _1),
                     isTerminal_cache_capacity),
    getLegalMoves_cache_capacity(state_cache_capacity),
    getLegalMoves_cache(boost::bind(&GDL::cached_getLegalMoves, this, _1),
                        boost::bind(&GDL::StateHash, this, _1),
                        getLegalMoves_cache_capacity),
    getGoal_cache_capacity(state_cache_capacity),
    getGoal_cache(boost::bind(&GDL::cached_getGoal, this, _1, _2),
                  boost::bind(&GDL::StateRoleHash, this, _1, _2),
                  getGoal_cache_capacity),
    log(l)

{}

template<class Reasoner>
bool GDL<Reasoner>::IsTerminal(const State& state, bool useCache)
{
  bool* out;
  if(useCache) out = isTerminal_cache.Get(state);
  else out = cached_IsTerminal(state);

  return *out;
}

template<class Reasoner>
bool* GDL<Reasoner>::cached_IsTerminal(const State& state)
{
  reasoner.ApplyState(state);

  // check if terminal is satisfiable with current knowledge
  bool* result = reasoner.IsTerminal();

  reasoner.RemoveState();

  return result;
}

template<class Reasoner>
MoveList GDL<Reasoner>::GetLegalMoves(const State& state, bool useCache)
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
MoveList* GDL<Reasoner>::cached_getLegalMoves(const State& state)
{
  reasoner.ApplyState(state);

  MoveList* out = reasoner.GetLegalMoves();

  reasoner.RemoveState();

  return out;
}

template<class Reasoner>
State GDL<Reasoner>::GetNextState(const State& state,
                                  const Move& moves,
                                  bool useCache)
{
  State* out;

  if(useCache)
  {
    out = next_state_cache.Get(state, moves);
  }
  else out = cached_GetNextState(state, moves);

  return *out;
}

template<class Reasoner>
State* GDL<Reasoner>::cached_GetNextState(const State& state,
                                          const Move& moves)
{
  reasoner.ApplyState(state);
  reasoner.ApplyActions(moves);

  State* out = reasoner.GetNextState();

  reasoner.RemoveState();
  reasoner.RemoveActions();

  return out;
}

template<class Reasoner>
size_t GDL<Reasoner>::StateMoveHash(const State& state, const Move& moves) const
{
  size_t seed = state.GetHash();
  boost::hash_combine(seed, moves.Hash());
  return seed;
}

template<class Reasoner>
size_t GDL<Reasoner>::GetGoal(const State& state, const size_t rid, bool useCache)
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
size_t* GDL<Reasoner>::cached_getGoal(const State& state, const size_t rid)
{
  reasoner.ApplyState(state);

  size_t* out = reasoner.GetGoal(rid);

  reasoner.RemoveState();

  return out;
}

template<class Reasoner>
size_t GDL<Reasoner>::StateRoleHash(const State& state, const size_t role) const
{
  size_t seed = role;
  boost::hash_combine(seed, state.GetHash());
  return seed;
}

}
