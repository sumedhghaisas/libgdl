/**
 * @file kif.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation of KIF class.
 */
#include "kif.hpp"

// Color code escape sequences
#define BASH_RED "\033[0;31m"
#define BASH_YELLOW "\033[0;33m"
#define BASH_CLEAR "\033[0m"

#include <list>

using namespace std;
using namespace libgdl;
using namespace libgdl::gdlparser;

KIF::KIF(bool isWarn,
         bool isDebuggingSymbols,
         char o_level,
         ostream* stream)
  : stream(stream), isWarn(isWarn),
    isDebuggingSymbols(isDebuggingSymbols),
    o_level(o_level),
    driver(*this)
{
  f_last_index_with_linemark = 0;
  c_last_index_with_linemark = 0;
}

bool KIF::Parse(bool ignoreErrors)
{
  bool res = driver.Parse();
  if(!res && !ignoreErrors)
  {
    facts.clear();
    clauses.clear();
    return false;
  }
  return true;
}

KIF::~KIF()
{
  for(map<std::string, DGraphNode*>::iterator it = dgraph.begin();
                                                      it != dgraph.end();it++)
      delete it->second;
}

bool KIF::PrintDependencyGraph(const string& filename) const
{
  ofstream graph(filename.c_str());
  if(!graph.is_open())
  {
    *stream << BASH_RED "[ERROR] " BASH_CLEAR;
    cout << "Unable to open file " << filename << " for saving graph" << endl;
    return false;
  }

  graph << "digraph dependency_graph {" << endl;

  for(map<string, DGraphNode*>::const_iterator it = dgraph.begin();
                                                    it != dgraph.end(); it++)
  {
    graph << it->first << " [label = \"" << it->first << "\"];" << endl;
  }

  for(map<string, DGraphNode*>::const_iterator it = dgraph.begin();
                                                  it != dgraph.end(); it++)
  {
    const vector<DGraphNode*>& out = (it->second)->out;
    const vector<bool>& isNot = (it->second)->isNot;

    set<string> str_set;
    set<string>::iterator sit;
    for(size_t i = 0; i < out.size(); i++)
    {
      if((sit = str_set.find(out[i]->name)) == str_set.end())
      {
        if(isNot[i] == true) graph << it->first << " -> " << out[i]->name <<
                                                      " [color = red];" << endl;
        else graph << it->first << " -> " << out[i]->name << ";" << endl;
        str_set.insert(out[i]->name);
      }
    }
  }

  graph << "}";

  graph.close();

  return true;
}

bool KIF::PrintToFile(const string& filename) const
{
  ofstream out(filename.c_str());
  if(!out.is_open())
  {
    *stream << BASH_RED "[ERROR] " BASH_CLEAR;
    cout << "Unable to open file " << filename << std::endl;
    return false;
  }

  for(size_t i = 0; i < facts.size(); i++)
  {
    if(isDebuggingSymbols) out << ";#line " << facts[i].loc << std::endl;
    out << facts[i] << std::endl;
  }
  for(size_t i = 0; i < clauses.size(); i++)
  {
    if(isDebuggingSymbols) out << ";#line " << clauses[i].loc  << std::endl;
    out << clauses[i] << std::endl;
  }

  out.close();
  return true;
}

void KIF::AddLineMark(const location_type& loc)
{
  size_t f_size = facts.size();
  size_t c_size = clauses.size();

  for(size_t i = f_last_index_with_linemark;i < f_size;i++)
  {
    facts[i].AddLocation(loc);
  }

  for(size_t i = c_last_index_with_linemark;i < c_size;i++)
  {
    clauses[i].AddLocation(loc);
  }

  f_last_index_with_linemark = f_size;
  c_last_index_with_linemark = c_size;
}

const Fact& KIF::AddFact(const Fact& f, const location_type& loc)
{
  facts.push_back(f);
  facts[facts.size() - 1].AddLocation(loc);

  return facts.back();
}

const Fact& KIF::AddFact(Fact&& f, const location_type& loc)
{
  facts.push_back(std::move(f));
  facts[facts.size() - 1].AddLocation(loc);

  return facts.back();
}

const Clause& KIF::AddClause(const Clause& c, const location_type& loc)
{
  clauses.push_back(c);
  clauses[clauses.size() - 1].AddLocation(loc);

  return clauses.back();
}

const Clause& KIF::AddClause(Clause&& c, const location_type& loc)
{
  clauses.push_back(std::move(c));
  clauses[clauses.size() - 1].AddLocation(loc);

  return clauses.back();
}
