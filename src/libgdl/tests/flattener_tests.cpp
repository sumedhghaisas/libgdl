/**
 * @file _tests.cpp
 * @author Sumedh Ghaisas
 *
 * Test file for Argument data structure.
 */
#include <libgdl/core.hpp>
#include <libgdl/gdlparser/kif.hpp>
#include <libgdl/reasoners/gdlreasoner/kif_flattener.hpp>
#include <libgdl/reasoners/gdlreasoner/gdlreasoner.hpp>
#include <libgdl/gdl.hpp>

#include <list>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include <boost/test/unit_test.hpp>
#include "old_boost_test_definitions.hpp"
#include "libgdl_test.hpp"

BOOST_AUTO_TEST_SUITE(FlattenerTests);

using namespace std;
using namespace libgdl;
using namespace libgdl::core;
using namespace libgdl::gdlparser;
using namespace libgdl::gdlreasoner;

/**
 * Test Argument for construction from string
 */
BOOST_AUTO_TEST_CASE(RecursiveFlatteningTest)
{
  MARK_START;
  OPEN_LOG;
  
  KIF kif(false, 0, TEST_LOG);
  kif.AddFile("data/flattener_tests/recurse.kif");
  if(!kif.Parse(true)) MARK_FAIL;
  
  KIFFlattener kiff;
  kiff.Flatten(kif);
  
  KnowledgeBase kb(kiff);
  list<Argument*> result = kb.Ask("(base ?x)");
  if(result.size() != 10) MARK_FAIL;
    
  for(list<Argument*>::iterator it = result.begin();it != result.end();it++)
    delete *it; 
  MARK_END;
}

/**
 * Test for 8puzzle flattening
 */
BOOST_AUTO_TEST_CASE(Puzzle8FlattenTest)
{
  MARK_START;
  OPEN_LOG;
  
  KIF kif(true, 1, TEST_LOG);
  kif.AddFile("data/games/8puzzle.kif");
  if(!kif.Parse()) MARK_FAIL;

  KIFFlattener kf;
  kf.Flatten(kif);

  GDL<GDLReasoner> gdl(kf);

  const State& init = gdl.InitState();

  SymbolTable sym = gdl.GetSymbolTable();

  list<Move> moves;
  moves.push_back(Move("(move 2 3)", sym, TEST_LOG));
  moves.push_back(Move("(move 2 2)", sym, TEST_LOG));
  moves.push_back(Move("(move 3 2)", sym, TEST_LOG));
  moves.push_back(Move("(move 3 1)", sym, TEST_LOG));
  moves.push_back(Move("(move 2 1)", sym, TEST_LOG));
  moves.push_back(Move("(move 2 2)", sym, TEST_LOG));
  moves.push_back(Move("(move 1 2)", sym, TEST_LOG));
  moves.push_back(Move("(move 1 1)", sym, TEST_LOG));
  moves.push_back(Move("(move 2 1)", sym, TEST_LOG));

  State s = init;

  for(list<Move>::iterator it = moves.begin();it != moves.end();it++)
    s = gdl.GetNextState(s, *it);

  list<Argument*> result2;
  result2.push_back(new Argument("(cell 2 1 b)",
                                 sym, false, TEST_LOG));
  result2.push_back(new Argument("(cell 1 1 1)",
                                 sym, false, TEST_LOG));
  result2.push_back(new Argument("(cell 1 2 8)",
                                 sym, false, TEST_LOG));
  result2.push_back(new Argument("(cell 1 3 6)",
                                 sym, false, TEST_LOG));
  result2.push_back(new Argument("(cell 2 2 7)",
                                 sym, false, TEST_LOG));
  result2.push_back(new Argument("(cell 2 3 4)",
                                 sym, false, TEST_LOG));
  result2.push_back(new Argument("(cell 3 1 5)",
                                 sym, false, TEST_LOG));
  result2.push_back(new Argument("(cell 3 2 2)",
                                 sym, false, TEST_LOG));
  result2.push_back(new Argument("(cell 3 3 3)",
                                 sym, false, TEST_LOG));
  result2.push_back(new Argument("(step 9)",
                                 sym, false, TEST_LOG));

  State s2(new RawState(result2));

  if(s != s2) MARK_FAIL;
  
  MARK_END;
}

/**
 * Check if flattening is working for all games.
 */
BOOST_AUTO_TEST_CASE(GamesFlattenTest_NMC)
{
  MARK_START;
  cout << endl;
  OPEN_LOG;

  /* initialize random seed: */
  srand (time(NULL));
  
  list<string> games;
  games.push_back("data/games/3puzzle.kif");
  games.push_back("data/games/8puzzle.kif");
  games.push_back("data/games/nine_board_tictactoe.kif");
  games.push_back("data/games/buttons_and_lights.kif");
  games.push_back("data/games/best_buttons_and_lights.kif");
  games.push_back("data/games/best_buttons_and_lights_extended.kif");
  games.push_back("data/games/connect_four.kif");
  games.push_back("data/games/dual_hamilton.kif");
  games.push_back("data/games/dual_hunter.kif");
  games.push_back("data/games/dual_rainbow.kif");
  games.push_back("data/games/farming_quandries.kif");
  games.push_back("data/games/hamilton.kif");
  games.push_back("data/games/horseshoe.kif");
  games.push_back("data/games/hunter.kif");
  games.push_back("data/games/joint_button_and_lights.kif");
  games.push_back("data/games/joint_connect_four.kif");
  games.push_back("data/games/knights_tour.kif");
  games.push_back("data/games/knights_tour_5v5.kif");

  for(auto game : games)
  {
    Log log;
    log.Info << "Testing for game: " << game;
    KIF kif(false, 0, TEST_LOG);
    kif.AddFile(game);
    if(!kif.Parse()) MARK_FAIL;

    KIFFlattener kf;
    kf.Flatten(kif);
    
    log.Info << ". Running simulation... ";

    GDL<> gdl(kf, 1024, TEST_LOG);
    State s = gdl.InitState();

    do
    {
      MoveList ml = gdl.GetLegalMoves(s);
      size_t index = rand() % ml.size();

      MoveList::iterator it = ml.begin();
      for(size_t i = 0;i < index;i++)
        it++;

      s = gdl.GetNextState(s, *it);
    }while(!gdl.IsTerminal(s));
    log.Info << "\033[0;32m" "[PASSED]" "\033[0m" << endl;
  }
  
  //! Games which are too expensive knowledgebase simulation
  
  games.clear();
  games.push_back("data/games/alquerque.kif"); 
  games.push_back("data/games/alquerque_zero_sum.kif");
  games.push_back("data/games/breakthrough.kif");
  games.push_back("data/games/checker_on_barrel_no_kings.kif");
  games.push_back("data/games/chinook.kif");
  games.push_back("data/games/freeforall.kif");
  
  for(auto game : games)
  {
    Log log;
    log.Info << "Testing for game: " << game;
    KIF kif(false, 0, TEST_LOG);
    kif.AddFile(game);
    kif.Parse();

    KIFFlattener kf;
    kf.Flatten(kif);
    
    log.Info << "\033[0;32m" " [PASSED]" "\033[0m" << endl;
  }
  
  //! Games that are too expensive for flattening
  //games.push_back("data/games/chinese_checker.kif");
  //games.push_back("data/games/chinese_checker_4.kif");
  //games.push_back("data/games/fire_sheep.kif");
  //games.push_back("data/games/hex.kif");
}


BOOST_AUTO_TEST_SUITE_END();
