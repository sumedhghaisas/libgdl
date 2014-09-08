#include "gdl.hpp"

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/functional/hash.hpp>
#include <sstream>

using namespace std;
using namespace boost;
using namespace libgdl;
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

  id_map = kif.IDMap();
  base_rules = KnowledgeBase(kif);
  kif.Clear();

  list<Argument*> result;// = base_rules.Ask(Argument("(role ?x)"));
  for(list<Argument*>::iterator it = result.begin();it != result.end();it++)
  {
    roles.push_back((*it)->args[0]);
    (*it)->args.clear();
    delete *it;
  }
  result.clear();

  result;// = base_rules.Ask(Argument("(init ?x)"));
  list<Argument*> temp;
  for(list<Argument*>::iterator it = result.begin();it != result.end();it++)
  {
    temp.push_back((*it)->args[0]);
    (*it)->args.clear();
    delete *it;
  }
  init = new State(new RawState(temp, *id_map));
}

GDL::GDL(KIF& kif,
         size_t state_cache_capacity,
         const Log& l)
  : id_map(kif.IDMap()),
    base_rules(kif),
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

  list<Argument*> result;// = base_rules.Ask(Argument("(role ?x)"));
  for(list<Argument*>::iterator it = result.begin();it != result.end();it++)
  {
    roles.push_back((*it)->args[0]);
    (*it)->args.clear();
    delete *it;
  }
  result.clear();

  result;// = base_rules.Ask(Argument("(init ?x)"));
  list<Argument*> temp;
  for(list<Argument*>::iterator it = result.begin();it != result.end();it++)
  {
    temp.push_back((*it)->args[0]);
    (*it)->args.clear();
    delete *it;
  }
  init = new State(new RawState(temp, *id_map));
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

  // check if terminal is satisfiable with current knowledge
  bool* result;// = new bool(base_rules.IsSatisfiable(Argument("terminal")));

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

  size_t i = 0;
  for(list<Argument*>::const_iterator it = roles.begin();it != roles.end();it++)
  {
    result[i];// = base_rules.Ask(Argument("(legal " + (*it)->val + " ?x)"));
    i++;
  }

  RemoveState();

  list<Argument*>::iterator it[roles.size()];
  for(size_t i = 0;i < roles.size();i++)
    it[i] = result[i].begin();

  MoveList* out = new MoveList(new IntrusiveList<Move>());

  while(true)
  {
    vector<Argument*> moves;
    for(size_t i = 0;i < roles.size();i++)
    {
      moves.push_back((*it[i])->args[1]);
    }
    (*out)->push_back(Move(moves, *id_map));

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

  // get base propositions true in the next state
  list<Argument*> result;// = base_rules.Ask(Argument("(next ?x)"));

  RemoveState();
  RemoveActions();

  list<Argument*> temp;
  for(list<Argument*>::const_iterator it = result.begin();it != result.end();it++)
  {
    temp.push_back((*it)->args[0]);
    (*it)->args.clear();
    delete *it;
  }
  return new State(new RawState(temp, *id_map));
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

  // get goal value for the given role
  list<Argument*> result;// =
              //base_rules.Ask(Argument("(goal " + (*it)->val + " ?x)"));

  RemoveState();

  stringstream stream;
  stream << *(*result.begin())->args[1];
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
//
//
//int GDL::GetGoal(const State& state, const std::string& role, bool useCache) const
//{
//    size_t rid = 0;
//    for(size_t i = 0; i < roles.size(); i++)
//        if(roles[i] == role)
//        {
//            rid = i;
//            break;
//        }
//
//    return GetGoal(state,rid,useCache);
//}
//
//GDL::StringVec GDL::GetRandomJointMove(const State& state, RandomNumberGenerator& ran, bool useCache) const
//{
//    // flush the knowledge base cache
//    base_rules.flushCache();
//
//    // apply the given state to the knowledge base
//    const StringVec& facts = state.getFacts();
//    for(size_t i = 0; i < facts.size(); i++)
//    {
//        logicbase::Fact f ("(" + Compress("true") + " " + facts[i] + ")");
//        // add to map
//        base_rules.m_facts[s_sig].push_back(f);
//    }
//
//    std::vector<StringVec>* t_vec = NULL;
//
//    if(useCache) t_vec = combination_cache->Get(state);
//    else t_vec = cached_GetLegalJointMoves(state);
//
//    // remove the knowledge of state from the knowledge base
//    base_rules.m_facts.erase(base_rules.m_facts.find(s_sig));
//
//    // return random entry in it
//    size_t index = ran.GetRandomNumber(0,t_vec->size() - 1);
//    return (*t_vec)[index];
//}
//
//State GDL::GetRandomNextState(const State& state, RandomNumberGenerator& ran, bool useCache) const
//{
//    // flush the knowledge base cache
//    base_rules.flushCache();
//
//    // apply the given state to the knowledge base
//    const StringVec& facts = state.getFacts();
//    for(size_t i = 0; i < facts.size(); i++)
//    {
//        logicbase::Fact f ("(" + Compress("true") + " " + facts[i] + ")");
//        // add to map
//        base_rules.m_facts[s_sig].push_back(f);
//    }
//
//    std::vector<StringVec> *all;
//    if(useCache) all = combination_cache->Get(state);
//    else all = cached_GetLegalJointMoves(state);
//
//    // random entry in it
//    StringVec& moves = (*all)[ran.GetRandomNumber(0,all->size() - 1)];
//
//    // apply move given to the knowledge base
//    for(size_t i = 0; i < moves.size(); i++)
//    {
//        logicbase::Fact f ("(" + Compress("does") + " " + roles[i] + " " + moves[i] + ")");
//        // add to map
//        base_rules.m_facts[m_sig].push_back(f);
//    }
//
//    State *out;
//    if(useCache)
//    {
//        boost::function<size_t (const State&)> hash_f(boost::bind(&GDL::StateMoveHash, this, _1, moves));
//        boost::function<State* (const State&)> miss_f(boost::bind(&GDL::cached_GetNextState, this, _1, moves));
//
//        out = next_state_cache->Get(state, miss_f, hash_f);
//    }
//    else out = cached_GetNextState(state,moves);
//
//    // remove the knowledge of state from the knowledge base
//    base_rules.m_facts.erase(base_rules.m_facts.find(s_sig));
//    // remove the knowledge of inputs from the knowledge base
//    base_rules.m_facts.erase(base_rules.m_facts.find(m_sig));
//
//    return *out;
//}
//
//State GDL::PerformRandomDepthCharge(const State& s, RandomNumberGenerator& ran, bool useCache) const
//{
//
////    State t_state = s;
////    int count = 0;
////    while(!IsTerminal(t_state,useCache))
////    {
////        t_state = GetRandomNextState(t_state, ran, useCache);
////        count++;
////    }
////    Log::Warn << count << std::endl;
////    return t_state;
//
//    State t_state = s;
//    do
//    {
//        // flush the knowledge base cache
//        base_rules.flushCache();
//
//        // apply the given state to the knowledge base
//        const StringVec& facts = t_state.getFacts();
//        for(size_t i = 0; i < facts.size(); i++)
//        {
//            logicbase::Fact f ("(" + Compress("true") + " " + facts[i] + ")");
//            // add to map
//            base_rules.m_facts[s_sig].push_back(f);
//        }
//
//        // check for terminal state
//        bool terminal = false;
//        if(useCache) terminal = *(sim_isTerminal_cache->Get(t_state));
//        else terminal = *(cached_IsTerminal(t_state));
//
//        // if state is terminal break
//        if(terminal)
//        {
//            // remove the knowledge of state from the knowledge base
//            base_rules.m_facts.erase(base_rules.m_facts.find(s_sig));
//            break;
//        }
//
//        // get legal joint moves
//        std::vector<StringVec>* all;
//        if(useCache) all = sim_combination_cache->Get(t_state);
//        else all = cached_GetLegalJointMoves(t_state);
//
//        // get random move out of legal joint moves
//        size_t r = ran.GetRandomNumber(0, all->size() - 1);
//        StringVec& moves = (*all)[r];
//
//        // apply move given to the knowledge base
//        for(size_t i = 0; i < moves.size(); i++)
//        {
//            logicbase::Fact f ("(" + Compress("does") + " " + roles[i] + " " + moves[i] + ")");
//            // add to map
//            base_rules.m_facts[m_sig].push_back(f);
//        }
//
//        boost::function<State* (const State&)> miss_f(boost::bind(&GDL::cached_GetNextState, this, _1, moves));
//        boost::function<size_t (const State&)> hash_f(boost::bind(&GDL::StateMoveHash, this, _1, moves));
//
//        if(useCache) t_state = *(sim_next_state_cache->Get(t_state, miss_f, hash_f));
//        else t_state = *(cached_GetNextState(t_state, moves));
//
//        // remove the knowledge of state from the knowledge base
//        base_rules.m_facts.erase(base_rules.m_facts.find(s_sig));
//        // remove the knowledge of inputs from the knowledge base
//        base_rules.m_facts.erase(base_rules.m_facts.find(m_sig));
//    }
//    while(true);
//    return t_state;
//}

//
//KnowledgeBase GDL::GetDataKnowledge(std::map<logicbase::RelationSignature, std::vector<logicbase::RelationSignature> >& domain_map) const
//{
//    // knowledge base of data
//    KnowledgeBase out;
//
//    const KnowledgeBase::FactMap& m_facts = base_rules.getAllFacts();
//    const KnowledgeBase::ClauseMap& m_clauses = base_rules.getAllClauses();
//
//    std::set<logicbase::RelationSignature> data_propositions;
//
//    // loop through all facts and identify data propositions
//    // add this knowledge to the knowledge base
//    // ignore 'legal', 'init', 'base', 'input' etc.
//    for(KnowledgeBase::FactMap::const_iterator it = m_facts.begin(); it != m_facts.end(); it++)
//    {
//        const logicbase::RelationSignature& sig = it->first;
//        if(sig.name != Compress("legal") && sig.name != Compress("input") &&
//                sig.name != Compress("base") && sig.name != Compress("next") &&
//                sig.name != Compress("goal") && sig.name != Compress("init") &&
//                sig.name != Compress("role") && m_clauses.find(sig) == m_clauses.end())
//        {
//            data_propositions.insert(sig);
//            const KnowledgeBase::FactVec& f_vec = it->second;
//            for(KnowledgeBase::FactVec::const_iterator it2 = f_vec.begin(); it2 != f_vec.end(); it2++)
//            {
//                const KnowledgeBase::Fact& fact = *it2;
//                out.add(fact.text);
//            }
//        }
//    }
//
//    // stores the information gathered in the current pass
//    std::vector<logicbase::RelationSignature> newly_added;
//
//    // iterate through clauses and identify data relations
//    // in this pass relation dependancies are also computed
//    for(KnowledgeBase::ClauseMap::const_iterator it = m_clauses.begin(); it != m_clauses.end(); it++)
//    {
//        const KnowledgeBase::ClauseVec& clauses = it->second;
//        const logicbase::RelationSignature& c_sig = it->first;
//
//        // ignore 'legal', 'next', 'init' etc. clauses
//        if(c_sig.name == Compress("legal") || c_sig.name == Compress("next") ||
//                c_sig.name == Compress("base") || c_sig.name == Compress("input") ||
//                c_sig.name == Compress("goal") || c_sig.name == Compress("terminal") ||
//                c_sig.name == Compress("init") || c_sig.name == Compress("role")) continue;
//
//        // is this relation data relation
//        bool isData= true;
//
//        // stores all clauses of this signature in string format
//        StringVec sig_clauses;
//
//        for(size_t i = 0; i < clauses.size(); i++)
//        {
//            const std::string& clause = clauses[i].toString();
//            sig_clauses.push_back(clause);
//
//            std::string cmd;
//            StringVec args;
//            KIFSyntax::separateCommand(clause, cmd, args);
//
//            for(size_t j = 1; j < args.size(); j++)
//            {
//                std::set<logicbase::RelationSignature> domains;
//
//                // check whether the current premis is data premis
//                if(!IsDataPremiss(args[j], data_propositions, domains))
//                {
//                    isData = false;
//                }
//                // update domain information (dependency map information)
//                // this helps in deciding which clauses to select in the next pass
//                for(std::set<logicbase::RelationSignature>::const_iterator d_it = domains.begin(); d_it != domains.end(); d_it++)
//                {
//                    domain_map[*d_it].push_back(c_sig);
//                }
//            }
//        }
//
//        if(isData)
//        {
//            newly_added.push_back(c_sig);
//            data_propositions.insert(c_sig);
//            out.add(sig_clauses);
//        }
//    }
//
//    // make passes until there is no more data relation
//    while(!newly_added.empty())
//    {
//        // find relations which need to be considered for next pass
//        std::set<logicbase::RelationSignature> toCheck;
//        for(size_t i = 0; i < newly_added.size(); i++)
//        {
//            // add all relations dependent on data relations detected in this pass
//            const std::vector<logicbase::RelationSignature>& sigs = domain_map[newly_added[i]];
//            for(size_t j = 0; j < sigs.size(); j++) toCheck.insert(sigs[j]);
//        }
//
//        newly_added.clear();
//
//        // next pass
//        for(std::set<logicbase::RelationSignature>::const_iterator it = toCheck.begin(); it != toCheck.end(); it++)
//        {
//            const logicbase::RelationSignature& c_sig = *it;
//            const KnowledgeBase::ClauseVec& clauses = m_clauses.find(c_sig)->second;
//
//            // ignore 'legal', 'next', 'init' etc. clauses
//            if(c_sig.name == Compress("legal") || c_sig.name == Compress("next") ||
//                    c_sig.name == Compress("base") || c_sig.name == Compress("input") ||
//                    c_sig.name == Compress("goal") || c_sig.name == Compress("terminal") ||
//                    c_sig.name == Compress("init") || c_sig.name == Compress("role")) continue;
//
//            // if it is already detected as data proposition ignore
//            if(data_propositions.find(c_sig) != data_propositions.end()) continue;
//
//            bool isData= true;
//
//            StringVec sig_clauses;
//
//            for(size_t i = 0; i < clauses.size(); i++)
//            {
//                const std::string& clause = clauses[i].toString();
//                sig_clauses.push_back(clause);
//
//                std::string cmd;
//                StringVec args;
//                KIFSyntax::separateCommand(clause, cmd, args);
//
//                for(size_t j = 1; j < args.size(); j++)
//                {
//                    std::set<logicbase::RelationSignature> domains;
//
//                    // check whether the current premis is data premis
//                    if(!IsDataPremiss(args[j], data_propositions, domains))
//                    {
//                        isData = false;
//                    }
//                }
//            }
//
//            if(isData)
//            {
//                newly_added.push_back(c_sig);
//                data_propositions.insert(c_sig);
//                out.add(sig_clauses);
//            }
//        }
//    }
//
//    return out;
//}
//
//bool GDL::IsDataPremiss(const std::string& premiss,
//                        const std::set<logicbase::RelationSignature>& data_propositions,
//                        std::set<logicbase::RelationSignature>& domains) const
//{
//    std::string cmd;
//    StringVec args;
//    KIFSyntax::separateCommand(premiss, cmd, args);
//
//    if(cmd == "not") return IsDataPremiss(args[0], data_propositions, domains);
//    else if(cmd == "distinct") return true;
//    else if(cmd == "or")
//    {
//        bool out = true;
//        for(size_t i = 1; i < args.size(); i++) out = IsDataPremiss(args[i], data_propositions, domains);
//        return out;
//    }
//    else
//    {
//        logicbase::RelationSignature sig(premiss);
//        domains.insert(sig);
//        if(data_propositions.find(sig) != data_propositions.end()) return true;
//        return false;
//    }
//}
//
//bool GDL::IsDataPremiss(const std::string& premiss,
//                        const KnowledgeBase& data_kb) const
//{
//    std::string cmd;
//    StringVec args;
//    KIFSyntax::separateCommand(premiss, cmd, args);
//
//    if(cmd == "not") return IsDataPremiss(args[0], data_kb);
//    else if(cmd == "distinct") return true;
//    else if(cmd == "or")
//    {
//        bool out = true;
//        for(size_t i = 1; i < args.size(); i++) out = IsDataPremiss(args[i], data_kb);
//        return out;
//    }
//    else
//    {
//        logicbase::RelationSignature sig(premiss);
//        if(data_kb.isSatisfiable(premiss)) return true;
//        else return false;
//        return false;
//    }
//}
//
//std::string GDL::ProcessClause(std::string& in,
//                               std::string& head,
//                               StringSet& domain,
//                               const KnowledgeBase& kb,
//                               size_t index) const
//{
//    std::string out;
//
//    // Tokenize clause
//    std::string cmd;
//    StringVec args;
//    KIFSyntax::separateCommand(in,cmd,args);
//
//    std::string flattenedConstruct;
//    flattenedConstruct = "(<= " + args[0];
//
//    StringSet tset;
//
//    // vars of the entire clause
//    StringSet vars = KIFSyntax::getVariables(in);
//
//    std::string head_cmd;
//    StringVec head_args;
//    KIFSyntax::separateCommand(args[0], head_cmd, head_args);
//
//    std::ostringstream tstream;
//    tstream << index;
//    head = "(" + head_cmd + "_" + tstream.str();
//    for(size_t i = 0; i < head_args.size(); i++) head += " " + head_args[i];
//    head += ")";
//
//    head = ProcessPremiss(head,tset,kb);
//    tset.clear();
//
//    // adjust head to include vars of entire clause
//    AdjustVariables(head,vars);
//
//    out = "(<= " + head;
//
//    bool hasAnyNonDataPremiss = false;
//    for(size_t i = 1; i < args.size(); i++)
//    {
//        out += " " + ProcessPremiss(args[i], domain, kb);
//        if(!IsDataPremiss(args[i], kb))
//        {
//            flattenedConstruct += " " + args[i];
//            hasAnyNonDataPremiss = true;
//        }
//        tset.clear();
//    }
//    out += ")";
//    if(!hasAnyNonDataPremiss) flattenedConstruct = flattenedConstruct.substr(4);
//    else flattenedConstruct += ")";
//    in = flattenedConstruct;
//    return out;
//}
//
//std::string GDL::ProcessPremiss(const std::string& premiss,
//                                StringSet& domain,
//                                const KnowledgeBase& kb) const
//{
//    std::string out;
//    std::string in;
//    if(KIFSyntax::isNotBraced(premiss)) in = "(" + premiss + " 1)";
//    else in = premiss;
//
//    std::string cmd;
//    StringVec args;
//    // separate input string
//    KIFSyntax::separateCommand(in,cmd,args);
//
//    // process special syntax
//    if(cmd == Compress("base") || cmd == Compress("init") || cmd == Compress("next") ||
//            cmd == Compress("input") || cmd == Compress("goal") || cmd == Compress("goal") ||
//            cmd == Compress("legal")) out = in;
//    else if(cmd == Compress("true"))
//    {
//        out = "(" + Compress("base") + " " + args[0] + ")";
//    }
//    // if command is 'not'
//    // if the argument is data proposition then keep not as it is and process the argument
//    // else return only the processed argument
//    else if(cmd == Compress("not"))
//    {
//        out = ProcessPremiss(args[0],domain,kb);
//
//        // if argument is satisfied my kb then it must be a data proposition
//        if(kb.isSatisfiable(out))
//        {
//            out = "(" + Compress("not") + " " + out + ")";
//        }
//    }
//    // process arguments of 'or' proposition
//    else if(cmd == Compress("or"))
//    {
//        out = "(" + Compress("or") + " ";
//        for(size_t i = 0; i < args.size(); i++)
//        {
//            out += ProcessPremiss(args[i], domain, kb) + " ";
//        }
//        out += ")";
//    }
//    // change the proposition to input proposition
//    else if(cmd == Compress("does"))
//    {
//        domain.insert("input");
//        out = "(" + Compress("input") + " " + args[0] + " " + args[1] + ")";
//    }
//    else
//    {
//        out = in;
//        domain.insert(cmd);
//    }
//    return out;
//}
//
//void GDL::AdjustVariables(std::string& in,const StringSet& vars) const
//{
//    StringSet hvars = KIFSyntax::getVariables(in);
//    hvars = epicurus::util::setop::setDifference(vars,hvars);
//    for(std::set<std::string>::iterator it = hvars.begin(); it != hvars.end(); it++)
//    {
//        in.replace(in.size() - 1,0," " + *it);
//    }
//}
//
//};
