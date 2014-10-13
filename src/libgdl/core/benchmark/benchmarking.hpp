#ifndef _LIBGDL_BENCHMARK_BENCHMARKING_HPP_INCLUDED
#define _LIBGDL_BENCHMARK_BENCHMARKING_HPP_INCLUDED

#include <string>
#include <limits>

#include <libgdl/core/util/timer.hpp>

#include "base_test.hpp"
#include "pmeasure.hpp"
#include "handler.hpp"

#define LIBGDL_AUTO_BENCHMARK_SUITE(Name) \
const std::string CMN = #Name

#define SetUp(Code) \
double total = 0; \
double min = std::numeric_limits<double>::max(); \
double max = std::numeric_limits<double>::min(); \
for(size_t i = 0;i < iterations;i++) \
{ \
  Code

#define Measure(Code) \
uint64_t start = libgdl::Timer::microtimer(); \
Code \
double time = libgdl::Timer::microtimer() - start; \
total += time; \
if(time < min) min = time; \
if(time > max) max = time; \

#define TearDown(Code) \
Code \
}

#define DEINITIALIZE \
return libgdl::benchmark::PMeasure(min, max, total / iterations)

#define BENCHMARK(Name, Desc, Iter) \
class Name ## _ ## Benchmark \
  : public libgdl::benchmark::BaseTest \
{ \
 public: \
  Name ## _ ## Benchmark() \
    : BaseTest(CMN, #Name, #Desc, Iter) \
  {} \
 private: \
  static size_t* static_temp; \
\
  libgdl::benchmark::PMeasure Run(); \
}; \
size_t* Name ## _ ## Benchmark::static_temp = \
libgdl::benchmark::Handler::Instance().RegisterTest(new Name ## _ ## Benchmark()); \
libgdl::benchmark::PMeasure Name ## _ ## Benchmark::Run() \

#endif // _LIBGDL_BENCHMARK_BENCHMARKING_HPP_INCLUDED
