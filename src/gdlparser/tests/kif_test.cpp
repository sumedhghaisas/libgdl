/**
 * @file kif_test.cpp
 * @author Sumedh Ghaisas
 *
 * Test file for GDLParser.
 */
#include <gdlparser/kif.hpp>

#include <boost/test/unit_test.hpp>
#include "old_boost_test_definitions.hpp"
#include "gdlparser_test.hpp"

BOOST_AUTO_TEST_SUITE(KIFTest);

using namespace std;
using namespace gdlparser;

/**
 * Check normal parsing.
 */
BOOST_AUTO_TEST_CASE(NormalTest)
{
    MARK_START;
    OPEN_LOG;
	KIF kif(false, TEST_LOG);
	kif.AddFile("data/8puzzle.kif");
	if(!kif.Parse()) MARK_FAIL;
	MARK_END;
}

/**
 * Check multiple arity error.
 */
BOOST_AUTO_TEST_CASE(MultipleArityErrorTest)
{
    MARK_START;
    OPEN_LOG;
	KIF kif(false, TEST_LOG);
	kif.AddFile("data/mularity.kif");
	if(kif.Parse()) MARK_FAIL;
	MARK_END;
}

BOOST_AUTO_TEST_SUITE_END();
