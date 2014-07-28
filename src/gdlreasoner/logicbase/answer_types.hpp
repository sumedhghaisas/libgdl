/**
 * @file answertypes.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of Answer class.
 */
#ifndef ANSWERTYPES_HPP_INCLUDED
#define ANSWERTYPES_HPP_INCLUDED

#include <string>
#include <map>
#include <iostream>
#include <list>
#include <deque>

/// import primary KnowledgeBase types
#include <gdlparser/data_types.hpp>
/// import unification module
#include <gdlreasoner/logicbase/unify.hpp>

#include <gdlparser/util/setop.hpp>

namespace gdlreasoner
{
/// forward declaration for class KnowledgeBase
class KnowledgeBase;

namespace logicbase
{

/**
 * Parent class for all the answers types returned by knowledge base query.
 * Answer typically implements next which in turn computes the next viable result.
 * getQuestionMap returns the current variable map (map of values against variables).
 *
 * @see KnowledgeBase
 */
class Answer
{
public:
    /// A map from variables to values
    typedef gdlparser::Argument Argument;
    typedef Unify::VariableSet VariableSet;

    Answer(const Argument& question,
           const VariableSet& v_set,
           const VariableSet& h_set,
           const KnowledgeBase & kb,
           const std::set<size_t>& v)
        : kb (kb), question (question), visited(v), o_v_set(v_set), h_set(h_set) {}

    virtual ~Answer() {}

    /// go to the next result
    /// returns false if no next result is available
    virtual bool next () = 0;
    /// returns the variable mapping in which this solution is valid
    /// returns variable map of last viable solution after next() returns false
    inline VariableSet GetVariableSet()
    {
        return gdlparser::util::setop::setUnion(v_set, h_set);
    }

    inline void DecodeVariables()
    {
        Unify::DecodeSubstitutions(o_v_set, v_set);
        v_set = o_v_set;
    }

    const std::set<size_t>& Visited()
    {
        return visited;
    }

protected:
    //! variable map of the current result
    VariableSet v_set;
    //! KnowledgeBase
    const KnowledgeBase& kb;
    /// Original question (for hinting purposes)s
    const Argument& question;
    //! set of visited clauses
    std::set<size_t> visited;
    //! default variable map
    const VariableSet o_v_set;
    //! history set
    VariableSet h_set;
};

/**
 * Renames variables in results from intern back into their real ones. Used
 * As a decorator for answers which are directed to questions with intern variables only.
 *
 * @see KnowledgeBase
 */
class AnswerDecoder : public Answer
{
public:
    /// Constructs an Answer Decoder from given answer and question
    AnswerDecoder (Answer* ans, const Argument& question, const VariableSet& v_set,
                   const VariableSet& h_set, const KnowledgeBase & kb)
        : Answer(question, v_set, h_set, kb, std::set<size_t>()) , m_wasTrueTimes(0)
    {
        m_answer = ans;
    }

    /// Deletes question encoder along with the hidden answer
    ~AnswerDecoder ()
    {
        delete m_answer;
    }

    /// Go to the next result
    bool next ();

private:
    /// answer to decode
    Answer* m_answer;
    /// How often it was true (for caching purposes)
    int m_wasTrueTimes;
};

/**
 * A version of QueryAnswer which shall used when asking against facts AND clauses.
 *
 * @see KnowledgeBase
 */
class ClauseAnswer : public Answer
{
public:
    //! Protected constructor ( can be accessed by KnowledgeBase)
    ClauseAnswer (const Argument& question, const VariableSet& v_set, const VariableSet& h_set,
                  const KnowledgeBase & kb,
                  const std::set<size_t>& v);

    //! virtual empty destructor
    ~ClauseAnswer ();

    //! go to the next result
    bool next ();

protected:

