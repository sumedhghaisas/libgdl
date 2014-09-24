/**
 * @file dgraph.cpp
 * @author Sumedh Ghaisas
 *
 * Declaration DGraph class.
 */
#include "dgraph.hpp"

#include <sstream>

#include <libgdl/core/data_types/clause.hpp>

#include <libgdl/core/symbol_table/symbol_decode_stream.hpp>

using namespace std;
using namespace libgdl;
using namespace libgdl::core;

RawDGraph::~RawDGraph()
{
  for(map<size_t, DGraphNode*>::iterator it = graph.begin();it != graph.end();
                                                                          it++)
  {
    delete it->second;
  }
}

bool RawDGraph::AddNode(size_t id)
{
  map<size_t, DGraphNode*>::const_iterator it = graph.find(id);

  if(it != graph.end()) return false;
  graph[id] = new DGraphNode(id);
  return true;
}

bool RawDGraph::AddDependency(size_t h,
                              size_t r,
                              const Clause* c,
                              const Argument* arg,
                              bool isNot)
{
  map<size_t, DGraphNode*>::iterator it = graph.find(h);

  DGraphNode* head;
  if(it != graph.end())
    head = it->second;
  else return false;

  it = graph.find(r);

  DGraphNode* rel;
  if(it != graph.end())
    rel = it->second;
  else return false;

  rel->out.push_back(head);
  rel->clauses.push_back(c);
  rel->args.push_back(arg);
  rel->isNot.push_back(isNot);

  head->in.push_back(rel);
  return true;
}

void RawDGraph::StrongConnect(DGraphNode* v,
                              stack<DGraphNode*>& nstack,
                              set<DGraphNode*>& nset,
                              vector<set<DGraphNode*> >& scc)
{
  //! Tarjan's Strongly Connected Component Algorithm
  v->index = current_id;
  v->low_link = current_id;
  current_id++;
  nstack.push(v);
  nset.insert(v);

  std::vector<DGraphNode*>& out = v->out;

  for(size_t i = 0; i < out.size(); i++)
  {
    if(out[i]->index == -1)
    {
      StrongConnect(out[i], nstack, nset, scc);
      v->low_link = (v->low_link > out[i]->low_link) ? out[i]->low_link : v->low_link;
    }
    else if(nset.find(out[i]) != nset.end())
    {
      v->low_link = (v->low_link > out[i]->index) ? out[i]->index : v->low_link;
    }
  }

  if(v->index == v->low_link)
  {
    std::set<DGraphNode*> nscc;
    DGraphNode* temp = NULL;
    while(temp != v)
    {
      temp = nstack.top();
      nstack.pop();
      nset.erase(nset.find(temp));
      nscc.insert(temp);
    }
    scc.push_back(nscc);
  }
}

