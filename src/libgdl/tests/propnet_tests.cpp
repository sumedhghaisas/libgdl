/**
 * @file propnet_tests.cpp
 * @author Sumedh Ghaisas
 *
 * Test file for Argument data structure.
 */
#include <libgdl/core.hpp>
#include <libgdl/gdlparser/kif.hpp>
#include <libgdl/propnet/propnet.hpp>

#include <list>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include <boost/test/unit_test.hpp>
#include "old_boost_test_definitions.hpp"
#include "libgdl_test.hpp"

using namespace std;
using namespace libgdl;
using namespace libgdl::propnet;

BOOST_AUTO_TEST_SUITE(PropnetTests);

BOOST_AUTO_TEST_CASE(PropNetGenerationTest_NMC)
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
  games.push_back("data/games/kono.kif");
  games.push_back("data/games/multiple_buttons_and_lights.kif");
  
  Log log;
  
  for(auto it : games)
  {
    log.Info << "Generation PropNet for game " << it << "...";
    PropNet pn(it, TEST_LOG);
    log.Info << "\033[0;32m" "[PASSED]" "\033[0m" << endl;
  }
  MARK_END;
}

BOOST_AUTO_TEST_CASE(PropNetGenerationMemoryTest)
{
  MARK_START;
  OPEN_LOG;
  
  PropNet pn("data/games/3puzzle.kif", TEST_LOG);
  
  MARK_END;
} 

BOOST_AUTO_TEST_SUITE_END();