    //! Represents the sub-answer generated when subset of total premisses are considered.
    //! Also stores information about the next premiss to be answered.
    struct SubAnswer
    {
        //! points to the next premiss to be answered
        std::vector<Argument*>::const_iterator nextPremiss;
        //! Answer* instance at this point
        Answer * partAnswer;
        //! Head map until this
        VariableSet headMap;
    };

    //! Position of the fact for next answer to check
    size_t m_position;

    std::list<gdlparser::Fact>::const_iterator fit;
    std::list<gdlparser::Clause>::const_iterator cit;
    std::deque<SubAnswer> m_subAnswers;
    //! Length of current clause
    size_t m_clauseLength;
    //! is currently on an answer
    bool m_onAnAnswer;
    //! Answer is based on facts
    bool m_isFactAnswer;

    const std::list<gdlparser::Fact>* facts;
    const std::list<gdlparser::Clause>* clauses;

    bool toDel_f;
    bool toDel_c;
};

/**
 * Represents answer to question with OR.
 * This basically calls ask on each sub-premiss in the OR premiss and by using set
 * operation construct the overall answer.
 *
 * @see KnowledgeBase
 */
class OrClauseAnswer : public Answer
{
public:
    //! Constructs the object with OR question and given knowledge base
    OrClauseAnswer(const Argument& question,
                   const VariableSet& m,
                   const VariableSet& h_set,
                   const KnowledgeBase & kb,
                   const std::set<size_t>& v)
        : Answer(question, m, h_set, kb, v), m_currentAnswer(NULL), current_arg(0) {}


    //! destructor
    ~OrClauseAnswer()
    {
        delete m_currentAnswer;
    }

    //! go te next result
    bool next ();


protected:
    //! Current sub Answer
    Answer * m_currentAnswer;
    //! current argument in 'or'
    size_t current_arg;
};

/**
 * Represents simple distinct comparison answering.
 * All variables must have been bound.
 *
 * @see KnowledgeBase
 */
class DistinctAnswer : public Answer
{
public:
    /// Construct DistinctAnswer from question and given knowledge base
    DistinctAnswer (const Argument& q, const VariableSet& m, const VariableSet& h_set,
                    const KnowledgeBase & kb,
                    const std::set<size_t>& v);

    ~DistinctAnswer() {}

    /// go to next result
    bool next ();

protected:
    /// is the result returned
    bool m_returnedResult;
    /// is the current result distinct
    bool m_distinct;
};

/**
 * Represents simple NOT answering algorithm.
 * All variable inside NOT have to be bounded.
 *
 * @see KnowledgeBase
 */
class NotAnswer : public Answer
{
public:
    //! Constructs NotAnswer from question and given knowledge base
    NotAnswer (const Argument& q, const VariableSet& m, const VariableSet& h_set,
               const KnowledgeBase & kb,
               const std::set<size_t>& v);
    //! destructor
    ~NotAnswer()
    {
        delete m_subAnswer;
    }

    //! Go to next result
    bool next ();

protected:
    //! represents sub-answer
    Answer* m_subAnswer;
    //! is the result returned
    bool m_returnedResult;
    //! is it valid NOT
    bool m_not;
};

/**
 * Represents Answer for questions which are ground.
 * Return true if question is satisfiable.
 *
 * @see KnowledgeBase
 */
class GroundQuestionAnswer : public Answer
{
public:
    //! constructs ground answer
    GroundQuestionAnswer(Answer* ans, const Argument& q, const VariableSet& m,
                         const VariableSet& h_set,
                         const KnowledgeBase& kb,
                         const std::set<size_t>& v)
            : Answer(q, m, h_set, kb, v), ans(ans) { isAnswerReturned = false; }

    //! destructor
    ~GroundQuestionAnswer() { delete ans; }

    //! Go to next result
    bool next();

private:
    //! answer to given ground question
    Answer* ans;
    //! stores if the answer is already returned
    bool isAnswerReturned;
};

}
} // namespave gdlreasoner


#endif // _EPICURUS_LOGICBASE_ANSWERTYPES_HPP_INCLUDED
