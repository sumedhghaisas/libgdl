/**
 * @file gdl_benchmarks.cpp
 * @author Sumedh Ghaisas
 *
 * Benchmark file for GDL module.
 */
#include "libgdl_benchmark.hpp"

#include <stdlib.h>

#include <libgdl/core.hpp>
#include <libgdl/gdl.hpp>

LIBGDL_AUTO_BENCHMARK_SUITE(GDLBenchmarks);

using namespace std;
using namespace libgdl;

/**
 * Benchmark next state computation.
 */
BENCHMARK_T(GDLGetNextStateBenchmark,
            Benchmark function 'GetNextState' of GDL module.,
            2,
            50)
{
  OPEN_LOG;
  
  GDL<> gdl("data/games/nine_board_tictactoe.kif", 1024, BENCHMARK_LOG);
  State s = gdl.InitState();
  
  SetUp
  (
    MoveList ml = gdl.GetLegalMoves(s);
    const Move& m = *ml.begin();
  )
  
  Measure
  (
    s = gdl.GetNextState(s, m);
  )
  
  TearDown()
  
  DEINITIALIZE;
}

/**
 * Benchmark next state cache hit time.
 */
BENCHMARK_T(GDLGetNextStateCacheBenchmark,
            Benchmarks 'GetNextState' cache hit.,
            50,
            1)
{
  OPEN_LOG;
  
  GDL<> gdl("data/games/nine_board_tictactoe.kif", 1024, BENCHMARK_LOG);
  State s = gdl.InitState();
  MoveList ml = gdl.GetLegalMoves(s);
  Move m = *ml.begin();
  gdl.GetNextState(s, m);
  
  SetUp()
  
  Measure
  (
    gdl.GetNextState(s, m);
  )
  
  TearDown()
  
  DEINITIALIZE;
}

/**
 * Benchmark is terminal computation.
 */
BENCHMARK_T(GDLIsTerminalBenchmark,
            Benchmarks 'IsTerminal' function of GDL.,
            50,
            50)
{
  OPEN_LOG;
  
  GDL<> gdl("data/games/nine_board_tictactoe.kif", 1024, BENCHMARK_LOG);
  State s = gdl.InitState();
  
  SetUp()
  
  Measure
  (
    gdl.IsTerminal(s);
  )
  
  TearDown
  (
    MoveList ml = gdl.GetLegalMoves(s);
    Move m = *ml.begin();
    s = gdl.GetNextState(s, m);
  )
  
  DEINITIALIZE;
}

/**
 * Benchmark is terminal cache hit time.
 */
BENCHMARK_T(GDLIsTerminalCacheBenchmark,
            Benchmarks 'IsTerminal' cache hit.,
            50,
            1)
{
  OPEN_LOG;
  
  GDL<> gdl("data/games/nine_board_tictactoe.kif", 1024, BENCHMARK_LOG);
  State s = gdl.InitState();
  gdl.IsTerminal(s);
  
  SetUp()
  
  Measure
  (
    gdl.IsTerminal(s);
  )
  
  TearDown()
  
  DEINITIALIZE;
}

/**
 * Benchmark GetLegalMoves computation.
 */
BENCHMARK_T(GDLGetLegalMovesBenchmark,
            Benchmarks 'GetLegalMoves' function of GDL.,
            20,
            50)
{
  OPEN_LOG;
  
  GDL<> gdl("data/games/nine_board_tictactoe.kif", 1024, BENCHMARK_LOG);
  State s = gdl.InitState();
  
  SetUp()
  
  Measure
  (
    MoveList ml = gdl.GetLegalMoves(s);
  )
  
  TearDown
  (
    Move m = *ml.begin();
    s = gdl.GetNextState(s, m);
  )
  
  DEINITIALIZE;
}

/**
 * Benchmark GetLegalMoves cache hit.
 */
BENCHMARK_T(GDLGetLegalMovesCacheBenchmark,
            Benchmarks 'GetLegalMoves' cache hit.,
            50,
            1)
{
  OPEN_LOG;
  
  GDL<> gdl("data/games/nine_board_tictactoe.kif", 1024, BENCHMARK_LOG);
  State s = gdl.InitState();
  gdl.GetLegalMoves(s);
  
  SetUp()
  
  Measure
  (
    gdl.GetLegalMoves(s);
  )
  
  TearDown()
  
  DEINITIALIZE;
}

/**
 * Benchmark GetGoal computation.
 */
BENCHMARK_T(GDLGetGoalBenchmark,
            Benchmarks 'GetGoal' function of GDL.,
            20,
            50)
{
  OPEN_LOG;
  
  GDL<> gdl("data/games/nine_board_tictactoe.kif", 1024, BENCHMARK_LOG);
  State s = gdl.InitState();
  
  SetUp()
  
  Measure
  (
    gdl.GetGoal(s, 0);
  )
  
  TearDown
  (
    MoveList ml = gdl.GetLegalMoves(s);
    Move m = *ml.begin();
    s = gdl.GetNextState(s, m);
  )
  
  DEINITIALIZE;
}

/**
 * Benchmark GetGoal cache hit.
 */
BENCHMARK_T(GDLGetGoalCacheBenchmark,
            Benchmarks 'GetGoal' cache hit.,
            20,
            50)
{
  OPEN_LOG;
  
  GDL<> gdl("data/games/nine_board_tictactoe.kif", 1024, BENCHMARK_LOG);
  State s = gdl.InitState();
  gdl.GetGoal(s, 0);
  
  SetUp()
  
  Measure
  (
    gdl.GetGoal(s, 0);
  )
  
  TearDown()
  
  DEINITIALIZE;
}
