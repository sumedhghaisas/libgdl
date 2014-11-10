/**
 * @file kif_tests.cpp
 * @author Sumedh Ghaisas
 *
 * Test file for KIF parser.
 */
#include <libgdl/core.hpp>
#include <libgdl/gdlparser/kif.hpp>
#include <libgdl/reasoners/gdlreasoner/knowledgebase.hpp>

#include <boost/test/unit_test.hpp>
#include "old_boost_test_definitions.hpp"
#include "libgdl_test.hpp"

BOOST_AUTO_TEST_SUITE(KIFTests);

using namespace std;
using namespace libgdl;
using namespace libgdl::core;
using namespace libgdl::gdlparser;
using namespace libgdl::gdlreasoner;

/**
 * Variable parsing test
 */
BOOST_AUTO_TEST_CASE(VariableParsingTest)
{
  MARK_START;
  OPEN_LOG;
  KIF kif(false, 0, TEST_LOG);
  kif.AddFile("data/parser_tests/variable_parsing.kif");
  if(!kif.Parse(true)) MARK_FAIL;
  
  const Clause& c = kif.Clauses().front();
  if(c.head->args[0] != c.premisses[0]->args[0]) MARK_FAIL;
  if(c.head->args[1] != c.premisses[0]->args[1]->args[0]) MARK_FAIL;
  MARK_END;
}

/**
 * game parsing test.
 */
BOOST_AUTO_TEST_CASE(GameParsingTest)
{
  MARK_START;
  cout << endl;
  OPEN_LOG;
  
  list<string> games;
  games.push_back("data/games/3puzzle.kif");
  games.push_back("data/games/8puzzle.kif");
  games.push_back("data/games/nine_board_tictactoe.kif");
  games.push_back("data/games/alquerque.kif");
  games.push_back("data/games/alquerque_zero_sum.kif");
  games.push_back("data/games/buttons_and_lights.kif");
  games.push_back("data/games/best_buttons_and_lights.kif");
  games.push_back("data/games/best_buttons_and_lights_extended.kif");
  games.push_back("data/games/breakthrough.kif");
  games.push_back("data/games/checker_on_barrel_no_kings.kif");
  games.push_back("data/games/chinese_checker.kif");
  games.push_back("data/games/chinese_checker_4.kif");
  games.push_back("data/games/chinook.kif");
  games.push_back("data/games/connect_four.kif");
  games.push_back("data/games/dual_hamilton.kif");
  games.push_back("data/games/dual_hunter.kif");
  games.push_back("data/games/dual_rainbow.kif");
  games.push_back("data/games/farming_quandries.kif");
  games.push_back("data/games/fire_sheep.kif");
  games.push_back("data/games/freeforall.kif");
  games.push_back("data/games/hamilton.kif");
  games.push_back("data/games/hex.kif");
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
    log.Info << "\033[0;32m" " [PASSED]" "\033[0m" << endl;
  }
}

/**
 * Check multiple arity error.
 */
BOOST_AUTO_TEST_CASE(MultipleArityErrorTest)
{
  MARK_START;
  OPEN_LOG;
  KIF kif(false, 0, TEST_LOG);
  kif.AddFile("data/parser_tests/mularity.kif");
  if(kif.Parse()) MARK_FAIL;
  MARK_END;
}

/**
 * Check stratified negation.
 */
BOOST_AUTO_TEST_CASE(StratifiedNegationTest)
{
  MARK_START;
  OPEN_LOG;
  KIF kif(false, 0, TEST_LOG);
  kif.AddFile("data/parser_tests/notunstrat.kif");
  if(kif.Parse()) MARK_FAIL;
  MARK_END;
}

/**
 * Check for stratified recursion. (Refer to definition 15 in GDL specifications.)
 */
BOOST_AUTO_TEST_CASE(StratifiedRecursionTest)
{
  MARK_START;
  OPEN_LOG;
  KIF kif(false, 0, TEST_LOG);
  kif.AddFile("data/parser_tests/recurseunstrat.kif");
  if(kif.Parse()) MARK_FAIL;
  MARK_END;
}

/**
 * Check if head safety is correctly implemented
 */
BOOST_AUTO_TEST_CASE(HeadSafetyTest)
{
  MARK_START;
  OPEN_LOG;
  KIF kif(false, 0, TEST_LOG);
  kif.AddFile("data/parser_tests/headsafe.kif");
  if(kif.Parse()) MARK_FAIL;
  MARK_END;
}

/**
 * Check if not safety is correctly implemented
 */
