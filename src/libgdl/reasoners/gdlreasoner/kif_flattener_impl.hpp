#include <libgdl/core/symbol_table/symbol_decode_stream.hpp>

namespace libgdl
{
namespace gdlreasoner
{

template<typename Collector>
void KIFFlattener::Flatten(gdlparser::KIF& kif,
                           Collector& collector,
                           bool useCache)
{
  bool toOptimizeForPropnet = false;

  core::SymbolTable symbol_table = kif.GetSymbolTable();

  DGraph graph = kif.DependencyGraph();

  // dependency graph
  const std::map<size_t, DGraphNode*>& dgraph = graph.GetGraph();

  // construct a knowledge base consisting of entire knowledge
  KnowledgeBase all_kb(kif);

  // the temporary knowledge base
  KnowledgeBase m_kb;
  m_kb.SetSymbolTable(symbol_table);

  // stores data relations(which are also state independent)
  std::set<size_t> state_independent = GetStateIndependentRelations(all_kb, m_kb, dgraph);

  if(useCache)
    AddStateIndependentRelationToCache(state_independent, dgraph, m_kb);

  // start bottom up dependency flattening
  // start with those relation which are not dependent on other
  // then use dfs to flatten all the relations bottom up
  std::set<size_t> relations_done;
  for(auto it = dgraph.begin(); it != dgraph.end(); it++)
  {
    auto mit = relations_done.find(it->first);
    if(mit == relations_done.end())
    {
      std::stack<const DGraphNode*> S_n;
      std::stack<size_t> S_i;

      S_n.push(it->second);
      S_i.push(0);

      relations_done.insert(it->second->id);

      while(!S_n.empty())
      {
        const DGraphNode* temp = S_n.top();
        S_n.pop();
        size_t index = S_i.top();
        S_i.pop();

        const std::vector<DGraphNode*>& in = temp->in;
        if(index == in.size())
        {
          if(state_independent.find(temp->id) == state_independent.end())
          {
            FlattenRelation(temp, all_kb, state_independent,
                            m_kb, collector, useCache, toOptimizeForPropnet);
          }
          continue;
        }

        S_n.push(temp);
        S_i.push(index + 1);

        if(relations_done.find(in[index]->id) == relations_done.end())
        {
          S_n.push(in[index]);
          S_i.push(0);
          relations_done.insert(in[index]->id);
        }
      }
    }
  }
}

template<typename Collector>
void KIFFlattener::FlattenRelation(const DGraphNode* n,
                                   const KnowledgeBase& all_kb,
                                   const std::set<size_t>& state_independent,
                                   KnowledgeBase& m_kb,
                                   Collector& collector,
                                   bool useCache,
                                   bool toOptimizeForPropnet)
{
  //compute signature of relation
  const size_t& sig = n->id;

  std::list<Argument*> f_heads;

  // get all the facts and clauses associated with this signature
  const KnowledgeBase::FactList* facts = all_kb.GetFacts(sig);
  const KnowledgeBase::ClauseList* p_clauses = all_kb.GetClauses(sig);

  if(p_clauses == NULL)
  {
    if(facts != NULL)
      for(KnowledgeBase::FactList::const_iterator it = facts->begin();
                                                      it != facts->end();it++)
      {
        //f_facts.push_back(*it);
        collector.AddFact(*it);
        m_kb.Tell(*it);
      }
    return;
  }

  const KnowledgeBase::ClauseList& clauses = *p_clauses;

  std::list<const Clause*> rec_clauses;

  // start flattening clauses
  for(std::list<Clause>::const_iterator it = clauses.begin();it != clauses.end();it++)
  {
    core::SymbolDecodeStream sds(all_kb.GetSymbolTable());
    sds << *it << std::endl;

    FlattenClause(*it, state_independent, all_kb.GetSymbolTable(), f_heads, collector, rec_clauses, m_kb, combination_optimization_index, toOptimizeForPropnet);
  }

  // add all the facts related to this relation to knowledge base
  if(facts != NULL)
    for(std::list<Fact>::const_iterator it = facts->begin();it != facts->end();it++)
    {
      //f_facts.push_back(*it);
      collector.AddFact(*it);
      m_kb.Tell(*it);
    }

  // compute signature and add to appropriate FactVec
  size_t command = n->id;
  KnowledgeBase::FactList& fl = m_kb.m_facts[command];

  std::set<size_t> head_hashes;

  // add heads of all the clauses to knowledge base
  for(std::list<Argument*>::iterator it = f_heads.begin();it != f_heads.end();it++)
  {
    //core::SymbolDecodeStream sds(all_kb.GetSymbolTable());
    //sds << **it << std::endl;

    Fact f;
    f.arg = *it;

    size_t h_hash = f.arg->Hash(all_kb.GetSymbolTable());

    if(head_hashes.find(h_hash) == head_hashes.end())
    {
      head_hashes.insert(h_hash);
      fl.push_back(std::move(f));
    }
  }

  std::list<Argument*> rec_f_heads;

  //SymbolDecodeStream sds(symbol_table);

  SymbolTable symbol_table = all_kb.GetSymbolTable();

  for(auto c_rec : rec_clauses)
  {
    FlattenRecursiveClause(*c_rec, state_independent, symbol_table, rec_f_heads, collector, m_kb);
  }

  // add heads of all the clauses to knowledge base
  for(std::list<Argument*>::iterator it = rec_f_heads.begin();it != rec_f_heads.end();it++)
  {
    Fact f;
    f.arg = *it;

    size_t h = f.arg->Hash(all_kb.GetSymbolTable());
    if(head_hashes.find(h) == head_hashes.end())
    {
      fl.push_back(std::move(f));
      head_hashes.insert(h);
    }
  }

  //cout << symbol_table.GetCommandName(sig) << endl;

  m_kb.AddCacheRel(sig);
}

template<typename Collector>
void KIFFlattener::FlattenRecursiveClause(const Clause& clause,
                                          const std::set<size_t>& state_independent,
                                          core::SymbolTable& symbol_table,
                                          std::list<Argument*>& rec_f_heads,
                                          Collector& collector,
                                          KnowledgeBase& m_kb)
{
  Clause* p_clause = ProcessClause(clause, state_independent);

  size_t head_id = p_clause->head->value;

  std::string head_command = symbol_table->GetCommandName(head_id);

  head_command += "_recurse";

  size_t rec_head_id = symbol_table.AddEntry(head_command, core::Location());

  p_clause->head->value = rec_head_id;

  Clause* rec_clause = new Clause(*p_clause);
  rec_clause->head->value = rec_head_id;
  for(auto premiss : rec_clause->premisses)
  {
    if(premiss->value == SymbolTable::OrID)
    {
      for(auto arg : premiss->args)
      {
        if(arg->value == head_id)
          arg->value = rec_head_id;
      }
    }
    else if(premiss->value == head_id)
      premiss->value = rec_head_id;
  }

  //sds << *p_clause << endl;
  //sds << *rec_clause << endl;

  // add the processed clause temporarily to knowledge base
  size_t c_index = m_kb.Tell(*p_clause);
  size_t rec_c_index = m_kb.Tell(*rec_clause);

  core::VariableMap h_v_map;
  Argument* question = SpecialArgCopy2(p_clause->head, h_v_map);

  // after adding the head of the clause will be the question to ask
  logicbase::Answer* ans = m_kb.GetAnswer(*question, core::VariableMap(), std::set<size_t>());

  bool toAdd = false;
  if(question->value != core::SymbolTable::LegalID &&
     question->value != core::SymbolTable::NextID &&
     question->value != core::SymbolTable::InitID)
    toAdd = true;

  while(ans->next())
  {
    core::VariableMap ans_v_map = ans->GetVariableMap();
    for(auto va : h_v_map)
      ans_v_map.insert(va);

    // compute the answer with substitution
    Clause* to_add = logicbase::Unify::GetSubstitutedClause(&clause, ans_v_map);

    //sds << *to_add << endl;

    // remove all the occurrences of data relations
    Clause* temp = RemoveDataFromClause(to_add, state_independent);

    //sds << *temp << endl;

    // check whether clause can be converted to fact after data relations removal
    if(temp == NULL)
    {
      collector.AddFact(*to_add->head);
      if(toAdd)
      {
        rec_f_heads.push_back(to_add->head);
        to_add->head = NULL;
      }
      delete to_add;
//      f_facts.push_back(*h);
//      f_facts.back().loc = clause.loc;
//      f_facts.back().isLocation = clause.isLocation;
    }
    else
    {
//      f_clauses.push_back(*temp);
//      f_clauses.back().loc = clause.loc;
//      f_clauses.back().isLocation = clause.isLocation;
      collector.AddClause(*temp);

      if(toAdd)
      {
        rec_f_heads.push_back(temp->head);
        temp->head = NULL;
      }
      delete temp;
    }
  }

  // delete answer
  delete ans;

  // erase the temporary knowledge from knowledge base
  m_kb.Erase(*rec_clause, rec_c_index);
  m_kb.Erase(*p_clause, c_index);

  // delete the processed clause(without deleting the variables
  SpecialClauseDelete(p_clause);

  delete rec_clause;

  delete question;
}

template<typename Collector>
void KIFFlattener::FlattenClause(const Clause& clause,
                                 const std::set<size_t>& state_independent,
                                 const SymbolTable& symbol_table,
                                 std::list<Argument*>& f_heads,
                                 Collector& collector,
                                 std::list<const Clause*>& rec_clauses,
                                 KnowledgeBase& m_kb,
                                 size_t& combination_optimization_index,
                                 bool toOptimizeForPropnet)
{
  core::SymbolDecodeStream sds(symbol_table);

  //sds << *it << endl;

  // if the clause is already ground add it directly
  // add its head to heads list
  if(clause.IsGround())
  {
    Clause* to_add = new Clause(clause);
    //f_clauses.push_back(*to_add);
    collector.AddClause(*to_add);
    f_heads.push_back(to_add->head);
    to_add->head = NULL;
    delete to_add;
    return;
  }

  if(this->IsSelfRecursive(clause))
  {
    rec_clauses.push_back(&clause);
    return;
  }

  // pre-process the clause
  // adjust the extra variables which are present in body but not head
  // adjust 'not' relation appropriately
  Clause* p_clause = ProcessClause(clause, state_independent);

  // add the processed clause temporarily to knowledge base
  size_t c_index = m_kb.Tell(*p_clause);

  core::VariableMap h_v_map;
  Argument* question = SpecialArgCopy2(p_clause->head, h_v_map);

  // after adding the head of the clause will be the question to ask
  logicbase::Answer* ans = m_kb.GetAnswer(*question, core::VariableMap(), std::set<size_t>());

  Argument opt_args[2];
  bool is_initialized = false;

  std::list<Clause> opt_clauses;

  bool toAdd = false;
  if(question->value != core::SymbolTable::LegalID &&
     question->value != core::SymbolTable::NextID &&
     question->value != core::SymbolTable::InitID)
  {
    //std::cout << "Adding to knowledge..." << std::endl;
    toAdd = true;
  }

  while(ans->next())
  {
    core::VariableMap ans_v_map = ans->GetVariableMap();
    for(auto va : h_v_map)
      ans_v_map.insert(va);

    // compute the answer with substitution
    Clause* to_add = logicbase::Unify::GetSubstitutedClause(&clause, ans_v_map);

    sds << *to_add << std::endl;

    // remove all the occurrences of data relations
    Clause* temp = RemoveDataFromClause(to_add, state_independent);

    // check whether clause can be converted to fact after data relations removal
    if(temp == NULL)
    {
      collector.AddFact(*to_add->head);
      if(toAdd)
      {
        f_heads.push_back(to_add->head);
        to_add->head = NULL;
      }
      delete to_add;
//      f_facts.push_back(*h);
//      f_facts.back().loc = clause.loc;
//      f_facts.back().isLocation = clause.isLocation;

      if(is_initialized)
      {
        OptimizeClausesForPropnet(opt_clauses, opt_args, symbol_table, combination_optimization_index, collector);
        is_initialized = false;
      }
    }
    else
    {
      if(toOptimizeForPropnet)
      {
        std::cout << "Optimizing for propnet..." << std::endl;
        if(temp->premisses.size() > 2)
        {
          if(!is_initialized)
          {
            InitializePropnetOptimizer(clause, temp, opt_clauses, opt_args);
            is_initialized = true;
          }

          if(*temp->premisses[0] == opt_args[0] && *temp->premisses[1] == opt_args[1])
          {
            opt_clauses.push_back(*temp);
            opt_clauses.back().loc = clause.loc;
            opt_clauses.back().isLocation = clause.isLocation;
          }
          else
          {
            OptimizeClausesForPropnet(opt_clauses, opt_args, symbol_table, combination_optimization_index, collector);

            opt_clauses.clear();

            InitializePropnetOptimizer(clause, temp, opt_clauses, opt_args);
          }
        }
        else
        {
  //        f_clauses.push_back(*temp);
  //        f_clauses.back().loc = clause.loc;
  //        f_clauses.back().isLocation = clause.isLocation;
          collector.AddClause(*temp);

          if(is_initialized)
          {
            OptimizeClausesForPropnet(opt_clauses, opt_args, symbol_table, combination_optimization_index, collector);

            opt_clauses.clear();
            is_initialized = false;
          }
        }
      }
      else collector.AddClause(*temp);

      if(toAdd)
      {
        f_heads.push_back(temp->head);
        temp->head = NULL;
      }
      delete temp;
    }
  }

  if(is_initialized)
  {
    OptimizeClausesForPropnet(opt_clauses, opt_args, symbol_table, combination_optimization_index, collector);
    is_initialized = false;
  }

  // delete answer
  delete ans;

  // erase the temporary knowledge from knowledge base
  m_kb.Erase(*p_clause, c_index);

  // delete the processed clause(without deleting the variables
  SpecialClauseDelete(p_clause);

  delete question;
}

template<typename Collector>
void KIFFlattener::OptimizeClausesForPropnet(std::list<Clause>& opt_clauses,
                                             Argument* opt_args,
                                             SymbolTable symbol_table,
                                             size_t& combination_optimization_index,
                                             Collector& collector)
{
  if(opt_clauses.size() > 3)
  {
    std::stringstream stream;
    stream << "combination_optimization_" << combination_optimization_index++;
    Argument* arg = new Argument(stream.str(), symbol_table, true);

    Clause c;
    c.head = arg;
    c.premisses.push_back(new Argument(opt_args[0]));
    c.premisses.push_back(new Argument(opt_args[1]));

    //f_clauses.push_back(c);
    collector.AddClause(c);

    for(auto it : opt_clauses)
    {
      delete it.premisses[0];
      delete it.premisses[1];

      it.premisses.erase(it.premisses.begin(), it.premisses.begin() + 2);

      it.premisses.push_back(new Argument(*arg));

      //f_clauses.push_back(it);
      collector.AddClause(it);
    }
    combination_optimization_index++;
  }
  else
  {
    for(auto it : opt_clauses)
    {
      //f_clauses.push_back(it);
      collector.AddClause(it);
    }
  }
}

}
}