list<ErrorType> RawDGraph::CheckCycles(const SymbolTable& symbol_table)
{
  list<ErrorType> errors;

  // stores set of strongly connected components
  std::vector<std::set<DGraphNode*> > scc;
  // stack of processed DGraphNodes
  std::stack<DGraphNode*> nstack;
  // set of processed DGraphNodes(for faster lookup)
  std::set<DGraphNode*> nset;

  //! Tarjan's strongly connected component algorithm
  // current(unique) id to be given as index to every DGraphNode
  current_id = 0;
  // run until all DGraphNodes in the graph are visited
  for(std::map<size_t, DGraphNode*>::iterator it = graph.begin();
      it != graph.end(); it++)
  {
    DGraphNode* v = it->second;
    // get strongly connected component of each unvisited DGraphNode
    if(v->index == -1) StrongConnect(v, nstack, nset, scc);
  }

  //! check if there is any edge between DGraphNodes of same SCC marked negative
  //! if yes then it can be proved easily using SCC properties that a cycle
  //! exists with negative edge in it. Hence unstratified negation.
  for(size_t i = 0; i < scc.size(); i++)
  {
    const std::set<DGraphNode*>& sc = scc[i];

    for(set<DGraphNode*>::const_iterator it = sc.begin(); it != sc.end(); it++)
    {
      const vector<DGraphNode*>& out = (*it)->out;
      const vector<bool>& isNot = (*it)->isNot;

      for(size_t i = 0; i < out.size(); i++)
      {
        if(isNot[i] && sc.find(out[i]) != sc.end())
        {
          ErrorType error;
          error.AddEntry("Unstratified Negation involving relation '" +
                         symbol_table.GetCommandName((*it)->id) + "'", Location());
          errors.push_back(error);
        }
      }
    }
  }

  //! check that clause for any edge between DGraphNodes of same SCC
  //! definition 15(GDL_spec) is satisfied, else unstratified recursion
  for(size_t i = 0; i < scc.size(); i++)
  {
    const std::set<DGraphNode*>& sc = scc[i];

    for(set<DGraphNode*>::const_iterator it = sc.begin(); it != sc.end();it++)
    {
      const std::vector<DGraphNode*>& out = (*it)->out;
      const std::vector<bool>& isNot = (*it)->isNot;
      const std::vector<const Clause*>& clauses = (*it)->clauses;
      const std::vector<const Argument*>& args = (*it)->args;

      // for every out edge
      for(size_t i = 0; i < out.size(); i++)
      {
        // find non negative edges as negative edges are already considered for
        // unstratified negation
        // also check if the other DGraphNode is also in same scc
        // if it is then check if the clause corresponding to this edge satisfies
        // Definition 15(GDL_spec)
        if(!isNot[i] && sc.find(out[i]) != sc.end())
          CheckDef15(clauses[i], args[i], sc, symbol_table, errors);
      }
    }
  }

  return errors;
}

list<ErrorType> RawDGraph::CheckRecursiveDependencies()
{
  list<ErrorType> errors;

  map<size_t, DGraphNode*>::const_iterator it = graph.find(SymbolTable::DoesID);

  const DGraphNode* does = NULL;
  const DGraphNode* ttrue = NULL;

  const DGraphNode* base = NULL;
  const DGraphNode* init = NULL;
  const DGraphNode* input = NULL;
  const DGraphNode* legal = NULL;

  if((it = graph.find(SymbolTable::BaseID)) != graph.end()) base = it->second;
  if((it = graph.find(SymbolTable::InitID)) != graph.end()) init = it->second;
  if((it = graph.find(SymbolTable::InputID)) != graph.end()) input = it->second;
  if((it = graph.find(SymbolTable::LegalID)) != graph.end()) legal = it->second;

  if((it = graph.find(SymbolTable::DoesID)) != graph.end())
  {
    does = it->second;

    // perform DFS from does to detect legal, init, input, base
    // DGraphNode stack (used for DFS)
    std::stack<const DGraphNode*> st;
    std::set<const DGraphNode*> n_set;

    st.push(does);
    n_set.insert(does);

    while(!st.empty())
    {
      const DGraphNode* temp = st.top();
      st.pop();

      // if destination is found return true
      if(temp == base)
      {
        INVALID_DEP_ERROR(error, "'base'", "'does'");
        errors.push_back(error);
      }
      else if(temp == init)
      {
        INVALID_DEP_ERROR(error, "'init'", "'does'");
        errors.push_back(error);
      }
      else if(temp == legal)
      {
        INVALID_DEP_ERROR(error, "'legal'", "'does'");
        errors.push_back(error);
      }
      else if(temp == input)
      {
        INVALID_DEP_ERROR(error, "'input'", "'does'");
        errors.push_back(error);
      }

      // push all the children in the stack
      for(size_t i = 0; i < (temp->out).size(); i++)
      {
        if(n_set.find((temp->out)[i]) == n_set.end())
        {
          st.push((temp->out)[i]);
          n_set.insert((temp->out)[i]);
        }
      }
    }
  }

  if((it = graph.find(SymbolTable::TrueID)) != graph.end())
  {
    ttrue = it->second;

    // perform DFS from does to detect legal, init, input, base
    // DGraphNode stack (used for DFS)
    std::stack<const DGraphNode*> st;
    std::set<const DGraphNode*> n_set;

    st.push(ttrue);
    n_set.insert(ttrue);

    while(!st.empty())
    {
      const DGraphNode* temp = st.top();
      st.pop();

      // if destination is found return true
      if(temp == base)
      {
        INVALID_DEP_ERROR(error, "'base'", "'true'");
        errors.push_back(error);
      }

      else if(temp == init)
      {
        INVALID_DEP_ERROR(error, "'init'", "'true'");
        errors.push_back(error);
      }
      else if(temp == input)
      {
        INVALID_DEP_ERROR(error, "'input'", "'true'");
        errors.push_back(error);
      }

      // push all the children in the stack
      for(size_t i = 0; i < (temp->out).size(); i++)
      {
        if(n_set.find((temp->out)[i]) == n_set.end())
        {
          st.push((temp->out)[i]);
          n_set.insert((temp->out)[i]);
        }
      }
    }
  }

  return errors;
}

