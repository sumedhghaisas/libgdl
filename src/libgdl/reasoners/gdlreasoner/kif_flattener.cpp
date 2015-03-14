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
#include <libgdl/core/symbol_table/symbol_decode_stream.hpp>

size_t combination_optimization_index = 0;

using namespace std;
using namespace libgdl;
using namespace libgdl::core;
using namespace libgdl::gdlreasoner;
using namespace libgdl::gdlparser;
using namespace libgdl::gdlreasoner::logicbase;

set<size_t> KIFFlattener::GetStateIndependentRelations(const KnowledgeBase& all_kb,
                                                       KnowledgeBase& m_kb,
                                                       const map<size_t, DGraphNode*>& dgraph)
{
  set<size_t> state_independent;

  // stores marked relations in Dfs
  std::set<size_t> marked;

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

  return state_independent;
}

void KIFFlattener::AddStateIndependentRelationToCache(const set<size_t>& state_independent,
                                                      const map<size_t, DGraphNode*>& dgraph,
                                                      KnowledgeBase& m_kb)
{
  for(auto it : state_independent)
  {
    set<size_t> visited;

    stack<const DGraphNode*> s;
    s.push(dgraph.find(it)->second);

    bool isRecursive = false;

    while(!s.empty())
    {
      const DGraphNode* n = s.top();
      s.pop();

      if(visited.find(n->id) != visited.end())
      {
        isRecursive = true;
        break;
      }

      for(auto node : n->in)
      {
        s.push(node);
      }
    }

    if(!isRecursive)
      m_kb.AddCacheRel(it);
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

void KIFFlattener::InitializePropnetOptimizer(const Clause& c,
                                              Clause* temp,
                                              list<Clause>& opt_clauses,
                                              Argument* opt_args)
{
  opt_args[0] = *temp->premisses[0];
  opt_args[1] = *temp->premisses[1];

  opt_clauses.push_back(*temp);
  opt_clauses.back().loc = c.loc;
  opt_clauses.back().isLocation = c.isLocation;
}

bool KIFFlattener::IsSelfRecursive(const Clause& c)
{
  size_t sig = c.head->value;

  for(auto premiss : c.premisses)
  {
    size_t p_id = premiss->value;

    if(p_id == SymbolTable::OrID)
    {
      for(auto arg : premiss->args)
      {
        if(arg->value == sig)
          return true;
      }
    }
    else
    {
      if(p_id == sig)
        return true;
    }
  }

  return false;
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
