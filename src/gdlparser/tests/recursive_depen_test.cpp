/**
 * @file recursive_depen_test.cpp
 * @author Sumedh Ghaisas
 *
 * Test file for GDLParser.
 */
#include <gdlparser/kif.hpp>

#include <boost/test/unit_test.hpp>
#include "old_boost_test_definitions.hpp"
#include "gdlparser_test.hpp"

BOOST_AUTO_TEST_SUITE(RecursiveDepenTest);

using namespace std;
using namespace gdlparser;

/**
 * Check that base cannot be dependent on true or does.
 */
BOOST_AUTO_TEST_CASE(BaseDependencyTest)
{
    MARK_START;
    OPEN_LOG;
	KIF kif("", false, false, TEST_LOG);
	kif.AddFile("data/base_depen.kif");
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
	KIF kif("", false, false, TEST_LOG);
	kif.AddFile("data/input_depen.kif");
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
	KIF kif("", false, false, TEST_LOG);
	kif.AddFile("data/init_depen.kif");
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
	KIF kif("", false, false, TEST_LOG);
	kif.AddFile("data/legal_depen.kif");
	if(kif.Parse()) MARK_FAIL;
	MARK_END;
}

BOOST_AUTO_TEST_SUITE_END();
