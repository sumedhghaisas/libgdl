/**
 * @file answertypes.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation of Answer class.
 */
#include "answer_types.hpp"

#include <gdlreasoner/knowledgebase.hpp>

using namespace gdlreasoner;
using namespace gdlreasoner::logicbase;
using namespace gdlparser;

bool AnswerDecoder::next()
{
    // go to the next result of the answer represented by this answer decoder
    bool result = m_answer->next();
    // update variable map if result is valid

    v_map = m_answer->GetVariableMap();

    // remember total valid results
    if (result) m_wasTrueTimes++;
    // store hints in the knowledge base about the current question
    if (!result)
    {
        // if no result is found
        if (m_wasTrueTimes == 0)
        {
            // give a hint to the knowledge base that we found a unforfillable question
            //m_kb.failedHint (m_originalQuestion);
        }
        // if only one result is found
        else if (m_wasTrueTimes == 1)
        {
            // we can give a hint to the knowledge base that we found a question with only one solution
            //m_kb.onlyOneHint (m_originalQuestion, m_questionMap);
        }
        else
        {
            //m_kb.listHint(m_originalQuestion, maps);
        }
    }

    return result;
}

ClauseAnswer::ClauseAnswer(const Argument& question,
                           const VariableMap& m,
                           const KnowledgeBase & kb,
                           const std::set<size_t>& v)
    : Answer(question, m, kb, v)
{
    m_position = 0;
    m_onAnAnswer = false;
    toDel_f = true;
    toDel_c = true;

    std::stringstream stream;
    stream << question.args.size();
    std::string sig = question.val + "/" + stream.str();

    std::map<std::string, KnowledgeBase::FactVec>::const_iterator it1 = kb.GetAllFacts().find(sig);
    if(it1 != kb.GetAllFacts().end())
    {
        toDel_f = false;
        facts = &(it1->second);
    }
    else facts = new KnowledgeBase::FactVec();

    std::map<std::string, KnowledgeBase::ClauseVec>::const_iterator it2 = kb.GetAllClauses().find(sig);
    if(it2 != kb.GetAllClauses().end())
    {
        clauses = &(it2->second);
        toDel_c = false;
    }
    else clauses = new KnowledgeBase::ClauseVec();
}

ClauseAnswer::~ClauseAnswer ()
{
    // if stack is yet net empty we have to delete some subresults
    for (std::deque<SubAnswer>::iterator i = m_subAnswers.begin(); i!=m_subAnswers.end(); i++)
    {
        delete i->partAnswer;
    }

    if(toDel_f) delete facts;
    if(toDel_c) delete clauses;
}

