/**
 * @file gdl_tests.cpp
 * @author Sumedh Ghaisas
 *
 * Test file for GDL class.
 */
#include <libgdl/core.hpp>
#include <libgdl/gdl.hpp>

#include <vector>

#include <boost/test/unit_test.hpp>
#include "old_boost_test_definitions.hpp"
#include "libgdl_test.hpp"

BOOST_AUTO_TEST_SUITE(GDLTests);

using namespace std;
using namespace libgdl;

/**
 * Test GDL abstraction for GetNextState
 */ 
BOOST_AUTO_TEST_CASE(GDLGetNextStateTest)
{
  MARK_START;
  OPEN_LOG;
  
  GDL gdl("data/games/3puzzle.kif", 1024, TEST_LOG);
  
  const State& s1 = gdl.InitState();
  
  Move moves("right");  
  State s2 = gdl.GetNextState(s1, moves);
  
  list<Argument*> result2;
  result2.push_back(new Argument("(cell 1 1 3)"));
  result2.push_back(new Argument("(cell 1 2 b)"));
  result2.push_back(new Argument("(cell 2 1 2)"));
  result2.push_back(new Argument("(cell 2 2 1)"));
  result2.push_back(new Argument("(step 2)"));

  State s3(result2, gdl.IDMap());
  
  if(s2 != s3) MARK_FAIL;
  
  MARK_END;
}

/**
 * Test GDL abstraction for GetNextState cache
 */ 
BOOST_AUTO_TEST_CASE(GDLGetNextStateCacheTest)
{
  MARK_START;
  OPEN_LOG;
  
  GDL gdl("data/games/3puzzle.kif", 1024, TEST_LOG);
  
  const State& s1 = gdl.InitState();
  
  Move moves("right");
  
  gdl.GetNextState(s1, moves);
  
  size_t start = microtimer();
  gdl.GetNextState(s1, moves);
  size_t end = microtimer();
  
  if(end - start > 15) MARK_FAIL;
  
  MARK_END;
}

/**
 * Test GDL abstraction for IsTerminal
 */
BOOST_AUTO_TEST_CASE(GDLIsTerminalTest)
{
  MARK_START;
  OPEN_LOG;
  
  GDL gdl("data/games/3puzzle.kif", 1024, TEST_LOG);
  
  State s = gdl.InitState();
  
  if(gdl.IsTerminal(s)) MARK_FAIL;
  
  vector<Move> moves;
  moves.push_back(Move("down"));
  moves.push_back(Move("right"));
  moves.push_back(Move("up"));
  moves.push_back(Move("left"));
  moves.push_back(Move("down"));
  moves.push_back(Move("right"));
  
  for(size_t i = 0;i < moves.size();i++)
    s = gdl.GetNextState(s, moves[i]);
  
  if(!gdl.IsTerminal(s)) MARK_FAIL;
  
  MARK_END;
}

/**
 * Test GDL abstraction for IsTerminal cache
 */
BOOST_AUTO_TEST_CASE(GDLIsTerminalCacheTest)
{
  MARK_START;
  OPEN_LOG;
  
  GDL gdl("data/games/3puzzle.kif", 1024, TEST_LOG);
  
  State s = gdl.InitState();
  
  gdl.IsTerminal(s);
  
  size_t start = microtimer();
  gdl.IsTerminal(s);
  size_t end = microtimer();
  
  if(end - start > 15) MARK_FAIL;
    
  MARK_END;
}

BOOST_AUTO_TEST_SUITE_END();
