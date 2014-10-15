/**
 * @file flattener_benchmarks.cpp
 * @author Sumedh Ghaisas
 *
 * Benchmark file for KIFFLattener module.
 */
#include "libgdl_benchmark.hpp"

#include <libgdl/core.hpp>
#include <libgdl/reasoners/gdlreasoner/kif_flattener.hpp>
#include <libgdl/gdlparser/kif.hpp>

LIBGDL_AUTO_BENCHMARK_SUITE(KIFFlattenerBenchmarks);

using namespace std;
using namespace libgdl;
using namespace libgdl::core;
using namespace libgdl::gdlreasoner;
using namespace libgdl::gdlparser;

/**
 * Benchmark the time taken for flattening.
 */
BENCHMARK_T(KIFFlattenerFlattenTimeBenchmark,
            Benchmarks the time taken for flattening.,
            50,
            500)
{
  OPEN_LOG;
  
  KIFFlattener kf(BENCHMARK_LOG);
  
  SetUp
  (
    KIF kif(BENCHMARK_LOG);
    kif.AddFile("data/games/nine_board_tictactoe.kif");
    kif.Parse();
  )
  
  Measure
  (
    kf.Flatten(kif);
  )
  
  TearDown()
  
  DEINITIALIZE;
}

