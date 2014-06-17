/**
 * @file defined_rel_test.cpp
 * @author Sumedh Ghaisas
 *
 * Test file for GDLParser.
 */
#include <gdlparser/kif.hpp>

#include <boost/test/unit_test.hpp>
#include "old_boost_test_definitions.hpp"
#include "gdlparser_test.hpp"

BOOST_AUTO_TEST_SUITE(DRelTests);

using namespace std;
using namespace gdlparser;

/**
 * Check if error occurs when role is undefined.
 */
BOOST_AUTO_TEST_CASE(RoleUndefinedTest)
{
    MARK_START;
    OPEN_LOG;
	KIF kif(false, TEST_LOG);
    kif.AddFile("data/ndef_role.kif");
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
	KIF kif(false, TEST_LOG);
    kif.AddFile("data/ndef_legal.kif");
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
	KIF kif(false, TEST_LOG);
    kif.AddFile("data/ndef_terminal.kif");
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
	KIF kif(false, TEST_LOG);
    kif.AddFile("data/ndef_goal.kif");
    if(kif.Parse()) MARK_FAIL;
    MARK_END;
}

BOOST_AUTO_TEST_SUITE_END();

