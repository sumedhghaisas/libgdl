#include "gdl.hpp"

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/functional/hash.hpp>
#include <sstream>

using namespace std;
using namespace boost;
using namespace libgdl;
using namespace libgdl::core;
using namespace libgdl::gdlparser;
using namespace libgdl::gdlreasoner;

GDL::GDL(const string& filename,
         size_t state_cache_capacity,
         const Log& l)
  : next_state_cache_capacity(state_cache_capacity),
    next_state_cache(next_state_cache_capacity),
    isTerminal_cache_capacity(state_cache_capacity),
    isTerminal_cache(bind(&GDL::cached_IsTerminal, this, _1),
                     isTerminal_cache_capacity),
    getLegalMoves_cache_capacity(state_cache_capacity),
    getLegalMoves_cache(bind(&GDL::cached_getLegalMoves, this, _1),
                        getLegalMoves_cache_capacity),
    log(l)
{
  KIF kif;
  kif.GetLog() = log;
  kif.AddFile(filename);
  if(!kif.Parse())
  {
    log.Fatal << "Could Not create a gdl object from file " << filename << endl;
    exit(1);
  }

  base_rules = KnowledgeBase(kif);

  kif.GetSymbolTable() = NULL;
  kif.Clear();

  list<Argument*> result = base_rules.Ask("(role ?x)");
  for(list<Argument*>::iterator it = result.begin();it != result.end();it++)
  {
    roles.push_back((*it)->args[0]);
    (*it)->args.clear();
    delete *it;
  }
  result.clear();

  result = base_rules.Ask("(init ?x)");
  list<Argument*> temp;
  for(list<Argument*>::iterator it = result.begin();it != result.end();it++)
  {
    temp.push_back((*it)->args[0]);
    (*it)->args.clear();
    delete *it;
  }
  init = new State(new core::RawState(temp));
}

GDL::GDL(KIF& kif,
         size_t state_cache_capacity,
         const Log& l)
  : base_rules(kif),
    next_state_cache_capacity(state_cache_capacity),
    next_state_cache(next_state_cache_capacity),
    isTerminal_cache_capacity(state_cache_capacity),
    isTerminal_cache(bind(&GDL::cached_IsTerminal, this, _1),
                      isTerminal_cache_capacity),
    getLegalMoves_cache_capacity(state_cache_capacity),
    getLegalMoves_cache(bind(&GDL::cached_getLegalMoves, this, _1),
                        getLegalMoves_cache_capacity),
    log(l)

{
  kif.Clear();

  list<Argument*> result = base_rules.Ask("(role ?x)");
  for(list<Argument*>::iterator it = result.begin();it != result.end();it++)
  {
    roles.push_back((*it)->args[0]);
    (*it)->args.clear();
    delete *it;
  }
  result.clear();

  result = base_rules.Ask("(init ?x)");
  list<Argument*> temp;
  for(list<Argument*>::iterator it = result.begin();it != result.end();it++)
  {
    temp.push_back((*it)->args[0]);
    (*it)->args.clear();
    delete *it;
  }
  init = new State(new core::RawState(temp));
}

bool GDL::IsTerminal(const State& state, bool useCache)
{
  bool* out;
  if(useCache) out = isTerminal_cache.Get(state);
  else out = cached_IsTerminal(state);

  return *out;
}

bool* GDL::cached_IsTerminal(const State& state)
{
  ApplyState(state);

  Argument* question = new Argument;
  question->t = Argument::Relation;
  question->value = SymbolTable::TerminalID;

  // check if terminal is satisfiable with current knowledge
  bool* result = new bool(base_rules.IsSatisfiable(*question));

  delete question;

  RemoveState();

  return result;
}

MoveList GDL::GetLegalMoves(const State& state, bool useCache)
{
  MoveList* out;
  if(useCache)
  {
    //size_t start = microtimer();
    out = getLegalMoves_cache.Get(state);
    //cout << microtimer() - start << endl;
  }
  else out = cached_getLegalMoves(state);

  return *out;
}

