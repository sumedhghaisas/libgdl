/**
 * @file state_tests.cpp
 * @author Sumedh Ghaisas
 *
 * Tests for State of GDL.
 */
#include <list>
#include <boost/unordered_map.hpp> 

#include <libgdl/core.hpp>
#include <libgdl/core/data_types/b_state.hpp>
#include <libgdl/gdlparser/kif.hpp>
#include <libgdl/reasoners/gdlreasoner/knowledgebase.hpp>

#include <boost/test/unit_test.hpp>
#include "old_boost_test_definitions.hpp"
#include "libgdl_test.hpp"

using namespace std;
using namespace boost;
using namespace libgdl;
using namespace libgdl::core;
using namespace libgdl::gdlparser;
using namespace libgdl::gdlreasoner;

BOOST_AUTO_TEST_SUITE(BStateTests);

/**
 * Test BState for hash computation.
 */
BOOST_AUTO_TEST_CASE(BStateHashTest)
{
  MARK_START;
  OPEN_LOG;
  
  KIF kif;
  kif.GetLog() = TEST_LOG;
  kif.AddFile("data/games/3puzzle.kif");
  kif.Parse();
  
  KnowledgeBase kb(kif, TEST_LOG);
  kb.GetLog() = TEST_LOG;
  list<Argument*> result = kb.Ask("(init ?x)");
  
  BState s1(new RawBState(result));
  
  SymbolTable sym = kb.GetSymbolTable();
  
  list<Argument*> result2;
  result2.push_back(new Argument("(init (cell 2 2 1))", 
                                 sym, true, TEST_LOG));
  result2.push_back(new Argument("(init (cell 1 2 3))",
                                 sym, true, TEST_LOG));
  result2.push_back(new Argument("(init (cell 1 1 b))", 
                                 sym, true, TEST_LOG));
  result2.push_back(new Argument("(init (cell 2 1 2))", 
                                 sym, true, TEST_LOG));
  result2.push_back(new Argument("(init (step 1))", 
                                 sym, true, TEST_LOG));

  BState s2(new RawBState(result2));

  if(s1 != s2) MARK_FAIL;
  
  MARK_END;
}

BOOST_AUTO_TEST_SUITE_END();
