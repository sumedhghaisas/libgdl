/**
 * @file rule_safety_test.cpp
 * @author Sumedh Ghaisas
 *
 * Test file for GDLParser.
 */
#include <gdlparser/kif.hpp>

#include <boost/test/unit_test.hpp>
#include "old_boost_test_definitions.hpp"
#include "gdlparser_test.hpp"

BOOST_AUTO_TEST_SUITE(SafetyTest);

using namespace std;
using namespace gdlparser;

/**
 * Check if head safety is correctly implemented
 */
BOOST_AUTO_TEST_CASE(HeadSafetyTest)
{
    MARK_START;
    OPEN_LOG;
	KIF kif("", false, false, TEST_LOG);
	kif.AddFile("data/headsafe.kif");
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
	KIF kif("", false, false, TEST_LOG);
	kif.AddFile("data/notsafe.kif");
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
	KIF kif("", false, false, TEST_LOG);
	kif.AddFile("data/orsafe.kif");
	if(kif.Parse()) MARK_FAIL;
	MARK_END;
}

BOOST_AUTO_TEST_SUITE_END();

