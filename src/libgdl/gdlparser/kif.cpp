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
         bool isDebuggingSymbols,
         char o_level,
         const Log& log)
  : log(log), isWarn(isWarn),
    isDebuggingSymbols(isDebuggingSymbols),
    o_level(o_level),
    driver(*this),
    symbol_table(new SymbolTable()),
    dgraph(new DGraph())
{
  f_last_index_with_linemark = 0;
  c_last_index_with_linemark = 0;

  id_index = 0;
  id_map = new boost::unordered_map<string, size_t>();
}

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
  errors.clear();
  bool res = driver.Parse();

  if(res)
  {
    list<ErrorType> e = dgraph->CheckCyclesWithNegation(*symbol_table);
    for(list<ErrorType>::iterator it = e.begin();it != e.end();it++)
      errors.push_back(*it);
    e = dgraph->CheckRecursiveDependencies();
    for(list<ErrorType>::iterator it = e.begin();it != e.end();it++)
      errors.push_back(*it);
  }

  for(list<ErrorType>::const_iterator it = errors.begin();it != errors.end();it++)
    log.Fatal << *it << std::endl;

  if(isWarn)
  {
    for(list<ErrorType>::const_iterator it = warnings.begin();it != warnings.end();
        it++)
      log.Warn << *it << std::endl;
  }

  if(!res && !ignoreErrors)
  {
    facts.clear();
    clauses.clear();
    return false;
  }

  SymbolDecodeStream stream(symbol_table);

  for(size_t i = 0;i < facts.size();i++)
    stream << facts[i] << endl;

  for(size_t i = 0;i < clauses.size();i++)
    stream << clauses[i] << endl;

  return true;
}

KIF::~KIF()
{
  delete id_map;

  delete symbol_table;
  delete dgraph;
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

  SymbolDecodeStream stream(symbol_table, util::PrefixedOutStream(graph));
  stream << *dgraph << endl;

  graph.close();

  return true;
}

bool KIF::PrintToFile(const string& filename) const
{
  ofstream out(filename.c_str());
  if(!out.is_open())
  {
    log.Fatal << LOGID << "Unable to open file " << filename << std::endl;
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
