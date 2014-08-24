/**
 * @file state_tests.cpp
 * @author Sumedh Ghaisas
 *
 * Tests for State of GDL.
 */
#include <list>
#include <boost/unordered_map.hpp> 

#include <libgdl/core.hpp>
#include <libgdl/gdlparser/kif.hpp>
#include <libgdl/gdlreasoner/knowledgebase.hpp>

#include <boost/test/unit_test.hpp>
#include "old_boost_test_definitions.hpp"
#include "libgdl_test.hpp"

using namespace std;
using namespace boost;
using namespace libgdl;
using namespace libgdl::gdlparser;
using namespace libgdl::gdlreasoner;

BOOST_AUTO_TEST_SUITE(StateTests);

/**
 * Test State for hash computation.
 */
BOOST_AUTO_TEST_CASE(StateHashTest)
{
  MARK_START;
  OPEN_LOG;
  
  KIF kif;
  kif.GetLog() = TEST_LOG;
  kif.AddFile("data/games/3puzzle.kif");
  kif.Parse();
  
  unordered_map<string, size_t>& id_map = *kif.IDMap();
  
  KnowledgeBase kb(kif);
  kb.GetLog() = TEST_LOG;
  list<Argument*> result = kb.Ask(Argument("(init ?x)"));
  
  State s1(result, id_map);
  
  list<Argument*> result2;
  result2.push_back(new Argument("(init (cell 2 2 1))"));
  result2.push_back(new Argument("(init (cell 1 2 3))"));
  result2.push_back(new Argument("(init (cell 1 1 b))"));
  result2.push_back(new Argument("(init (cell 2 1 2))"));
  result2.push_back(new Argument("(init (step 1))"));

  State s2(result2, id_map);

  if(s1 != s2) MARK_FAIL;
  
  MARK_END;
}

BOOST_AUTO_TEST_SUITE_END();