/**
 * @file clause_tests.cpp
 * @author Sumedh Ghaisas
 *
 * Test file for Clause data structure.
 */
#include <libgdl/core.hpp>

#include <iostream>
#include <sstream>

#include <boost/test/unit_test.hpp>
#include "old_boost_test_definitions.hpp"
#include "libgdl_test.hpp"

BOOST_AUTO_TEST_SUITE(ClauseTests);

using namespace std;
using namespace libgdl;
using namespace libgdl::core;

/**
 * Test Clause for construction from string.
 */
BOOST_AUTO_TEST_CASE(ClauseStringConstructionTest)
{
  MARK_START;
  OPEN_LOG;
  SymbolTable symbol_table;
  Clause c("(<= terminal (test x))", symbol_table, TEST_LOG);
  
  stringstream stream;
  SymbolDecodeStream sds(&symbol_table, stream);
  sds << c;
  
  if(stream.str() != "(<= terminal ( test x ) )") MARK_FAIL;
  
  c = Clause("(<= (test ?x) (test2 ?x ?y))", symbol_table, TEST_LOG);
  
  if(c.head->args[0] != c.premisses[0]->args[0]) MARK_FAIL; 
  MARK_END;
}

BOOST_AUTO_TEST_SUITE_END();
