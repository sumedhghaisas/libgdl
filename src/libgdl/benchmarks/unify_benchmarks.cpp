/**
 * @file unify_benchmarks.cpp
 * @author Sumedh Ghaisas
 *
 * Benchmark file for Unify module.
 */
#include "libgdl_benchmark.hpp"

#include <libgdl/core.hpp>
#include <libgdl/reasoners/gdlreasoner/logicbase/unify.hpp>

LIBGDL_AUTO_BENCHMARK_SUITE(UnifyBenchmarks);

using namespace std;
using namespace libgdl;
using namespace libgdl::core;
using namespace libgdl::gdlreasoner::logicbase;

/**
 * Benchmark unification time.
 */
BENCHMARK_T(UnifyTimeBenchmark,
            Benchmarks unification time.,
            50,
            5)
{
  OPEN_LOG;
  SymbolTable sym;
  Argument arg1("(test ?a ?b ?c (test2 ?x ?y))", sym, true, BENCHMARK_LOG);
  Argument arg2("(test x y c ?x)", sym, true, BENCHMARK_LOG);
  
  SetUp
  (
    VariableMap v_map;
  )
  
  Measure
  (
    Unify::mgu(arg1, arg2, v_map);
  )
  
  TearDown()
  
  DEINITIALIZE;
}
