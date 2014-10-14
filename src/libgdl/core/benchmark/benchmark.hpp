/**
 * @file benchmark.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of important Preprocessor macros associated with benchmarking.
 */
#ifndef _LIBGDL_BENCHMARK_BENCHMARK_HPP_INCLUDED
#define _LIBGDL_BENCHMARK_BENCHMARK_HPP_INCLUDED

//! While the benchmark is running all the output on std::cout and std::cerr
//! is redirected to another stream
//! If that stream is not defined then define it ourselves
#ifndef BENCHMARK_REDIRECTION_STREAM

//! Open the logging file stream
#define OPEN_LOG std::ofstream f("benchmark_log.txt", std::fstream::app);
#define BENCHMARK_REDIRECTION_STREAM f.rdbuf()

#endif

#include <string>
#include <limits>
#include <fstream>
#include <iostream>

#include <libgdl/core/util/timer.hpp>

#include "base_test.hpp"
#include "pmeasure.hpp"
#include "handler.hpp"

//! Define default tolerance for any benchmarking
#define DEFAULT_TOLERANCE 10


//! This line declares the module name of the following benchmarks
//! Every following benchmarking will marked with the same module name
#define LIBGDL_AUTO_BENCHMARK_SUITE(Name) \
const std::string CMN = #Name


//! The code given in SetUp will run at startup of each iteration
#define SetUp(Code) \
double total = 0; \
double min = std::numeric_limits<double>::max(); \
double max = std::numeric_limits<double>::min(); \
for(size_t i = 0;i < iterations;i++) \
{ \
  Code


//! The code to be benchmarked
#define Measure(Code) \
OPEN_LOG; \
std::streambuf *coutbuf = std::cout.rdbuf(); \
std::streambuf *cerrbuf = std::cout.rdbuf(); \
std::cout.rdbuf(BENCHMARK_REDIRECTION_STREAM); \
std::cerr.rdbuf(BENCHMARK_REDIRECTION_STREAM); \
\
uint64_t start = libgdl::Timer::microtimer(); \
Code \
double time = libgdl::Timer::microtimer() - start; \
\
total += time; \
std::cout << time << std::endl; \
if(time < min) min = time; \
if(time > max) max = time; \
std::cout << min << " " << max << std::endl; \
\
std::cout.rdbuf(coutbuf); \
std::cerr.rdbuf(cerrbuf);


//! The code given in TearDown will run before starting a new iteration
#define TearDown(Code) \
Code \
}

//! This will DEINITIALIZE the benchmark
//! This line has to be added at the end of each benchmark
#define DEINITIALIZE \
return libgdl::benchmark::PMeasure(min, max, total / iterations)

//! Add a new benchmark with given tolerance
//!
//! \param Name Name of the benchamrk
//! \param Desc Description of the benchmark
//! \param Iter Number of iterations to perform
//! \param DTOL Tolerance of this benchmark
//!
//!
#define BENCHMARK_T(Name, Desc, Iter, DTOL) \
class Name ## _ ## Benchmark \
  : public libgdl::benchmark::BaseTest \
{ \
 public: \
  Name ## _ ## Benchmark() \
    : BaseTest(CMN, #Name, #Desc, Iter, DTOL) \
  {} \
 private: \
  static size_t* static_temp; \
\
  libgdl::benchmark::PMeasure Run(); \
}; \
size_t* Name ## _ ## Benchmark::static_temp = \
libgdl::benchmark::Handler::Instance().RegisterTest(new Name ## _ ## Benchmark()); \
libgdl::benchmark::PMeasure Name ## _ ## Benchmark::Run() \


//! Add a new benchmark with default tolerance
//!
//! \param Name Name of the benchamrk
//! \param Desc Description of the benchmark
//! \param Iter Number of iterations to perform
//!
//!
#define BENCHMARK(Name, Desc, Iter) BENCHMARK_T(Name, Desc, Iter, DEFAULT_TOLERANCE)

#endif // _LIBGDL_BENCHMARK_BENCHMARK_HPP_INCLUDED
