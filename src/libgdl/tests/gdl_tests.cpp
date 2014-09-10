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
using namespace libgdl::core;

/**
 * Test GDL abstraction for GetNextState
 */ 
BOOST_AUTO_TEST_CASE(GDLGetNextStateTest)
{
  MARK_START;
  OPEN_LOG;
  
  GDL gdl("data/games/3puzzle.kif", 1024, TEST_LOG);
  
  const State& s1 = gdl.InitState();
  
  Move moves("right", *gdl.GetSymbolTable(), TEST_LOG);  
  State s2 = gdl.GetNextState(s1, moves);
  
  list<Argument*> result2;
  result2.push_back(new Argument("(cell 1 1 3)", 
                                 *gdl.GetSymbolTable(), false, TEST_LOG));
  result2.push_back(new Argument("(cell 1 2 b)", 
                                 *gdl.GetSymbolTable(), false, TEST_LOG));
  result2.push_back(new Argument("(cell 2 1 2)", 
                                 *gdl.GetSymbolTable(), false, TEST_LOG));
  result2.push_back(new Argument("(cell 2 2 1)", 
                                 *gdl.GetSymbolTable(), false, TEST_LOG));
  result2.push_back(new Argument("(step 2)", 
                                 *gdl.GetSymbolTable(), false, TEST_LOG));

  State s3(new RawState(result2));
  
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
  
  Move moves("right", *gdl.GetSymbolTable(), TEST_LOG);  
  
  gdl.GetNextState(s1, moves);
  
  size_t start = microtimer();
  gdl.GetNextState(s1, moves);
  size_t end = microtimer();
  
  if(end - start > 8) MARK_FAIL;
  
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
  moves.push_back(Move("down", *gdl.GetSymbolTable(), TEST_LOG));
  moves.push_back(Move("right", *gdl.GetSymbolTable(), TEST_LOG));
  moves.push_back(Move("up", *gdl.GetSymbolTable(), TEST_LOG));
  moves.push_back(Move("left", *gdl.GetSymbolTable(), TEST_LOG));
  moves.push_back(Move("down", *gdl.GetSymbolTable(), TEST_LOG));
  moves.push_back(Move("right", *gdl.GetSymbolTable(), TEST_LOG));
  
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
  
  if(end - start > 4) MARK_FAIL;
    
  MARK_END;
}

/**
 * Test GDL abstraction for GetLegalMoves
 */
BOOST_AUTO_TEST_CASE(GDLGetLegalMovesTest)
{
  MARK_START;
  OPEN_LOG;
  
  GDL gdl("data/games/nine_board_tictactoe.kif", 1024, TEST_LOG);
  
  const State& init = gdl.InitState();
  
  MoveList moves = gdl.GetLegalMoves(init);
    
  if(moves.size() != 81) MARK_FAIL;
    
  MARK_END;
}

/**
 * Test GDL abstraction for GetLegalMoves cache
 */
BOOST_AUTO_TEST_CASE(GDLGetLegalMovesCacheTest)
{
  MARK_START;
  OPEN_LOG;
  
  GDL gdl("data/games/nine_board_tictactoe.kif", 1024, TEST_LOG);
  
  const State& init = gdl.InitState();
  
  gdl.GetLegalMoves(init);
  size_t start = microtimer();
  gdl.GetLegalMoves(init);
  size_t end = microtimer();
  
  // for safety its 3, should be 2
  if(end - start > 3) MARK_FAIL;
    
  MARK_END;
}

/**
 * Test GDL abstraction for GetGoal
 */
BOOST_AUTO_TEST_CASE(GDLGetGoalTest)
{
  MARK_START;
  OPEN_LOG;
  
  GDL gdl("data/games/3puzzle.kif", 1024, TEST_LOG);
  
  const State& init = gdl.InitState();
  
  size_t g1 = gdl.GetGoal(init, 0);
  if(g1 != 0) MARK_FAIL;
  
  State s = init;
  
  vector<Move> moves;
  moves.push_back(Move("down", *gdl.GetSymbolTable(), TEST_LOG));
  moves.push_back(Move("right", *gdl.GetSymbolTable(), TEST_LOG));
  moves.push_back(Move("up", *gdl.GetSymbolTable(), TEST_LOG));
  moves.push_back(Move("left", *gdl.GetSymbolTable(), TEST_LOG));
  moves.push_back(Move("down", *gdl.GetSymbolTable(), TEST_LOG));
  moves.push_back(Move("right", *gdl.GetSymbolTable(), TEST_LOG));
  
  for(size_t i = 0;i < moves.size();i++)
    s = gdl.GetNextState(s, moves[i]);
    
  g1 = gdl.GetGoal(s, 0);
  if(g1 != 100) MARK_FAIL;
    
  MARK_END;
}

/**
 * Test GDL abstraction for GetGoal cache
 */
BOOST_AUTO_TEST_CASE(GDLGetGoalCacheTest)
{
  MARK_START;
  OPEN_LOG;
  
  GDL gdl("data/games/nine_board_tictactoe.kif", 1024, TEST_LOG);
  
  const State& init = gdl.InitState();
  
  gdl.GetGoal(init, 0);
  size_t start = microtimer();
  gdl.GetGoal(init, 0);
  size_t end = microtimer();
  
  if(end - start > 4) MARK_FAIL;
    
  MARK_END;
}


BOOST_AUTO_TEST_SUITE_END();
