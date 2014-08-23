/**
 * @file gdl_tests.cpp
 * @author Sumedh Ghaisas
 *
 * Test file for GDL class.
 */
#include <libgdl/core.hpp>
#include <libgdl/gdlparser/kif.hpp>
#include <libgdl/gdl.hpp>

#include <boost/test/unit_test.hpp>
#include "old_boost_test_definitions.hpp"
#include "libgdl_test.hpp"

BOOST_AUTO_TEST_SUITE(GDLTests);

using namespace std;
using namespace libgdl;
using namespace libgdl::gdlparser;

/**
 * Test GDL abstraction for GetNextState
 */ 
BOOST_AUTO_TEST_CASE(GDLGetNextStateTest)
{
  MARK_START;
  OPEN_LOG;
  
  KIF kif;
  kif.GetLog() = TEST_LOG;
  kif.AddFile("data/games/3puzzle.kif");
  if(!kif.Parse()) MARK_FAIL;
  
  GDL gdl(kif);
  
  const State& s1 = gdl.InitState();
  
  vector<Argument*> moves;
  moves.push_back(new Argument("right"));
  
  State s2 = gdl.GetNextState(s1, moves);
  
  if(s2.GetHash() != 52) MARK_FAIL;
  
  MARK_END;
}

/**
 * Test GDL abstraction for GetNextState cache
 */ 
BOOST_AUTO_TEST_CASE(GDLGetNextStateCacheTest)
{
  MARK_START;
  OPEN_LOG;
  
  KIF kif;
  kif.GetLog() = TEST_LOG;
  kif.AddFile("data/games/3puzzle.kif");
  if(!kif.Parse()) MARK_FAIL;
  
  GDL gdl(kif);
  
  const State& s1 = gdl.InitState();
  
  vector<Argument*> moves;
  moves.push_back(new Argument("right"));
  
  gdl.GetNextState(s1, moves);
  
  size_t start = microtimer();
  gdl.GetNextState(s1, moves);
  size_t end = microtimer();
  
  if(end - start > 15) MARK_FAIL;
  
  MARK_END;
}

BOOST_AUTO_TEST_SUITE_END();
