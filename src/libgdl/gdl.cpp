#include "gdl.hpp"

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/functional/hash.hpp>

using namespace std;
using namespace boost;
using namespace libgdl;
using namespace libgdl::gdlparser;
using namespace libgdl::gdlreasoner;

GDL::GDL(const string& filename, size_t state_cache_capacity)
  : next_state_cache_capacity(state_cache_capacity),
    next_state_cache(next_state_cache_capacity),
    isTerminal_cache_capacity(state_cache_capacity),
    isTerminal_cache(bind(&GDL::cached_IsTerminal, this, _1),
                      isTerminal_cache_capacity)
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

  list<Argument*> result = base_rules.Ask(Argument("(role ?x)"));
  for(list<Argument*>::iterator it = result.begin();it != result.end();it++)
  {
    roles.push_back((*it)->args[0]);
    (*it)->args.clear();
    delete *it;
  }
  result.clear();

  result = base_rules.Ask(Argument("(init ?x)"));
  list<Argument*> temp;
  for(list<Argument*>::iterator it = result.begin();it != result.end();it++)
  {
    temp.push_back((*it)->args[0]);
    (*it)->args.clear();
    delete *it;
  }
  init = new State(temp, *id_map);
}

GDL::GDL(KIF& kif, size_t state_cache_capacity)
  : id_map(kif.IDMap()),
    base_rules(kif),
    next_state_cache_capacity(state_cache_capacity),
    next_state_cache(next_state_cache_capacity),
    isTerminal_cache_capacity(state_cache_capacity),
    isTerminal_cache(bind(&GDL::cached_IsTerminal, this, _1),
                      isTerminal_cache_capacity)

{
  kif.Clear();

  list<Argument*> result = base_rules.Ask(Argument("(role ?x)"));
  for(list<Argument*>::iterator it = result.begin();it != result.end();it++)
  {
    roles.push_back((*it)->args[0]);
    (*it)->args.clear();
    delete *it;
  }
  result.clear();

  result = base_rules.Ask(Argument("(init ?x)"));
  list<Argument*> temp;
  for(list<Argument*>::iterator it = result.begin();it != result.end();it++)
  {
    temp.push_back((*it)->args[0]);
    (*it)->args.clear();
    delete *it;
  }
  init = new State(temp, *id_map);

//  function<bool* (const State&)>
//        f_terminal(bind(&GDL::cached_IsTerminal, this, _1));
//  function<vector<StringVec>* (const State& state)>
//                  f_combination(bind(&GDL::cached_GetLegalJointMoves,this,_1));
//
//
//  // legal move combination cache which stores legal move combinations of certain states
//  combination_cache = new LRUCache<State, std::vector<StringVec> >(f_combination, state_cache_capacity);
//
//    // goal value cache which stores the goal values associated with certain states for different roles
//    goal_cache = new LRUCache<State, int*>(state_cache_capacity);
//
//
//    // simulation caches
//
//    // simulation terminal cache
//    sim_isTerminal_cache = new LRUCache<State, bool>(f_terminal, sim_cache_capacity);
//
//    // simulation cache for storing legal joint moves
//    sim_combination_cache = new LRUCache<State, std::vector<StringVec> >(f_combination, sim_cache_capacity);
//
//    // simulation next state cache
//    sim_next_state_cache = new LRUCache<State, State>(sim_cache_capacity);

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
    bool* result = new bool(base_rules.IsSatisfiable(Argument("terminal")));

    RemoveState();

    return result;
}

//std::vector<GDL::StringVec> GDL::GetLegalJointMoves(const State& state, bool useCache) const
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
//    std::vector<GDL::StringVec>* out;
//    if(useCache) out = combination_cache->Get(state);
//    else out = cached_GetLegalJointMoves(state);
//
//    // remove the knowledge of state from the knowledge base
//    base_rules.m_facts.erase(base_rules.m_facts.find(s_sig));
//
//    return *out;
//}
//
State GDL::GetNextState(const State& state,
                        const Move& moves,
                        bool useCache)
{
  State* out;
  if(useCache)
  {
    boost::function<size_t (const State&)> hash_f(bind(&GDL::StateMoveHash, this, _1, moves));
    boost::function<State* (const State&)> miss_f(bind(&GDL::cached_GetNextState, this, _1, moves));

    out = next_state_cache.Get(state, miss_f, hash_f);
  }
  else out = cached_GetNextState(state, moves);

  return *out;
}

State* GDL::cached_GetNextState(const State& state,
                                const Move& moves)
{
  ApplyState(state);
  ApplyActions(moves);

  // get base propositions true in the next state
  list<Argument*> result = base_rules.Ask(Argument("(next ?x)"));

  RemoveState();
  RemoveActions();

  list<Argument*> temp;
  for(list<Argument*>::const_iterator it = result.begin();it != result.end();it++)
    temp.push_back((*it)->args[0]);
  return new State(temp, *id_map);
}

