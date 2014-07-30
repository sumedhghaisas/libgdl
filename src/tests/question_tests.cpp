/**
 * @file question_tests.cpp
 * @author Sumedh Ghaisas
 *
 * Test file for GDLParser.
 */
#include <gdlparser/kif.hpp>
#include <gdlreasoner/knowledgebase.hpp>

#include <boost/test/unit_test.hpp>
#include "old_boost_test_definitions.hpp"
#include "libgdl_test.hpp"

BOOST_AUTO_TEST_SUITE(QuestionTests);

using namespace std;
using namespace gdlparser;
using namespace gdlreasoner;

/**
 * Answer question 1.
 */
BOOST_AUTO_TEST_CASE(Question1Test)
{
    MARK_START;
    OPEN_LOG;
    KIF kif(false, false, 0, TEST_LOG);
    kif.AddFile("data/questions/question1.kif");
    if(!kif.Parse(true)) MARK_FAIL;
    KnowledgeBase test(kif);
    list<Argument*> result = test.Ask(Argument("(engineer ?x)"), true);
    if(result.size() != 1) MARK_FAIL;
    if(**result.begin() != Argument("(engineer bajaj)")) MARK_FAIL;
    MARK_END;
}

/**
 * Answer question 2.
 */
BOOST_AUTO_TEST_CASE(Question2Test)
{
    MARK_START;
    OPEN_LOG;
    KIF kif(false, false, 0, TEST_LOG);
    kif.AddFile("data/questions/question2.kif");
    if(!kif.Parse(true)) MARK_FAIL;
    KnowledgeBase test(kif);
    list<Argument*> result = test.Ask(Argument("(married cam ?x)"), true);
    if(result.size() != 1) MARK_FAIL;
    if(**result.begin() != Argument("(married cam carrie)")) MARK_FAIL;
    MARK_END;
}

BOOST_AUTO_TEST_SUITE_END();