MoveList* GDL::cached_getLegalMoves(const State& state)
{
  ApplyState(state);

  // get legal moves for all the roles
  list<Argument*> result[roles.size()];

  Argument* question = new Argument;
  question->t = Argument::Relation;
  question->value = SymbolTable::LegalID;

  Argument* var_x = new Argument;
  var_x->t = Argument::Var;
  var_x->val = "?x";

  question->args.push_back(NULL);
  question->args.push_back(var_x);

  size_t i = 0;
  for(list<Argument*>::const_iterator it = roles.begin();it != roles.end();it++)
  {
    question->args[0] = *it;
    result[i] = base_rules.Ask(*question);
    i++;
  }

  delete var_x;
  question->args.clear();
  delete question;

  RemoveState();

  list<Argument*>::iterator it[roles.size()];
  for(size_t i = 0;i < roles.size();i++)
    it[i] = result[i].begin();

  MoveList* out = new MoveList(new core::IntrusiveList<Move>());

  while(true)
  {
    vector<Argument*> moves;
    for(size_t i = 0;i < roles.size();i++)
    {
      moves.push_back((*it[i])->args[1]);
    }
    (*out)->push_back(Move(moves));

    it[0]++;
    size_t index = 1;
    if(it[0] == result[0].end())
    {
      it[0] = result[0].begin();

      while(true)
      {
        if(index == roles.size() ||
                (it[index] == (--result[index].end()) && index == roles.size() - 1))
        {
          for(size_t i = 0;i < roles.size();i++)
          {
            for(list<Argument*>::iterator it = result[i].begin();
                                                    it != result[i].end();it++)
            {
              delete *it;
            }
          }
          return out;
        }
        else if(it[index] == (--result[index].end()))
        {
          it[index] = result[index].begin();
          index++;
        }
        else
        {
          it[index]++;
          break;
        }
      }
    }
  }
  return out;
}


State GDL::GetNextState(const State& state,
                        const Move& moves,
                        bool useCache)
{
  State* out;
  //size_t start = microtimer();
  if(useCache)
  {
    //start = microtimer();
    boost::function<size_t (const State&)>
                      hash_f(bind(&GDL::StateMoveHash, this, _1, moves));
    boost::function<State* (const State&)>
                      miss_f(bind(&GDL::cached_GetNextState, this, _1, moves));
    //cout << microtimer() - start << endl;

    out = next_state_cache.Get(state, miss_f, hash_f);
  }
  else out = cached_GetNextState(state, moves);
  //cout << microtimer() - start << " haha" << endl;

  return *out;
}

State* GDL::cached_GetNextState(const State& state,
                                const Move& moves)
{
  ApplyState(state);
  ApplyActions(moves);

  Argument* question = new Argument;
  question->t = Argument::Relation;
  question->value = SymbolTable::NextID;

  Argument* var_x = new Argument;
  var_x->t = Argument::Var;
  var_x->val = "?x";

  question->args.push_back(var_x);

  // get base propositions true in the next state
  list<Argument*> result = base_rules.Ask("(next ?x)");

  delete question;

  RemoveState();
  RemoveActions();

  list<Argument*> temp;
  for(list<Argument*>::const_iterator it = result.begin();it != result.end();it++)
  {
    temp.push_back((*it)->args[0]);
    (*it)->args.clear();
    delete *it;
  }
  return new State(new core::RawState(temp));
}

size_t GDL::StateMoveHash(const State& state, const Move& moves) const
{
  size_t seed = state.GetHash();
  hash_combine(seed, moves.Hash());
  return seed;
}

size_t GDL::GetGoal(const State& state, const size_t rid, bool useCache)
{
  size_t* out;
  if(useCache)
  {
    boost::function<size_t (const State&)>
                      hash_f(bind(&GDL::StateRoleHash, this, _1, rid));
    boost::function<size_t* (const State&)>
                      miss_f(bind(&GDL::cached_getGoal, this, _1, rid));

    out = getGoal_cache.Get(state, miss_f, hash_f);
  }
  else out = cached_getGoal(state, rid);

  return *out;
}


size_t* GDL::cached_getGoal(const State& state, const size_t rid)
{
  ApplyState(state);

  list<Argument*>::iterator it = roles.begin();
  for(size_t i = 0;i < rid;i++) it++;

  Argument* question = new Argument;
  question->t = Argument::Relation;
  question->value = SymbolTable::GoalID;

  Argument* var_x = new Argument;
  var_x->t = Argument::Var;
  var_x->val = "?x";

  question->args.push_back(*it);
  question->args.push_back(var_x);

  // get goal value for the given role
  list<Argument*> result = base_rules.Ask(*question);

  delete var_x;
  question->args.clear();
  delete question;

  RemoveState();

  stringstream stream;
  stream <<
  base_rules.GetSymbolTable()->GetCommandName((*result.begin())->args[1]->value);
  size_t *out = new size_t;
  stream >> *out;

  delete *result.begin();

  return out;
}

size_t GDL::StateRoleHash(const State& state, const size_t role) const
{
  size_t seed = role;
  hash_combine(seed, state.GetHash());
  return seed;
}