size_t GDL::StateMoveHash(const State& state,
                          const Move& moves)
{
  size_t seed = state.GetHash();
  hash_combine(seed, moves.Hash());
  return seed;
}

//
//GDL::StringVec GDL::GetLegalMoves(const State& state, const std::string& role) const
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
//    // get legal moves for the given role
//    StringVec result = base_rules.ask("(" + Compress("legal") + " " + role + " ?x)", "?x",true);
//
//    // remove the knowledge of state from the knowledge base
//    base_rules.m_facts.erase(base_rules.m_facts.find(s_sig));
//
//    return result;
//}
//
//int GDL::GetGoal(const State& state, size_t rid, bool useCache) const
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
//    int out;
//    if(useCache)
//    {
//        // get the array of goal values associated with the state
//        boost::function<int** (const State&)> f_temp(boost::bind(&GDL::cached_GetGoal, this, _1, rid));
//        int **result = goal_cache->Get(state, f_temp);
//
//        int& slot = (*result)[rid];
//
//        // if goal value for given role exists in that array return that value
//        if(slot != -1)
//        {
//            out = slot;
//        }
//        // if it does not exist
//        else
//        {
//            // compute the goal value for given role and update it in the array
//            int** temp = cached_GetGoal(state,rid);
//            slot = (*temp)[rid];
//            out = slot;
//        }
//    }
//    else
//    {
//        int** temp = cached_GetGoal(state,rid);
//        out = (*temp)[rid];
//    }
//
//    // remove the knowledge of state from the knowledge base
//    base_rules.m_facts.erase(base_rules.m_facts.find(s_sig));
//
//    return out;
//}
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
//KIF GDL::GetFlattenedKIF() const
//{
//    StringVec flattenedClauses;
//
//    std::map<logicbase::RelationSignature, std::vector<logicbase::RelationSignature> > domain_map;
//    KnowledgeBase data_kb = GetDataKnowledge(domain_map);
//    Log::Info << data_kb << std::endl;
//
//    const StringVec& m_facts = kif.Facts();
//    StringVec m_clauses = kif.Clauses();
//
//    for(size_t i = 0;i < m_facts.size();i++)
//    {
//        if(!IsDataPremiss(m_facts[i], data_kb))
//        {
//            flattenedClauses.push_back(m_facts[i]);
//        }
//    }
//
//    for(size_t i = 0;i < flattenedClauses.size();i++) Log::Info << flattenedClauses[i] << std::endl;
//
//    for(size_t i = 0;i < m_clauses.size();i++)
//    {
//        std::string head;
//        StringSet temp;
//        Log::Info << m_clauses[i] << std::endl;
//        Log::Info << ProcessClause(m_clauses[i], head, temp, data_kb, i) << std::endl;
//        Log::Debug << head << std::endl;
//        Log::Debug << m_clauses[i] << std::endl;
//
//    }
//
//    return KIF();
////    int isDebug = 2;
////
////    std::list<std::string> FlattenedClauses;
////
////    std::map<logicbase::RelationSignature, std::vector<size_t> > domain_map;
////
////    KnowledgeBase data_kb = GetDataKnowledge(domain_map);
////    data_kb.optimizeSpecialRelations();
////
////    KnowledgeBase all_kb;
////
////    const StringVec& m_facts = kif.Facts();
////    const StringVec& m_clauses = kif.Clauses();
////
////    // add processed facts to temporary knowledge base
////    StringSet tset;
////    for(size_t i = 0; i < m_facts.size(); i++) all_kb.add(ProcessProposition(m_facts[i], tset, data_kb));
////    tset.clear();
////
////    // Array to indicate which clauses should be considered for next pass
////    bool clausesToConsider[m_clauses.size()];
////
////    // Initialize the array to all 1
////    for(size_t i = 0; i < m_clauses.size(); i++) clausesToConsider[i] = true;
////
////    std::map<logicbase::RelationSignature, size_t> newly_added;
////    StringSet prop_map;
////
////    bool anyNew = true;
////
////    // Helps to neglect non-variable(basically flattened) clauses not to be considered after first pass
////    bool isSecondTime = false;
////
////    while(anyNew)
////    {
////        for(size_t c_index = 0; c_index < m_clauses.size(); c_index++)
////        {
////            const std::string& clause = m_clauses[c_index];
////
////            if(!clausesToConsider[c_index]) continue;
////
////            clausesToConsider[c_index] = false;
////
////            if(isDebug > 1)
////            {
////                Log::Debug << "Unprocessed clause is" << std::endl;
////                Log::Debug << clause << std::endl;
////            }
////
////            std::string question;
////            std::string processed_clause = ProcessClause(clause, question, tset, data_kb);
////            tset.clear();
////
////            if(isDebug > 1)
////            {
////                Log::Debug << "Processed clause is " << std::endl;
////                Log::Debug << processed_clause << " with domains" << std::endl;
////            }
////
////            // variables in the clause
////            StringSet vars = KIFSyntax::getVariables(clause);
////
////            // If the clause is already flattened
////            if(vars.size() == 0)
////            {
////                // Dont consider the flattened clause in second pass
////                if(!isSecondTime)
////                {
////                    logicbase::RelationSignature h_sig(question);
////
////                    newly_added[h_sig] = c_index;
////
////                    const std::string& cmd = h_sig.name;
////                    if(cmd != "legal" && cmd != "goal" && cmd != "terminal" && cmd != "next" && cmd != "init")
////                        all_kb.add(question);
////
////                    FlattenedClauses.push_back(clause);
////                }
////                continue;
////            }
////
////            if(isDebug > 1) Log::Debug << "question is " << question << ", clause is " << processed_clause << std::endl;;
////
////            StringVec patterns;
////            patterns.push_back(clause);
////            patterns.push_back(question);
////
////            std::vector<StringVec> mul_result = all_kb.added(processed_clause)->ask_multiple_patterns(question, patterns, true);
////
////            const StringVec& result = mul_result[0];
////            const StringVec& temp_prop = mul_result[1];
////
////            // Adds appropriate props to kb for flattening further clasues
////            // Adds clauses to final clause list while avoiding duplicates
////            // Also updates newly added list accordingly
////            for(size_t r_index = 0; r_index < result.size(); r_index++)
////            {
////                const std::string& clause = result[r_index];
////                const std::string& toCheck = temp_prop[r_index];
////
////                std::string temp;
////                StringVec args;
////                KIFSyntax::separateCommand(clause, temp, args);
////                std::string head = args[0];
////
////                logicbase::RelationSignature h_sig(head);
////                newly_added[h_sig] = c_index;
////
////                std::ostringstream c_index_str;
////                c_index_str << c_index;
////
////                const StringSet::const_iterator it = prop_map.find(toCheck + "_" + c_index_str.str());
////                if(it == prop_map.end())
////                {
////                    prop_map.insert(toCheck + "_" + c_index_str.str());
////                    FlattenedClauses.push_back(clause);
////                }
////
////                const std::string& cmd = h_sig.name;
////                if(cmd != "legal" && cmd != "next" && cmd != "goal" && cmd != "terminal" && cmd != "init")
////                {
////                    // Add head to knowledge base for flattening further clauses
////                    all_kb.add(head);
////                }
////            }
////
////        }
////    }
//}
//
//std::string GDL::Compress(const std::string& str) const
//{
//    if(kif.IsCompressed())
//    {
//        std::string out;
//        kif.Compress(str, out);
//        return out;
//    }
//    return str;
//}
//
//int** GDL::cached_GetGoal(const State& state, size_t rid) const
//{
//    (void)state; // to remove unused warning
//
//    // get goal value for the given role
//    StringVec result = base_rules.ask("(" + Compress("goal") + " " + roles[rid] + " ?x)","?x", true);
//
//    // create an array and update the slot corresponding to the given role with the goal value
//    // all the other values are initialized to -1
//    int** out = new int*[1];
//    out[0] = new int[roles.size()];
//    for(size_t i = 0; i < roles.size(); i++) (*out)[i] = -1;
//    (*out)[rid] = atoi(result[0].c_str());
//
//    return out;
//}
//


