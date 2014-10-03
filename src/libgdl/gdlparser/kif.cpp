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

#include <libgdl/core/symbol_table/symbol_decode_stream.hpp>
#include <libgdl/core/util/prefixedoutstream.hpp>

using namespace std;
using namespace libgdl;
using namespace libgdl::gdlparser;
using namespace libgdl::gdlparser::parser;

KIF::KIF(bool isWarn,
         char o_level,
         const Log& log)
  : isWarn(isWarn),
    o_level(o_level),
    driver(*this),
    log(log)
{}

bool KIF::AddFile(const std::string& filename)
{
  std::ifstream* temp = new std::ifstream(filename);
  // if invalid file
  if(!temp->is_open()) return false;
  streams.push_back(util::GDLStream(filename, temp));
  return true;
}

bool KIF::Parse(bool ignoreErrors)
{
  isRole = false;
  isLegal = false;
  isGoal = false;
  isTerminal = false;

  errors.clear();
  warnings.clear();

  symbol_table = SymbolTable();
  dgraph = DGraph();

  bool res = driver.Parse();

  if(res)
  {
    list<ErrorType> e = dgraph.CheckCycles(symbol_table);
    for(list<ErrorType>::iterator it = e.begin();it != e.end();it++)
      errors.push_back(*it);
    e = dgraph->CheckRecursiveDependencies();
    for(list<ErrorType>::iterator it = e.begin();it != e.end();it++)
      errors.push_back(*it);

    if(!isRole)
    {
      SIMPLE_ERROR(error,"No role defined.");
      errors.push_back(error);
    }
    if(!isLegal)
    {
      SIMPLE_ERROR(error,"No legal rules defined.");
      errors.push_back(error);
    }
    if(!isGoal)
    {
      SIMPLE_ERROR(error,"No goal rules defined.");
      errors.push_back(error);
    }
    if(!isTerminal)
    {
      SIMPLE_ERROR(error,"No terminal rules defined.");
      errors.push_back(error);
    }
  }

  for(list<ErrorType>::const_iterator it = errors.begin();it != errors.end();it++)
    log.Fatal << *it << std::endl;

  if(isWarn)
  {
    for(list<ErrorType>::const_iterator it = warnings.begin();it != warnings.end();
        it++)
      log.Warn << *it << std::endl;
  }

  if((!res || errors.size() != 0) && !ignoreErrors )
  {
    facts.clear();
    clauses.clear();
    return false;
  }

  return true;
}

KIF::~KIF()
{
}

bool KIF::PrintDependencyGraph(const string& filename) const
{
  ofstream graph(filename.c_str());
  if(!graph.is_open())
  {
    log.Fatal << LOGID << "Unable to open file "
                              << filename << " for saving graph" << endl;
    return false;
  }

  core::SymbolDecodeStream stream(&symbol_table, util::PrefixedOutStream(graph));
  stream << dgraph << endl;

  graph.close();

  return true;
}

bool KIF::PrintToFile(const string& filename, bool isDebuggingSymbols) const
{
  ofstream out_p(filename.c_str());
  if(!out_p.is_open())
  {
    log.Fatal << LOGID << "Unable to open file " << filename << std::endl;
    return false;
  }

  core::SymbolDecodeStream out(&symbol_table, out_p);

  for(list<Fact>::const_iterator it = facts.begin();it != facts.end();it++)
  {
    if(isDebuggingSymbols) out << ";#line " << it->loc << std::endl;
    out << *it << std::endl;
  }
  for(list<Clause>::const_iterator it = clauses.begin();it != clauses.end();it++)
  {
    if(isDebuggingSymbols) out << ";#line " << it->loc  << std::endl;
    out << *it << std::endl;
  }

  out_p.close();
  return true;
}
