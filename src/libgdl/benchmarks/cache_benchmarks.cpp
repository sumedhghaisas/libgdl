/**
 * @file cache_benchamrks.hpp
 * @author Sumedh Ghaisas
 *
 * Benchmarking for Cache module.
 */
#include "libgdl_benchmark.hpp"

#include <libgdl/core.hpp>
#include <libgdl/core/cache/lru_cache.hpp>

LIBGDL_AUTO_BENCHMARK_SUITE(LRUCacheBenchmarks);

using namespace std;
using namespace libgdl;
using namespace libgdl::cache;

size_t* Fun(const size_t& a)
{
  size_t* out = new size_t;
  for(size_t i = 0;i < 10000;i++)
  {
    *out = 2 * a;
  }
  return out;
}

size_t GetHash(const size_t& a)
{
  return a;
}

/**
 * Benchmark LRUCache hit time
 */
BENCHMARK_T(LRUCacheHitTimeBenchmark, 
            This benchamrks cache hit time of LRUCache implemented in libGDL,
            10,
            1)
{
  OPEN_LOG;
  boost::function<size_t* (const size_t&)> def(Fun);
  boost::function<size_t (const size_t&)> h(GetHash);
  
  LRUCache<size_t, size_t> cache(4);
  cache.GetLog() = BENCHMARK_LOG;
  size_t* t1 = cache.Get(10, def, h);
  
  SetUp()
  
  Measure
  (
    t1 = cache.Get(10, def, h);
  )
  
  TearDown()
  
  MARK_USED(t1);
  
  DEINITIALIZE;
}
