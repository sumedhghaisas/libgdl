/**
 * @file parser_ds_tests.cpp
 * @author Sumedh Ghaisas
 *
 * GDLParser debugging symbol test.
 */
#include <libgdl/core.hpp>
#include <libgdl/gdlparser/kif.hpp>

#include <boost/test/unit_test.hpp>
#include "old_boost_test_definitions.hpp"
#include "libgdl_test.hpp"

BOOST_AUTO_TEST_SUITE(ParserDSTest);

using namespace std;
using namespace libgdl;
using namespace libgdl::gdlparser;

/**
 * Check whether line number and filename are created properly
 */
BOOST_AUTO_TEST_CASE(CreateDSTest)
{
  MARK_START;
  OPEN_LOG;
  KIF kif(false, false, 0, TEST_LOG);
  kif.AddFile("data/parser_tests/ds_test.kif");
  if(!kif.Parse(true)) MARK_FAIL;

  const vector<Fact>& facts = kif.Facts();
  if(facts[0].loc != Location(4, "data/parser_tests/ds_test.kif")) MARK_FAIL;

  MARK_END;
}

/**
 * Check whether line number and filename are loaded properly
 */
BOOST_AUTO_TEST_CASE(LoadDSTest)
{
  MARK_START;
  OPEN_LOG;
  KIF kif(false, true, 0, TEST_LOG);
  kif.AddFile("data/parser_tests/ds_test.kif");
  if(!kif.Parse(true)) MARK_FAIL;

  kif.PrintToFile("data/parser_tests/temp.kif");

  KIF kif2(false, true, 0, TEST_LOG);
  kif2.AddFile("data/parser_tests/temp.kif");
  if(!kif2.Parse(true)) MARK_FAIL;

  const vector<Fact>& facts = kif2.Facts();
  if(facts[0].loc != Location(4, "data/parser_tests/ds_test.kif")) MARK_FAIL;

  MARK_END;
}

BOOST_AUTO_TEST_SUITE_END();
