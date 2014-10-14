/**
 * @file gdlparser_benchmarks.cpp
 * @author Sumedh Ghaisas
 *
 * Benchmark file for KIF module.
 */
#include "libgdl_benchmark.hpp"

LIBGDL_AUTO_BENCHMARK_SUITE(GDLParserBenchmarks);

#include <libgdl/core.hpp>
#include <libgdl/gdlparser/kif.hpp>

using namespace std;
using namespace libgdl::gdlparser;
using namespace libgdl::benchmark;

/**
 * Test parsing time of KIF module.
 */
BENCHMARK_T(KIFParserBenchmark,
            This benchmarks KIF parsing time.,
            10,
            50)
{
  OPEN_LOG;
  KIF kif(false, 0, BENCHMARK_LOG);
  kif.AddFile("data/games/nine_board_tictactoe.kif");
  
  SetUp()
  
  Measure
  (
    kif.Parse();
  )
  
  TearDown()
  
  DEINITIALIZE;
}
