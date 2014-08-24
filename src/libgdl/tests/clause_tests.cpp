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

/**
 * Test Clause for construction from string.
 */
BOOST_AUTO_TEST_CASE(ClauseStringConstructionTest)
{
  MARK_START;
  Clause c("(<= terminal (test x))");
  stringstream stream;
  stream << c;
  if(stream.str() != "(<= terminal ( test x ) )") MARK_FAIL;
  MARK_END;
}

BOOST_AUTO_TEST_SUITE_END();