BOOST_AUTO_TEST_CASE(NotSafetyTest)
{
  MARK_START;
  OPEN_LOG;
  KIF kif(false, 0, TEST_LOG);
  kif.AddFile("data/parser_tests/notsafe.kif");
  if(kif.Parse()) MARK_FAIL;
  MARK_END;
}

/**
 * Check if boundedness of relation 'or' is correctly implemented
 */
BOOST_AUTO_TEST_CASE(OrBoundTest)
{
  MARK_START;
  OPEN_LOG;
  KIF kif(false, 0, TEST_LOG);
  kif.AddFile("data/parser_tests/orsafe.kif");
  if(kif.Parse()) MARK_FAIL;
  MARK_END;
}

/**
 * Check that base cannot be dependent on true or does.
 */
BOOST_AUTO_TEST_CASE(BaseDependencyTest)
{
  MARK_START;
  OPEN_LOG;
  KIF kif(false, 0, TEST_LOG);
  kif.AddFile("data/parser_tests/base_depen.kif");
  if(kif.Parse()) MARK_FAIL;
  MARK_END;
}

/**
 * Check that input cannot be dependent on true or does.
 */
BOOST_AUTO_TEST_CASE(InputDependencyTest)
{
  MARK_START;
  OPEN_LOG;
  KIF kif(false, 0, TEST_LOG);
  kif.AddFile("data/parser_tests/input_depen.kif");
  if(kif.Parse()) MARK_FAIL;
  MARK_END;
}

/**
 * Check that init cannot be dependent on true or does.
 */
BOOST_AUTO_TEST_CASE(InitDependencyTest)
{
  MARK_START;
  OPEN_LOG;
  KIF kif(false, 0, TEST_LOG);
  kif.AddFile("data/parser_tests/init_depen.kif");
  if(kif.Parse()) MARK_FAIL;
  MARK_END;
}

/**
 * Check that legal cannot be dependent on does.
 */
BOOST_AUTO_TEST_CASE(LeaglDependencyTest)
{
  MARK_START;
  OPEN_LOG;
  KIF kif(false, 0, TEST_LOG);
  kif.AddFile("data/parser_tests/legal_depen.kif");
  if(kif.Parse()) MARK_FAIL;
  MARK_END;
}

/**
 * Check if error occurs when role is undefined.
 */
BOOST_AUTO_TEST_CASE(RoleUndefinedTest)
{
  MARK_START;
  OPEN_LOG;
  KIF kif(false, 0, TEST_LOG);
  kif.AddFile("data/parser_tests/ndef_role.kif");
  if(kif.Parse()) MARK_FAIL;
  MARK_END;
}

/**
 * Check if error occurs when legal is undefined.
 */
BOOST_AUTO_TEST_CASE(LegalUndefinedTest)
{
  MARK_START;
  OPEN_LOG;
	KIF kif(false, 0, TEST_LOG);
  kif.AddFile("data/parser_tests/ndef_legal.kif");
  if(kif.Parse()) MARK_FAIL;
  MARK_END;
}

/**
 * Check if error occurs when terminal is undefined.
 */
BOOST_AUTO_TEST_CASE(TerminalUndefinedTest)
{
  MARK_START;
  OPEN_LOG;
  KIF kif(false, 0, TEST_LOG);
  kif.AddFile("data/parser_tests/ndef_terminal.kif");
  if(kif.Parse()) MARK_FAIL;
  MARK_END;
}

/**
 * Check if error occurs when goal is undefined.
 */
BOOST_AUTO_TEST_CASE(GoalUndefinedTest)
{
  MARK_START;
  OPEN_LOG;
  KIF kif(false, 0, TEST_LOG);
  kif.AddFile("data/parser_tests/ndef_goal.kif");
  if(kif.Parse()) MARK_FAIL;
  MARK_END;
}

/**
 * Test whether optmization level 1 of KIF is working fine.
 */
BOOST_AUTO_TEST_CASE(KIFOptLevel1Test)
{
  MARK_START;
  OPEN_LOG;
  KIF kif(false, 1, TEST_LOG);
  kif.AddFile("data/parser_tests/opt1.kif");
  if(!kif.Parse(true)) MARK_FAIL;
  
  KnowledgeBase kb(kif);
  list<Argument*> result = kb.Ask("(test2 ?x ?y)");
  if(result.size() != 2) MARK_FAIL;
  
  for(list<Argument*>::iterator it = result.begin();it != result.end();it++)
    delete *it;
  MARK_END;
}

BOOST_AUTO_TEST_SUITE_END();
