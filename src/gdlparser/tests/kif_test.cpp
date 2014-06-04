/**
 * @file kif_test.cpp
 * @author Sumedh Ghaisas
 *
 * Test file for GDLParser.
 */
#include <gdlparser/kif.hpp>

#include <boost/test/unit_test.hpp>
#include "old_boost_test_definitions.hpp"

BOOST_AUTO_TEST_SUITE(KIFTest);

using namespace std;
using namespace gdlparser;

/**
 * Check normal parsing.
 */
BOOST_AUTO_TEST_CASE(NormalTest)
{
	KIF kif("");
	kif.AddFile("data/8puzzle.kif");
	bool result = kif.Parse();
	if(!result) BOOST_FAIL("Parse on '8puzzle.kif' failed...");
}

/** 
 * Check head variable bound.
 */
BOOST_AUTO_TEST_CASE(HeadBoundTest)
{
	KIF kif("");
	kif.AddFile("data/headbound.kif");
	bool result = kif.Parse();
	if(result) BOOST_FAIL("Head variable bound test failed...");
}

/**
 * Check unstratified recursion is detected.
 */
BOOST_AUTO_TEST_CASE(UnstratTest)
{
	KIF kif("");
	kif.AddFile("data/unstratrec.kif");
	bool result = kif.Parse();
	if(result) BOOST_FAIL("Unstratified recursion detection test failed...");
}

BOOST_AUTO_TEST_SUITE_END();
