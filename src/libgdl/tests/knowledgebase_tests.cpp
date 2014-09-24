/**
 * @file knowledgebase_tests.cpp
 * @author Sumedh Ghaisas
 *
 * Test file for KnowledgeBase.
 */
#include <libgdl/core.hpp>
#include <libgdl/gdlreasoner/knowledgebase.hpp>
#include <libgdl/gdlparser/kif.hpp>

#include <boost/test/unit_test.hpp>
#include "old_boost_test_definitions.hpp"
#include "libgdl_test.hpp"

#include <list>

BOOST_AUTO_TEST_SUITE(KnowledgeBaseTests);

using namespace std;
using namespace libgdl;
using namespace libgdl::core;
using namespace libgdl::gdlparser;
using namespace libgdl::gdlreasoner;

/**
 * Check knowledge base construction from KIF.
 */
BOOST_AUTO_TEST_CASE(NormalReasonerTest)
{
  MARK_START;
  OPEN_LOG;
	KIF kif(false, 0, TEST_LOG);
	kif.AddFile("data/parser_tests/8puzzle.kif");
	kif.Parse();

	KnowledgeBase kb(kif, TEST_LOG);
	if(!kb.IsSatisfiable("(init ?x)")) MARK_FAIL;
	MARK_END;
}

/**
 * Check knowledge base for normal question
 */
BOOST_AUTO_TEST_CASE(QuestionTest)
{
  MARK_START;
  OPEN_LOG;
	KnowledgeBase kb(TEST_LOG);
	kb.Tell("(<= (test ?x) (test2 ?x))");
	kb.Tell("(test2 1)");
	if(!kb.IsSatisfiable("(test ?x)")) MARK_FAIL;
	MARK_END;
}

/**
 * Check knowledge base for 'or' question handling
 */
BOOST_AUTO_TEST_CASE(OrQuestionTest)
{
  MARK_START;
  OPEN_LOG;
	KnowledgeBase kb(TEST_LOG);
	kb.Tell("(<= (test ?x) (or (test2 ?x) (test3 ?x)))");
	kb.Tell("(test2 1)");
	kb.Tell("(test3 2)");
	list<Argument*> result;
	if((result = kb.Ask("(test ?x)")).size() != 2) MARK_FAIL;
	for(list<Argument*>::iterator it = result.begin();it != result.end();it++)
    delete *it;
	MARK_END;
}

/**
 * Check knowledge base for 'not' question handling
 */
BOOST_AUTO_TEST_CASE(NotQuestionTest)
{
  MARK_START;
  OPEN_LOG;
	KnowledgeBase kb(TEST_LOG);
	kb.Tell("(<= (test ?x) (test2 ?x) (not (test3 ?x)))");
	kb.Tell("(test2 1)");
	kb.Tell("(test2 2)");
	kb.Tell("(test3 2)");
	std::list<Argument*> result;
	if((result = kb.Ask("(test ?x)")).size() != 1) MARK_FAIL;
	for(std::list<Argument*>::iterator it = result.begin();it != result.end();it++)
    delete *it;
	MARK_END;
}

/**
 * Check knowledge base for 'distinct' question handling
 */
BOOST_AUTO_TEST_CASE(DistinctQuestionTest)
{
  MARK_START;
  OPEN_LOG;
	KnowledgeBase kb(TEST_LOG);
	kb.Tell("(<= (test ?x ?y) (test2 ?x) (test3 ?y) (distinct ?x ?y))");
	kb.Tell("(test2 1)");
	kb.Tell("(test3 1)");
	kb.Tell("(test3 2)");
	std::list<Argument*> result;
	if((result = kb.Ask("(test ?x ?y)")).size() != 1) MARK_FAIL;
	for(std::list<Argument*>::iterator it = result.begin();it != result.end();it++)
    delete *it;
	MARK_END;
}

/**
 * Check knowledge base for infinite loop avoidance
 */
BOOST_AUTO_TEST_CASE(RecursiveDependencyTest)
{
  MARK_START;
  OPEN_LOG;
	KnowledgeBase kb(TEST_LOG);
	kb.Tell("(<= (knows ?x ?z) (knows ?x ?y) (knows ?y ?z))");
	kb.Tell("(knows p q)");
	kb.Tell("(knows q r)");
	std::list<Argument*> result;
	if((result = kb.Ask("(knows p r)")).size() != 1) MARK_FAIL;
	for(std::list<Argument*>::iterator it = result.begin();it != result.end();it++)
    delete *it;
	MARK_END;
}

/**
 * Check knowledgebase for function implementation
 */
BOOST_AUTO_TEST_CASE(KBFunctionTest)
{
  MARK_START;
  OPEN_LOG;
  KnowledgeBase kb(TEST_LOG);
  kb.Tell("(true (step 1))");
  kb.Tell("(<= (test (step ?x)) (true (step ?x)))");
  
  list<Argument*> result = kb.Ask("(test ?x)");
  if(result.size() != 1) MARK_FAIL;
  for(std::list<Argument*>::iterator it = result.begin();it != result.end();it++)
    delete *it;
  MARK_END;
}

/**
 * Check Knowledgebase Erase.
 */
BOOST_AUTO_TEST_CASE(EraseTest)
{
  MARK_START;
  OPEN_LOG;
	KnowledgeBase kb(TEST_LOG);
	kb.Tell("(<= (test ?x) (test2 ?x))");
	size_t f_i = kb.Tell("(test2 1)");
	kb.Erase(Fact("(test2 1)", kb.GetSymbolTable(), TEST_LOG), f_i);
	std::list<Argument*> result;
	if((result = kb.Ask("(test ?x)")).size() != 0) MARK_FAIL;
	for(std::list<Argument*>::iterator it = result.begin();it != result.end();it++)
    delete *it;
  MARK_END;
}

/**
 * Answer question 1.
 */
BOOST_AUTO_TEST_CASE(Question1Test)
{
    MARK_START;
    OPEN_LOG;
    KIF kif(false, 0, TEST_LOG);
    kif.AddFile("data/questions/question1.kif");
    if(!kif.Parse(true)) MARK_FAIL;
    KnowledgeBase test(kif);
    list<Argument*> result = test.Ask("(engineer ?x)");
    if(result.size() != 1) MARK_FAIL;
    if(**result.begin() != Argument("(engineer bajaj)", 
                                    test.GetSymbolTable(), 
                                    true, TEST_LOG)) MARK_FAIL;
    for(std::list<Argument*>::iterator it = result.begin();it != result.end();it++)
      delete *it;
    MARK_END;
}

/**
 * Answer question 2.
 */
BOOST_AUTO_TEST_CASE(Question2Test)
{
    MARK_START;
    OPEN_LOG;
    KIF kif(false, 0, TEST_LOG);
    kif.AddFile("data/questions/question2.kif");
    if(!kif.Parse(true)) MARK_FAIL;
    KnowledgeBase test(kif);
    list<Argument*> result = test.Ask("(married cam ?x)");
    if(result.size() != 1) MARK_FAIL;
    if(**result.begin() != Argument("(married cam carrie)",
                                    test.GetSymbolTable(),
                                    true, TEST_LOG)) MARK_FAIL;
    for(std::list<Argument*>::iterator it = result.begin();it != result.end();it++)
      delete *it;
    MARK_END;
}

BOOST_AUTO_TEST_SUITE_END();
