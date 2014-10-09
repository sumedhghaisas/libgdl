/**
 * @file fact_tests.cpp
 * @author Sumedh Ghaisas
 *
 * Test file for Fact data structure.
 */
#include <libgdl/core.hpp>

#include <iostream>
#include <sstream>

#include <boost/test/unit_test.hpp>
#include "old_boost_test_definitions.hpp"
#include "libgdl_test.hpp"

BOOST_AUTO_TEST_SUITE(FactTests);

using namespace std;
using namespace libgdl;
using namespace libgdl::core;

/**
 * Test Fact for construction from string.
 */ 
BOOST_AUTO_TEST_CASE(FactStringConstructionTest)
{
  MARK_START;
  OPEN_LOG;
  SymbolTable symbol_table;
  Fact f("(test (test2 x))", symbol_table, TEST_LOG);
  
  stringstream stream;
  SymbolDecodeStream sds(symbol_table, stream);
  sds << f;
  
  if(stream.str() != "( test ( test2 x ) )") MARK_FAIL;
  MARK_END;
}

BOOST_AUTO_TEST_SUITE_END();
