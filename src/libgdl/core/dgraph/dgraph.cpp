/**
 * @file dgraph.cpp
 * @author Sumedh Ghaisas
 *
 * Declaration DGraph class.
 */
#include "dgraph.hpp"

#include <sstream>

using namespace std;
using namespace libgdl;

DGraph::~DGraph()
{
  for(map<size_t, DGraphNode*>::iterator it = graph.begin();it != graph.end();
                                                                          it++)
  {
    delete it->second;
  }
}

bool DGraph::AddNode(size_t id)
{
  map<size_t, DGraphNode*>::const_iterator it = graph.find(id);

  if(it != graph.end()) return false;
  graph[id] = new DGraphNode(id);
  return true;
}

bool DGraph::AddDependency(size_t h,
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

std::string DGraph::DecodeToString(const SymbolTable& symbol_table) const
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
