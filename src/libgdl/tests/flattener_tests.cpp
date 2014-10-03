/**
 * @file _tests.cpp
 * @author Sumedh Ghaisas
 *
 * Test file for Argument data structure.
 */
#include <libgdl/core.hpp>
#include <libgdl/gdlparser/kif.hpp>
#include <libgdl/gdlreasoner/kif_flattener.hpp>
#include <libgdl/gdlreasoner/knowledgebase.hpp>

#include <list>

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
  {
    delete *it; 
  }
  MARK_END;
}

BOOST_AUTO_TEST_SUITE_END();
