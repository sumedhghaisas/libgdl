/**
 * @file unify_tests.cpp
 * @author Sumedh Ghaisas
 *
 * Test file for Unification tools.
 */
#include <sstream> 
#include <list>

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
  
  VariableMap v_map;
  if(!Unify::mgu(arg1, arg2, v_map)) MARK_FAIL;
  
  VariableMap::iterator it = v_map.begin();
  if(*it->first != *arg1.args[0]) MARK_FAIL;
  if(*it->second != *arg2.args[0]) MARK_FAIL;
  
  arg2 = Argument("(test (test2 x))", symbol_table, true, TEST_LOG);
  
  v_map = VariableMap();
  if(!Unify::mgu(arg1, arg2, v_map)) MARK_FAIL;
  it = v_map.begin();
  if(*it->first != *arg1.args[0]) MARK_FAIL;
  if(*it->second != *arg2.args[0]) MARK_FAIL;
  MARK_END;
}

/**
 * Equate with substitution check
 */ 
BOOST_AUTO_TEST_CASE(EquateWithSubTest)
{
  MARK_START;
  OPEN_LOG;
  SymbolTable symbol_table;
  
  Argument arg1("(test ?x)", symbol_table, true, TEST_LOG);
  Argument arg2("(test ?y)", symbol_table, true, TEST_LOG);
  
  Argument arg3("x", symbol_table, false, TEST_LOG);
  
  VariableMap v_map;
  v_map[arg1.args[0]] = &arg3;
  v_map[arg2.args[0]] = &arg3;
  
  if(!Unify::EquateWithSubstitution(arg1, arg2, v_map)) MARK_FAIL;
  MARK_END;
}

/**
 * Get substituted argument test
 */ 
BOOST_AUTO_TEST_CASE(GetSubArgTest)
{
  MARK_START;
  OPEN_LOG;
  SymbolTable symbol_table;
  
  Argument arg1("(test ?x)", symbol_table, true, TEST_LOG);
  Argument arg2("(test2 x)", symbol_table, false, TEST_LOG);
  
  VariableMap v_map;
  v_map[arg1.args[0]] = &arg2;
  
  Argument* result = Unify::GetSubstitutedArgument(&arg1, v_map);
  
  stringstream stream;
  SymbolDecodeStream sds(symbol_table, stream);
  sds << *result;
  
  if(stream.str() != "( test ( test2 x ) )") MARK_FAIL;
  delete result;
  MARK_END; 
}

/**
 * IsGroundQuestion test
 */
BOOST_AUTO_TEST_CASE(IsGroundQuestionTest)
{
  MARK_START;
  OPEN_LOG;
  SymbolTable symbol_table;
  
  Argument arg1("(test ?x)", symbol_table, true, TEST_LOG);
  Argument arg2("(test2 x)", symbol_table, false, TEST_LOG);
  
  VariableMap v_map;
  v_map[arg1.args[0]] = &arg2;
  
  if(!Unify::IsGroundQuestion(&arg1, v_map)) MARK_FAIL;
  MARK_END;
}

/**
 * DecodeSubstitutions Test
 */ 
BOOST_AUTO_TEST_CASE(DecodeSubstitutionsTest)
{
  MARK_START;
  OPEN_LOG;
  SymbolTable symbol_table;
  
  Argument arg1("(test ?x)", symbol_table, true, TEST_LOG);
  Argument arg2("(test2 ?x)", symbol_table, false, TEST_LOG);
  Argument arg3("x", symbol_table, false, TEST_LOG);
  
  VariableMap v_map;
  v_map[arg1.args[0]] = &arg2;
  v_map[arg2.args[0]] = &arg3;
  
  list<Argument*> to_del;
  
  v_map = Unify::DecodeSubstitutions(v_map, &arg1, VariableMap(), to_del);
  
  if(v_map.begin()->second == &arg2) MARK_FAIL;
  if(to_del.empty()) MARK_FAIL;
  for(list<Argument*>::iterator it = to_del.begin();it != to_del.end();it++)
  {
    delete *it;
  }
  MARK_END;
}

BOOST_AUTO_TEST_SUITE_END();
