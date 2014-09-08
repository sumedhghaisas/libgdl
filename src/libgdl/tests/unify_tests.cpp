/**
 * @file unify_tests.cpp
 * @author Sumedh Ghaisas
 *
 * Test file for Unification tools.
 */
#include <libgdl/core.hpp>
#include <libgdl/gdlreasoner/logicbase/unify.hpp>

#include <boost/test/unit_test.hpp>
#include "old_boost_test_definitions.hpp"
#include "libgdl_test.hpp"

BOOST_AUTO_TEST_SUITE(UnifyTests);

using namespace std;
using namespace libgdl;
using namespace libgdl::core;
using namespace libgdl::gdlreasoner::logicbase;

/**
 * Basic MGU test
 */
BOOST_AUTO_TEST_CASE(MGUTest)
{
  MARK_START;
  OPEN_LOG;
  SymbolTable symbol_table;
  
  Argument arg1("(test ?x)", symbol_table, true, TEST_LOG);
  Argument arg2("(test x)", symbol_table, true, TEST_LOG);
  
  Unify::VariableMap v_map;
  if(!Unify::mgu(arg1, arg2, v_map)) MARK_FAIL;
  
  Unify::VariableMap::iterator it = v_map.begin();
  if(it->first != arg1.args[0]) MARK_FAIL;
  if(it->second != arg2.args[0]) MARK_FAIL;
  MARK_END;
}

BOOST_AUTO_TEST_SUITE_END();
