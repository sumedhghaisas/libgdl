/**
 * @file libgdl_benchmark.hpp
 * @author Sumedh Ghaisas
 *
 * Declaring the Benchmaking framework for libGDL.
 */
#include <libgdl/core.hpp>

#define OPEN_LOG std::ofstream benchmark_log("benchmark_log.log", std::fstream::app); \
                 Log log(benchmark_log);
                 
#define BENCHMARK_LOG benchmark_log

#define BENCHMARK_REDIRECTION_STREAM benchmark_log.rdbuf()

#include <libgdl/core/benchmark/benchmark.hpp>
