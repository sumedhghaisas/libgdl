/**
 * @file gdl_tests.cpp
 * @author Sumedh Ghaisas
 *
 * Test file for GDL class.
 */
#include <libgdl/core.hpp>

#include <iostream>
#include <sstream>

#include <boost/test/unit_test.hpp>
#include "old_boost_test_definitions.hpp"
#include "libgdl_test.hpp"

BOOST_AUTO_TEST_SUITE(ArgumentTests);

using namespace std;
using namespace libgdl;

/**
 * Test Argument for construction from string
 */
BOOST_AUTO_TEST_CASE(ArgumentStringConstructionTest)
{
  MARK_START;
  Argument arg("(test x)");
  stringstream stream;
  stream << arg;
  if(stream.str() != "( test x )") MARK_FAIL;
  
  arg = Argument("(test (test x))");
  stringstream stream2;
  stream2 << arg;
  if(stream2.str() != "( test ( test x ) )") MARK_FAIL;
  
  MARK_END;
}

/**
 * Test Argument for IsGround
 */
BOOST_AUTO_TEST_CASE(ArgumentIsGroundTest)
{
  MARK_START;
  Argument arg("(test (test x))");
  if(!arg.IsGround()) MARK_FAIL;
  
  arg = Argument("(test (test ?x))");
  if(arg.IsGround()) MARK_FAIL;
  
  arg = Argument("?x");
  if(arg.IsGround()) MARK_FAIL;
  
  MARK_END;
}

BOOST_AUTO_TEST_SUITE_END();