void RawDGraph::CheckDef15(const Clause* clause,
                            const Argument* arg,
                            const set<DGraphNode*>& scc,
                            const SymbolTable& symbol_table,
                            list<ErrorType>& errors)
{
  const Clause& c = *clause;

  const vector<Argument*>& premisses = c.premisses;

  // find the index of the given argument in the clause
  size_t arg_index = 0;
  for(size_t i = 0;i < premisses.size();i++)
    if(premisses[i]->OrEquate(*arg))
    {
      arg_index = i;
      break;
    }

  // check for each argument in given argument
  bool isValid = true;
  size_t invalid_index = 0;
  for(size_t i = 0;i < arg->args.size();i++)
  {
    // check if the argument is ground
    if(arg->args[i]->IsGround()) continue;
    // check if this argument is also argument to head
    if(c.head->HasAsArgument(*(arg->args[i]))) continue;

    bool isFound = false;

    for(size_t j = 0;j < premisses.size();j++)
    {
      // avoid checking in itself
      if(j == arg_index) continue;

      // if another premiss has same same argument and is not in the same SCC
      if(premisses[j]->HasAsArgument(*(arg->args[i])) &&
         scc.find(graph[premisses[j]->value]) == scc.end())
      {
        isFound = true;
        break;
      }
    }

    if(!isFound)
    {
      isValid = false;
      invalid_index = i;
      break;
    }
  }

  if(!isValid)
  {
    std::stringstream s;
    SymbolDecodeStream stream(&symbol_table, s);
    stream << *arg->args[invalid_index];
    ErrorType error;
    error.AddEntry("Recursion restriction violated for argument " + s.str(),
                   c.loc);
    error.AddEntry("Relation involved in the cycle is '" +
                   symbol_table.GetCommandName(arg->value) + "'", c.loc);
    errors.push_back(error);
  }
}

std::string RawDGraph::DecodeToString(const SymbolTable& symbol_table) const
{
  stringstream s;
  s << "digraph dependency_graph {" << endl;

  for(map<size_t, DGraphNode*>::const_iterator it = graph.begin();
                                                    it != graph.end(); it++)
  {
    s << symbol_table.GetCommandName(it->first) << " [label = \""
      << symbol_table.GetCommandName(it->first) << "\"];" << endl;
  }

  for(map<size_t, DGraphNode*>::const_iterator it = graph.begin();
                                                  it != graph.end(); it++)
  {
    const vector<DGraphNode*>& out = (it->second)->out;
    const vector<bool>& isNot = (it->second)->isNot;

    set<size_t> str_set;
    set<size_t>::iterator sit;
    for(size_t i = 0; i < out.size(); i++)
    {
      if((sit = str_set.find(out[i]->id)) == str_set.end())
      {
        if(isNot[i] == true)
          s << symbol_table.GetCommandName(it->first) << " -> "
            << symbol_table.GetCommandName(out[i]->id)
            << " [color = red];" << endl;
        else
          s << symbol_table.GetCommandName(it->first) << " -> "
            << symbol_table.GetCommandName(out[i]->id)
            << ";" << endl;
        str_set.insert(out[i]->id);
      }
    }
  }

  s << "}";
  return s.str();
}