//std::vector<GDL::StringVec>* GDL::cached_GetLegalJointMoves(const State& state) const
//{
//    (void)state; // to remove unused warning
//
//    // get legal moves for all the roles
//    StringVec result[roles.size()];
//    for(size_t i = 0; i < roles.size(); i++)
//        result[i] = base_rules.ask("(" + Compress("legal") + " " + roles[i] + " ?x)","?x",true);
//
//    // compute and store all the combination of legal moves of all the roles
//    std::vector<StringVec>* all = new std::vector<StringVec>();
//    size_t index[roles.size()];
//    for(size_t i = 0; i < roles.size(); i++) index[i] = 0;
//
//    // if there is more than 1 role
//    if(roles.size() > 1)
//    {
//        bool isDone = false;
//        // loop till all the legal move combinations encountered
//        while(true)
//        {
//            StringVec temp;
//            for(size_t i = 0; i < roles.size(); i++) temp.push_back(result[i][index[i]]);
//            all->push_back(temp);
//            index[0]++;
//            for(size_t i = 0; i < roles.size(); i++)
//            {
//                if(index[i] == result[i].size())
//                {
//                    if(i == roles.size() - 1)
//                    {
//                        isDone = true;
//                        break;
//                    }
//                    index[i] = 0;
//                    index[i + 1]++;
//                }
//            }
//            if(isDone) break;
//        }
//    }
//    // if only 1 role...
//    else
//    {
//        for(size_t i = 0; i < result[0].size(); i++)
//        {
//            StringVec temp;
//            temp.push_back(result[0][i]);
//            all->push_back(temp);
//        }
//    }
//
//    return all;
//}
//

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
