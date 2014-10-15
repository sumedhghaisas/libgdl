/**
 * @file gdlreasoner_benchmarks.cpp
 * @author Sumedh Ghaisas
 *
 * Benchmark file for GDLReasoner module.
 */
#include "libgdl_benchmark.hpp"

#include <string>
#include <sstream>
#include <list>

#include <libgdl/core.hpp>
#include <libgdl/reasoners/gdlreasoner/knowledgebase.hpp>
#include <libgdl/gdlparser/kif.hpp>

LIBGDL_AUTO_BENCHMARK_SUITE(GDLReasonerBenchmarks);

using namespace std;
using namespace libgdl;
using namespace libgdl::core;
using namespace libgdl::gdlreasoner;
using namespace libgdl::gdlparser;

/**
 * Benchmark the knowledge addition time.
 */
BENCHMARK_T(GDLReasonerTellBenchmark,
            Benchmarks the 'Tell' function of GDLReasoner.,
            50,
            1)
{
  OPEN_LOG;
  
  SetUp
  (
    KnowledgeBase kb(BENCHMARK_LOG);
    Clause c("(<= (test ?x ?y) (test2 ?x ?y))",
             kb.GetSymbolTable(),
             BENCHMARK_LOG); 
  )
  
  Measure
  (
    kb.Tell(c);
  )
  
  TearDown()
 
  DEINITIALIZE;
}


/**
 * Benchmark the query which involves many parallel unifications.
 */
BENCHMARK_T(GDLReasonerParallelQueryBenchmark,
            Benchmarks parallel unification capacity of knowledgebase.,
            50,
            10)
{
  OPEN_LOG;
  KnowledgeBase kb(BENCHMARK_LOG);
  for(size_t i = 0;i < 100;i++)
  {
    stringstream stream;
    stream << "(successor " << i << " " << i + 1 << ")";
    kb.Tell(stream.str());
  }
  Argument question("(successor ?x ?y)", 
                    kb.GetSymbolTable(), 
                    true, 
                    BENCHMARK_LOG);
  
  SetUp
  (
    list<Argument*> result;
  )
  
  Measure
  (
    result = kb.Ask(question, true);
  )
  
  TearDown
  (
    for(list<Argument*>::iterator it = result.begin();it != result.end();it++)
    {
      delete *it;
    }
  )
  
  DEINITIALIZE;
}

/**
 * Benchmark normal query.
 */
BENCHMARK_T(GDLReasonerNormalQueryBenchmark,
            Benchmarks average query satisfaction time.,
            50,
            10)
{
  OPEN_LOG;
  KIF kif(BENCHMARK_LOG);
  kif.AddFile("data/games/nine_board_tictactoe.kif");
  kif.Parse();
  
  KnowledgeBase kb(kif, BENCHMARK_LOG);
  Argument question("(base ?x)", 
                    kb.GetSymbolTable(), 
                    true,
                    BENCHMARK_LOG);
  SetUp
  (
    list<Argument*> result;
  )
  
  Measure
  (
    result = kb.Ask(question, true);
  )
  
  TearDown
  (
    for(list<Argument*>::iterator it = result.begin();it != result.end();it++)
      delete *it;
  )
  
  DEINITIALIZE;
}

/**
 * Benchmark recursive query.
 */
BENCHMARK_T(GDLReasonerRecursiveQueryBenchmark,
            Benchmarks recursive query satisfaction time.,
            50,
            10)
{
  OPEN_LOG;
  KnowledgeBase kb(BENCHMARK_LOG);
  for(size_t i = 0;i < 14;i++)
  {
    stringstream stream;
    stream << "(parent " << i << " " << i + 1 << ")";
    kb.Tell(stream.str());
  }
  kb.Tell("(<= (ancestor ?x ?y) (parent ?x ?y))");
  kb.Tell("(<= (ancestor ?x ?z) (parent ?x ?y) (ancestor ?y ?z))");
  Argument question("(ancestor ?x ?y)",
                    kb.GetSymbolTable(),
                    true,
                    BENCHMARK_LOG);
  SetUp
  (
    list<Argument*> result;
  )
  
  Measure
  (
    result = kb.Ask(question, true);
  )
  
  TearDown
  (
    for(list<Argument*>::iterator it = result.begin();it != result.end();it++)
      delete *it;
  )
  
  DEINITIALIZE;
}
