/**
 * @file kif_flattener.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation of KIFFlattener class.
 */
#include "kif_flattener.hpp"

#include <map>
#include <stack>

#include <libgdl/core/util/setop.hpp>

using namespace std;
using namespace libgdl;
using namespace libgdl::core;
using namespace libgdl::gdlreasoner;
using namespace libgdl::gdlparser;
using namespace libgdl::gdlreasoner::logicbase;

void KIFFlattener::Flatten(KIF& kif)
{
  symbol_table = kif.GetSymbolTable();

  // clear all the lists
  flattened_facts.clear();
  flattened_clauses.clear();

  DGraph graph = kif.DependencyGraph();

  // dependency graph
  const map<size_t, DGraphNode*>& dgraph = graph.GetGraph();

  // construct a knowledge base consisting of entire knowledge
  KnowledgeBase all_kb(kif);

  // the temporary knowledge base
  KnowledgeBase m_kb;
  m_kb.SetSymbolTable(symbol_table);

  // stores marked relations in Dfs
  set<size_t> marked;

  // stores data relations(which are also state independent)
  set<size_t> state_independent;

  map<size_t, DGraphNode*>::const_iterator it;
  set<size_t>::iterator mit;

  // mark all the relation dependent on 'does'
  if((it = dgraph.find(SymbolTable::DoesID)) != dgraph.end())
    DFSMarking(it->second, marked);
  //mark all the relations dependent on 'true'
  if((it = dgraph.find(SymbolTable::TrueID)) != dgraph.end())
    DFSMarking(it->second, marked);

  //compute state independent knowledge and add it to temporary knowledge base
  for(it = dgraph.begin(); it != dgraph.end(); it++)
  {
    if((mit = marked.find(it->first)) == marked.end())
    {
      const size_t& sig = it->first;
      if(sig != SymbolTable::BaseID &&
         sig != SymbolTable::InputID &&
         sig != SymbolTable::InitID &&
         sig != SymbolTable::RoleID &&
         sig != SymbolTable::GoalID &&
         sig != SymbolTable::TerminalID &&
         sig != SymbolTable::LegalID &&
         sig != SymbolTable::NextID)
      {
        state_independent.insert(sig);

        const KnowledgeBase::FactList* data_facts = all_kb.GetFacts(sig);
        if(data_facts != NULL)
        {
          for(list<Fact>::const_iterator it = data_facts->begin();
                                                  it != data_facts->end();it++)
          m_kb.Tell(*it);
        }

        const KnowledgeBase::ClauseList* data_clauses = all_kb.GetClauses(sig);
        if(data_clauses != NULL)
        {
          for(list<Clause>::const_iterator it = data_clauses->begin();
                                                it != data_clauses->end();it++)
          m_kb.Tell(*it);
        }
      }
    }
  }

  // start bottom up dependency flattening
  // start with those relation which are not dependent on other
  // then use dfs to flatten all the relations bottom up
  set<size_t> relations_done;
  for(it = dgraph.begin(); it != dgraph.end(); it++)
  {
    if((mit = relations_done.find(it->first)) == relations_done.end())
    {
      stack<const DGraphNode*> S_n;
      stack<size_t> S_i;

      S_n.push(it->second);
      S_i.push(0);

      relations_done.insert(it->second->id);

      while(!S_n.empty())
      {
        const DGraphNode* temp = S_n.top();
        S_n.pop();
        size_t index = S_i.top();
        S_i.pop();

        const vector<DGraphNode*>& in = temp->in;
        if(index == in.size())
        {
          if(state_independent.find(temp->id) == state_independent.end())
          {
            FlattenRelation(temp, all_kb, state_independent,
                            m_kb, flattened_clauses, flattened_facts);
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

void KIFFlattener::DFSMarking(const DGraphNode* n, set<size_t>& marked)
{
  stack<const DGraphNode*> S_n;
  stack<size_t> S_i;

  S_n.push(n);
  S_i.push(0);

  set<size_t> visited;
  visited.insert(n->id);

  while(!S_n.empty())
  {
    const DGraphNode* n = S_n.top();
    S_n.pop();

    size_t index = S_i.top();
    S_i.pop();

    marked.insert(n->id);

    if(index == n->out.size()) continue;

    S_n.push(n);
    S_i.push(index + 1);


    if(visited.find(n->out[index]->id) == visited.end())
    {
      S_n.push(n->out[index]);
      S_i.push(0);
      visited.insert(n->out[index]->id);
    }
  }
}

void KIFFlattener::FlattenRelation(const DGraphNode* n,
                                   const KnowledgeBase& all_kb,
                                   const std::set<size_t>& state_independent,
                                   KnowledgeBase& m_kb,
                                   std::list<Clause>& f_clauses,
                                   std::list<Fact>& f_facts)
{
  //compute signature of relation
  const size_t& sig = n->id;

  // get all the facts and clauses associated with this signature
  const KnowledgeBase::FactList* facts = all_kb.GetFacts(sig);
  const KnowledgeBase::ClauseList* p_clauses = all_kb.GetClauses(sig);

  if(p_clauses == NULL)
  {
    if(facts != NULL)
      for(KnowledgeBase::FactList::const_iterator it = facts->begin();
                                                      it != facts->end();it++)
      {
        f_facts.push_back(*it);
        m_kb.Tell(*it);
      }
    return;
  }

  const KnowledgeBase::ClauseList& clauses = *p_clauses;

  // to store the heads of the flattened clauses
  // these will be added later to temporary knowledge base
  list<Argument*> f_heads;

  // start flattening clauses
  for(list<Clause>::const_iterator it = clauses.begin();it != clauses.end();it++)
  {
    // if the clause is already ground add it directly
    // add its head to heads list
    if((*it).IsGround())
    {
      Clause* to_add = new Clause(*it);
      f_clauses.push_back(*to_add);
      f_heads.push_back(to_add->head);
      to_add->head = NULL;
      delete to_add;
      continue;
    }

    // pre-process the clause
    // adjust the extra variables which are present in body but not head
    // adjust 'not' relation appropriately
    Clause* p_clause = ProcessClause(*it, state_independent);

    // add the processed clause temporarily to knowledge base
    size_t c_index = m_kb.Tell(*p_clause);

    VariableMap h_v_map;
    Argument* question = SpecialArgCopy2(p_clause->head, h_v_map);

    // after adding the head of the clause will be the question to ask
    Answer* ans = m_kb.GetAnswer(*question, VariableMap(), set<size_t>());


    while(ans->next())
    {
      VariableMap ans_v_map = ans->GetVariableMap();
      for(auto va : h_v_map)
        ans_v_map.insert(va);

      // compute the answer with substitution
      Clause* to_add = Unify::GetSubstitutedClause(&(*it), ans_v_map);

      // remove all the occurrences of data relations
      Clause* temp = RemoveDataFromClause(to_add, state_independent);

      // check whether clause can be converted to fact after data relations removal
      if(temp == NULL)
      {
        Argument* h = to_add->head;
        to_add->head = NULL;
        delete to_add;
        f_facts.push_back(*h);
        f_facts.back().loc = (*it).loc;
        f_facts.back().isLocation = (*it).isLocation;
        f_heads.push_back(h);
      }
      else
      {
        f_clauses.push_back(*temp);
        f_clauses.back().loc = (*it).loc;
        f_clauses.back().isLocation = (*it).isLocation;
        f_heads.push_back(temp->head);
        temp->head = NULL;
        delete temp;
      }
    }

    // delete answer
    delete ans;

    // erase the temporary knowledge from knowledge base
    m_kb.Erase(*p_clause, c_index);

    // delete the processed clause(without deleting the variables
    SpecialClauseDelete(p_clause);

    delete question;
  }

  // add all the facts related to this relation to knowledge base
  if(facts != NULL)
    for(list<Fact>::const_iterator it = facts->begin();it != facts->end();it++)
    {
      f_facts.push_back(*it);
      m_kb.Tell(*it);
    }

  // compute signature and add to appropriate FactVec
  size_t command = n->id;
  KnowledgeBase::FactList& fl = m_kb.m_facts[command];

  // add heads of all the clauses to knowledge base
  for(list<Argument*>::iterator it = f_heads.begin();it != f_heads.end();it++)
  {
    Fact f;
    f.arg = *it;

    fl.push_back(std::move(f));
  }
}

Clause* KIFFlattener::RemoveDataFromClause(Clause* c,
                                           const set<size_t>& state_independent)
{
  std::vector<Argument*> args;
  for(size_t i = 0;i < c->premisses.size();i++)
  {
    Argument* p = c->premisses[i];

    const size_t& cmd = p->value;

    // delete premiss if data relation
    if(state_independent.find(cmd) != state_independent.end())
    {
      delete p;
    }
    // these premisses are no longer required
    else if(cmd == SymbolTable::DistinctID ||
            cmd == SymbolTable::RoleID ||
            cmd == SymbolTable::InputID ||
            cmd == SymbolTable::BaseID)
    {
      delete p;
    }
    // check if there is a data relation inside 'not' relation
    else if(cmd == SymbolTable::NotID)
    {
      const size_t& arg_cmd = p->args[0]->value;

      if(state_independent.find(arg_cmd) != state_independent.end())
        delete p;

      else args.push_back(p);
    }
    // remove data relation inside 'or'
    else if(cmd == SymbolTable::OrID)
    {
      Argument* temp = RemoveDataFromArgument(p, state_independent);

      if(temp != NULL)
        args.push_back(temp);
      else delete p;
    }
    else args.push_back(p);
  }
  if(args.size() == 0)
  {
    c->premisses.clear();
    return NULL;
  }

  c->premisses = args;
  return c;
}

Argument*
  KIFFlattener::RemoveDataFromArgument(Argument* arg,
                                       const set<size_t>& state_independent)
{
  std::vector<Argument*> args;
  for(size_t i = 0;i < arg->args.size();i++)
  {
    Argument* p = arg->args[i];

    const size_t& cmd = p->value;

    if(state_independent.find(cmd) != state_independent.end())
    {
      delete p;
    }
    else if(cmd == SymbolTable::DistinctID ||
            cmd == SymbolTable::RoleID ||
            cmd == SymbolTable::InputID ||
            cmd == SymbolTable::BaseID)
    {
      delete p;
    }
    else if(cmd == SymbolTable::NotID)
    {
      const size_t& arg_cmd = p->args[0]->value;

      if(state_independent.find(arg_cmd) != state_independent.end())
        delete p;
      else args.push_back(p);
    }
    else if(cmd == SymbolTable::OrID)
    {
      Argument* temp = RemoveDataFromArgument(p, state_independent);

      if(temp != NULL)
        args.push_back(temp);
      else delete p;
    }
    else args.push_back(p);
  }
  if(args.size() == 0)
  {
    arg->args.clear();
    return NULL;
  }

  arg->args = args;
  return arg;
}

Clause* KIFFlattener::ProcessClause(const Clause& c,
                                    const set<size_t>& state_independent)
{
  std::set<Argument*> head_vars;

  Clause* out = new Clause();

  // make special copy of head
  // shallow copy for variables
  out->head = SpecialArgCopy(c.head, head_vars);

  // make special copy of all the premisses and pre-process them
  for(size_t i = 0; i < c.premisses.size(); i++)
  {
    std::set<Argument*> p_args;

    Argument* p = SpecialArgCopy(c.premisses[i], p_args);
    p = ProcessPremiss(p, state_independent);

    out->premisses.push_back(p);

    set<Argument*> e_vars = util::setop::setDifference(p_args, head_vars);
    for(set<Argument*>::iterator it = e_vars.begin(); it != e_vars.end(); it++)
    {
      out->head->args.push_back(*it);
      head_vars.insert(*it);
    }
  }

  return out;
}

Argument* KIFFlattener::SpecialArgCopy(Argument* arg, set<Argument*>& vars)
{
  if(arg->IsVariable())
  {
    vars.insert(arg);
    return arg;
  }

  Argument* out = new Argument();
  out->t = arg->t;
  out->value = arg->value;

  for(size_t i = 0; i < arg->args.size(); i++)
    out->args.push_back(SpecialArgCopy(arg->args[i], vars));

  return out;
}

Argument* KIFFlattener::SpecialArgCopy2(Argument* arg, VariableMap& v_map)
{
  if(arg->IsVariable())
  {
    Argument* out = new Argument(*arg);
    v_map[arg] = out;
    return out;
  }

  Argument* out = new Argument();
  out->t = arg->t;
  out->value = arg->value;

  for(size_t i = 0; i < arg->args.size(); i++)
    out->args.push_back(SpecialArgCopy2(arg->args[i], v_map));

  return out;
}

void KIFFlattener::SpecialArgDelete(Argument* arg)
{
  if(arg->IsVariable()) return;

  for(size_t i = 0;i < arg->args.size();i++)
    SpecialArgDelete(arg->args[i]);

  arg->args.clear();
  delete arg;
}

void KIFFlattener::SpecialClauseDelete(Clause* c)
{
  SpecialArgDelete(c->head);
  c->head = NULL;

  for(size_t i = 0;i < c->premisses.size();i++)
    SpecialArgDelete(c->premisses[i]);

  c->premisses.clear();
  delete c;
}

Argument* KIFFlattener::ProcessPremiss(Argument* p,
                                       const set<size_t>& state_independent)
{
  size_t& sig = p->value;

  // convert does premisses to input premisses
  if(sig == SymbolTable::DoesID) sig = SymbolTable::InputID;
  // convert true premisses to base premisses
  else if(sig == SymbolTable::TrueID) sig = SymbolTable::BaseID;
  // if relation inside is not is state independent keep the not
  // else remove it
  else if(sig == SymbolTable::NotID)
  {
    Argument* arg = p->args[0];
    p->args.clear();

    const size_t& cmd = p->args[0]->value;
    set<size_t>::const_iterator it = state_independent.find(cmd);
    if(it == state_independent.end() &&
       cmd != SymbolTable::DistinctID &&
       cmd != SymbolTable::RoleID &&
       cmd != SymbolTable::InputID &&
       cmd != SymbolTable::BaseID &&
       cmd != SymbolTable::InitID)
    {
      delete p;
      return ProcessPremiss(arg, state_independent);
    }

    p->args.push_back(ProcessPremiss(arg, state_independent));
  }
  // recursively call process on each argument of 'or'
  else if(sig == SymbolTable::OrID)
  {
    std::vector<Argument*> args = p->args;
    p->args.clear();

    for(size_t i = 0; i < args.size(); i++)
      p->args.push_back(ProcessPremiss(args[i], state_independent));
  }

  return p;
}

bool KIFFlattener::PrintToFile(const std::string& filename)
{
  std::ofstream f(filename.c_str());

  if(!f.is_open()) return false;

  SymbolDecodeStream myfile(symbol_table, f);
  for(list<Fact>::iterator it = flattened_facts.begin();
                                              it != flattened_facts.end();it++)
  {
    myfile << *it << std::endl;
  }

  for(list<Clause>::iterator it = flattened_clauses.begin();
                                            it != flattened_clauses.end();it++)
  {
    myfile << *it << std::endl;
  }

  f.close();
  return true;
}