bool ClauseAnswer::next ()
{
    // flag that this object is now on an answer
    m_onAnAnswer = true;

    //if (m_kb.Debug() > 1) Log::Debug << LOGID << util::formatDepth (m_depth) << "Checking question " << m_question << " with signature " << m_sig << " against " << facts.size() << " facts and " << clauses.size() << " clauses" << std::endl;

    // if position exceeds sum of all facts and clauses then no next result can be found
    if (m_position >= facts->size() + clauses->size())
    {
        // set flag that this object is not on answer anymore
        m_onAnAnswer = false;
        return false;
    }

    // loop from last fact position
    for (; m_position < facts->size(); m_position++)
    {
        if(m_position == 0) fit = facts->begin();
        //if (m_kb.Debug() > 1) Log::Debug << LOGID << util::formatDepth (m_depth) << "Checking against fact " << fact << std::endl;
        const Fact& fact = *fit;

        v_map = o_v_map;

        if (Unify::mgu(question, fact.arg, v_map))
        {
            m_position++;
            fit++;
            m_isFactAnswer = true;
            //Unify::PrintVariableMap(std::cout, v_map);
            return true;
        }
        fit++;
    }

    // after facts search in clauses
    while (m_position < facts->size() + clauses->size())
    {
        if(m_position == facts->size()) cit = clauses->begin();
        // the clause against we find solutions
        const Clause& clause = *cit;

        // check for infinite recursion
        if(visited.find(clause.id) != visited.end())
        {
            m_position++; cit++; continue;
        }

        if (m_subAnswers.empty())
        {
            // checking the head of the clause
            v_map = o_v_map;
            if (Unify::mgu(question, *(clause.head), v_map))
            {
              //Unify::PrintVariableMap(std::cout, v_map);
                if (clause.premisses.empty())
                {
                    // clauses having no tail
                    // This is very seldom; but possible, that there is no tail in the clause
                    // e.g. in example maze.kif
                    m_position++;
                    cit++;
                    return true;
                }
                // putting sub answers onto the stack
                SubAnswer sanswer;
                sanswer.nextPremiss = clause.premisses.begin();
                sanswer.headMap = v_map;
                std::vector<Argument*>::const_iterator& current = sanswer.nextPremiss;
                std::set<size_t>* t_visited = new std::set<size_t>(visited);
                t_visited->insert(clause.id);

                sanswer.partAnswer = kb.GetAnswer(**current, sanswer.headMap, *t_visited);
                delete t_visited;
                sanswer.nextPremiss++;
                m_subAnswers.push_back(sanswer);
            }
        }

        while (!m_subAnswers.empty())
        {
            SubAnswer& tail = m_subAnswers.back();
            if (tail.partAnswer->next())
            {
                // Ok lets go further
                if (tail.nextPremiss != clause.premisses.end())
                {
                    SubAnswer nextTail;
                    nextTail.nextPremiss = tail.nextPremiss;
                    nextTail.headMap = tail.partAnswer->GetVariableMap();
                    //Unify::PrintVariableMap(std::cout, nextTail.headMap);
                    nextTail.partAnswer = kb.GetAnswer(**nextTail.nextPremiss, nextTail.headMap, tail.partAnswer->Visited());
                    nextTail.nextPremiss++; // shall show onto the next
                    m_subAnswers.push_back (nextTail);
                }
                else
                {
                    // final solution
                    v_map = tail.partAnswer->GetVariableMap();
                    //Unify::PrintVariableMap(std::cout, v_map);
                    return true;
                }
            }
            else
            {
                delete tail.partAnswer;
                m_subAnswers.pop_back ();
            }
        }
        m_position++;
        cit++;
    }

    m_onAnAnswer = false;
    return false;
}

bool OrClauseAnswer::next()
{
    // if current answer has next then go to the same and return true
    if (m_currentAnswer != NULL)
    {
        if (m_currentAnswer->next())
        {
            v_map = m_currentAnswer->GetVariableMap();
            return true;
        }
    }

    while (current_arg++ != question.args.size())
    {
        // delete the previous answer
        delete m_currentAnswer;
        // query knowledge base for answer to this token
        m_currentAnswer = kb.GetAnswer(*question.args[current_arg - 1], o_v_map, visited);
        // if there exists a valid result update the variable map with the same
        if (m_currentAnswer->next())
        {
            v_map = m_currentAnswer->GetVariableMap();
            //std::cout << m_questionMap << std::endl;
            return true;
        }
    }

    return false;
}

DistinctAnswer::DistinctAnswer(const Argument& q,
                               const VariableMap& m,
                               const KnowledgeBase & kb,
                               const std::set<size_t>& v)
    : Answer(q, m, kb, v)
{
    v_map = m;
    m_distinct = !Unify::EquateWithSubstitution(*q.args[0], *q.args[1], v_map);
    m_returnedResult = false;
}

bool DistinctAnswer::next ()
{
    if (m_returnedResult) return false;
    m_returnedResult = true;
    return m_distinct;
}

NotAnswer::NotAnswer (const Argument& q, const VariableMap& m,
                      const KnowledgeBase& kb,
                      const std::set<size_t>& v)
    :  Answer(q, m, kb, v)
{
    v_map = m;
    // TODO : here encode substitution can be removed...
    m_subAnswer = kb.GetAnswer(*q.args[0], v_map, v);
    m_returnedResult = false;
}

bool NotAnswer::next()
{
    if (m_returnedResult) return false;

    m_not = !m_subAnswer->next();
    m_returnedResult = true;
    return m_not;
}

bool GroundQuestionAnswer::next()
{
    // return true if any result found for question
    v_map = o_v_map;
    if(isAnswerReturned == false)
    {
        isAnswerReturned = true;
        bool out = ans->next();
        return out;
    }
    else return false;
}

