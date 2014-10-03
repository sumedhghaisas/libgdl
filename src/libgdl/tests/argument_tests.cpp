/**
 * @file argument_tests.cpp
 * @author Sumedh Ghaisas
 *
 * Test file for Argument data structure.
 */
#include <libgdl/core.hpp>
#include <libgdl/core/symbol_table/symbol_table.hpp>
#include <libgdl/core/symbol_table/symbol_decode_stream.hpp>

#include <iostream>
#include <sstream>

#include <boost/test/unit_test.hpp>
#include "old_boost_test_definitions.hpp"
#include "libgdl_test.hpp"

BOOST_AUTO_TEST_SUITE(ArgumentTests);

using namespace std;
using namespace libgdl;
using namespace libgdl::core;

/**
 * Test Argument for construction from string
 */
BOOST_AUTO_TEST_CASE(ArgumentStringConstructionTest)
{
  MARK_START;
  OPEN_LOG;
  SymbolTable symbol_table;
  Argument arg("(test x)", symbol_table, true, TEST_LOG);
  
  stringstream stream;
  SymbolDecodeStream sds(&symbol_table, stream);
  sds << arg;
  
  if(stream.str() != "( test x )") MARK_FAIL;
  
  arg = Argument("(test (test2 x))", symbol_table, true, TEST_LOG);
  
  stringstream stream2;
  SymbolDecodeStream sds2(&symbol_table, stream2);
  sds2 << arg;
  
  if(stream2.str() != "( test ( test2 x ) )") MARK_FAIL;
  
  arg = Argument("(test3 ?x (test2 ?x))", symbol_table, true, TEST_LOG);
  
  if(arg.args[0] != arg.args[1]->args[0]) MARK_FAIL;
  
  MARK_END;
}

/**
 * Test Argument for IsGround
 */
BOOST_AUTO_TEST_CASE(ArgumentIsGroundTest)
{
  MARK_START;
  OPEN_LOG;
  SymbolTable symbol_table;
  Argument arg("(test (test2 x))", symbol_table, true, TEST_LOG);
  if(!arg.IsGround()) MARK_FAIL;
  
  arg = Argument("(test (test2 ?x))", symbol_table, true, TEST_LOG);
  if(arg.IsGround()) MARK_FAIL;
  
  arg = Argument("?x", symbol_table, false, TEST_LOG);
  if(arg.IsGround()) MARK_FAIL;
  
  MARK_END;
}

BOOST_AUTO_TEST_SUITE_END();
