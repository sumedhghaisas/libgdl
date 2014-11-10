/**
 * @file cache_tests.cpp
 * @author Sumedh Ghaisas
 *
 * Test file of LRUCache
 */
#include <libgdl/core.hpp>
#include <libgdl/core/cache/lru_cache.hpp>

#include <boost/test/unit_test.hpp>
#include "old_boost_test_definitions.hpp"
#include "libgdl_test.hpp"

#include <list>
#include <functional>

BOOST_AUTO_TEST_SUITE(CacheTests);

using namespace std;
using namespace libgdl;
using namespace libgdl::util;
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
 * Check LRUCache for cache hit.
 */
BOOST_AUTO_TEST_CASE(LRUCacheHitTest_Time)
{
  MARK_START;
  OPEN_LOG;
  
  function<size_t* (const size_t&)> def(Fun);
  function<size_t (const size_t&)> h(GetHash);
  
  LRUCache<size_t, size_t> cache(def, h, 4);
  cache.GetLog() = TEST_LOG;
  
  size_t* t1 = cache.Get(10);
 
  size_t start = Timer::microtimer();
  t1 = cache.Get(10);
  size_t end = Timer::microtimer();
  if(end - start > 3) MARK_FAIL;
  (void)t1;
  MARK_END;
}

/**
 * Check LRUCache for cache least recently used replacement policy.
 */
BOOST_AUTO_TEST_CASE(LRUCacheReplacementPolicyTest_Time)
{
  MARK_START;
  OPEN_LOG;

  function<size_t* (const size_t&)> def(Fun);
  function<size_t (const size_t&)> h(GetHash);
  
  LRUCache<size_t, size_t> cache(def, h, 3);
  cache.GetLog() = TEST_LOG;
  
  size_t* t1 = cache.Get(10);
  t1 = cache.Get(11);
  t1 = cache.Get(12);
  t1 = cache.Get(10);
  t1 = cache.Get(13);

  size_t start = Timer::microtimer();
  t1 = cache.Get(10);
  size_t end = Timer::microtimer();
  if(end - start > 1) MARK_FAIL;
  
  start = Timer::microtimer();
  t1 = cache.Get(11);
  end = Timer::microtimer();
  if(end - start <= 1) MARK_FAIL;
  (void)t1;
  MARK_END;
}

BOOST_AUTO_TEST_SUITE_END();
