/**
 * @file gdl_tests.cpp
 * @author Sumedh Ghaisas
 *
 * Test file for GDL class.
 */
#include <libgdl/core.hpp>
#include <libgdl/core/data_types/b_state.hpp>
#include <libgdl/core/data_types/b_move.hpp>
#include <libgdl/core/data_types/move_list.hpp>
#include <libgdl/gdl.hpp>
#include <libgdl/reasoners/gdlreasoner/gdlreasoner.hpp>

#include <vector>

#include <boost/test/unit_test.hpp>
#include "old_boost_test_definitions.hpp"
#include "libgdl_test.hpp"

BOOST_AUTO_TEST_SUITE(GDLTests);

using namespace std;
using namespace libgdl;
using namespace libgdl::util;
using namespace libgdl::core;
using namespace libgdl::gdlreasoner;

/**
 * Test GDL abstraction for GetNextState
 */ 
BOOST_AUTO_TEST_CASE(GDLGetNextStateTest)
{
  MARK_START;
  OPEN_LOG;
  
  GDL<GDLReasoner> gdl("data/games/3puzzle.kif", 1024, TEST_LOG);
  
  const BState& s1 = gdl.InitState();
  
  SymbolTable sym = gdl.GetSymbolTable();
  
  BMove moves("right", sym, TEST_LOG);  
  BState s2 = gdl.GetNextState(s1, moves);
  
  list<Argument*> result2;
  result2.push_back(new Argument("(cell 1 1 3)", 
                                 sym, false, TEST_LOG));
  result2.push_back(new Argument("(cell 1 2 b)", 
                                 sym, false, TEST_LOG));
  result2.push_back(new Argument("(cell 2 1 2)", 
                                 sym, false, TEST_LOG));
  result2.push_back(new Argument("(cell 2 2 1)", 
                                 sym, false, TEST_LOG));
  result2.push_back(new Argument("(step 2)", 
                                 sym, false, TEST_LOG));

  BState s3(new RawBState(result2));
  
  if(s2 != s3) MARK_FAIL;
  
  MARK_END;
}

/**
 * Test GDL abstraction for GetNextState cache
 */
BOOST_AUTO_TEST_CASE(GDLGetNextBStateCacheTest_Time)
{
  MARK_START;
  OPEN_LOG;
  
  GDL<GDLReasoner> gdl("data/games/3puzzle.kif", 1024, TEST_LOG);
  
  const BState& s1 = gdl.InitState();
  
  SymbolTable sym = gdl.GetSymbolTable();
  
  BMove moves("right", sym, TEST_LOG);  
  
  gdl.GetNextState(s1, moves);
  
  size_t start = Timer::microtimer();
  gdl.GetNextState(s1, moves);
  size_t end = Timer::microtimer();
  
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
  
  GDL<GDLReasoner> gdl("data/games/3puzzle.kif", 1024, TEST_LOG);
  
  BState s = gdl.InitState();
  
  if(gdl.IsTerminal(s)) MARK_FAIL;
  
  SymbolTable sym = gdl.GetSymbolTable();
  
  vector<BMove> moves;
  moves.push_back(BMove("down", sym, TEST_LOG));
  moves.push_back(BMove("right", sym, TEST_LOG));
  moves.push_back(BMove("up", sym, TEST_LOG));
  moves.push_back(BMove("left", sym, TEST_LOG));
  moves.push_back(BMove("down", sym, TEST_LOG));
  moves.push_back(BMove("right", sym, TEST_LOG));
  
  for(size_t i = 0;i < moves.size();i++)
    s = gdl.GetNextState(s, moves[i]);
  
  if(!gdl.IsTerminal(s)) MARK_FAIL;
  
  MARK_END;
}

/**
 * Test GDL abstraction for IsTerminal cache
 */
BOOST_AUTO_TEST_CASE(GDLIsTerminalCacheTest_Time)
{
  MARK_START;
  OPEN_LOG;
  
  GDL<GDLReasoner> gdl("data/games/3puzzle.kif", 1024, TEST_LOG);
  
  BState s = gdl.InitState();
  
  gdl.IsTerminal(s);
  
  size_t start = Timer::microtimer();
  gdl.IsTerminal(s);
  size_t end = Timer::microtimer();
  
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
  
  GDL<GDLReasoner> gdl("data/games/nine_board_tictactoe.kif", 1024, TEST_LOG);
  
  const BState& init = gdl.InitState();
  
  MoveList<BMove> moves = gdl.GetLegalMoves(init);
    
  if(moves.size() != 81) MARK_FAIL;
    
  MARK_END;
}

/**
 * Test GDL abstraction for GetLegalMoves cache
 */
BOOST_AUTO_TEST_CASE(GDLGetLegalMovesCacheTest_Time)
{
  MARK_START;
  OPEN_LOG;
  
  GDL<GDLReasoner> gdl("data/games/nine_board_tictactoe.kif", 1024, TEST_LOG);
  
  const BState& init = gdl.InitState();
  
  gdl.GetLegalMoves(init);
  size_t start = Timer::microtimer();
  gdl.GetLegalMoves(init);
  size_t end = Timer::microtimer();
  
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
  
  GDL<GDLReasoner> gdl("data/games/3puzzle.kif", 1024, TEST_LOG);
  
  const BState& init = gdl.InitState();
  
  size_t g1 = gdl.GetGoal(init, 0);
  if(g1 != 0) MARK_FAIL;
  
  BState s = init;
  
  SymbolTable sym = gdl.GetSymbolTable();
  
  vector<BMove> moves;
  moves.push_back(BMove("down", sym, TEST_LOG));
  moves.push_back(BMove("right", sym, TEST_LOG));
  moves.push_back(BMove("up", sym, TEST_LOG));
  moves.push_back(BMove("left", sym, TEST_LOG));
  moves.push_back(BMove("down", sym, TEST_LOG));
  moves.push_back(BMove("right", sym, TEST_LOG));
  
  for(size_t i = 0;i < moves.size();i++)
    s = gdl.GetNextState(s, moves[i]);
    
  g1 = gdl.GetGoal(s, 0);
  if(g1 != 100) MARK_FAIL;
    
  MARK_END;
}

/**
 * Test GDL abstraction for GetGoal cache
 */
BOOST_AUTO_TEST_CASE(GDLGetGoalCacheTest_Time)
{
  MARK_START;
  OPEN_LOG;
  
  GDL<GDLReasoner> gdl("data/games/nine_board_tictactoe.kif", 1024, TEST_LOG);
  
  const BState& init = gdl.InitState();
  
  gdl.GetGoal(init, 0);
  size_t start = Timer::microtimer();
  gdl.GetGoal(init, 0);
  size_t end = Timer::microtimer();
  
  if(end - start > 4) MARK_FAIL;
    
  MARK_END;
}

BOOST_AUTO_TEST_SUITE_END();
