/**
 * @file primary_parser_tests.cpp
 * @author Sumedh Ghaisas
 *
 * Test file for GDLParser.
 */
#include <libgdl/core.hpp>
#include <libgdl/gdlparser/kif.hpp>

#include <boost/test/unit_test.hpp>
#include "old_boost_test_definitions.hpp"
#include "libgdl_test.hpp"

BOOST_AUTO_TEST_SUITE(PrimaryParserTests);

using namespace std;
using namespace libgdl;
using namespace libgdl::gdlparser;

/**
 * Check normal parsing.
 */
BOOST_AUTO_TEST_CASE(NormalParsingTest)
{
    MARK_START;
    OPEN_LOG;
	KIF kif(false, false, 0, TEST_LOG);
	kif.AddFile("data/parser_tests/8puzzle.kif");
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
	KIF kif(false, false, 0, TEST_LOG);
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
	KIF kif(false, false, 0, TEST_LOG);
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
	KIF kif(false, false, 0, TEST_LOG);
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
	KIF kif(false, false, 0, TEST_LOG);
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
	KIF kif(false, false, 0, TEST_LOG);
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
	KIF kif(false, false, 0, TEST_LOG);
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
	KIF kif(false, false, 0, TEST_LOG);
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
	KIF kif(false, false, 0, TEST_LOG);
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
	KIF kif(false, false, 0, TEST_LOG);
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
	KIF kif(false, false, 0, TEST_LOG);
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
	KIF kif(false, false, 0, TEST_LOG);
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
	KIF kif(false, false, 0, TEST_LOG);
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
	  KIF kif(false, false, 0, TEST_LOG);
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
	KIF kif(false, false, 0, TEST_LOG);
    kif.AddFile("data/parser_tests/ndef_goal.kif");
    if(kif.Parse()) MARK_FAIL;
    MARK_END;
}

BOOST_AUTO_TEST_SUITE_END();
