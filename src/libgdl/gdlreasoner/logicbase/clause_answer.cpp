/**
 * @file clause_answer.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation of ClauseAnswer class.
 */
#include "clause_answer.hpp"

#include <sstream>
#include <map>

#include <libgdl/gdlreasoner/knowledgebase.hpp>

using namespace std;
using namespace libgdl::gdlreasoner::logicbase;

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

  map<string, KnowledgeBase::FactVec>::const_iterator it1 =
                                                    kb.GetAllFacts().find(sig);
  if(it1 != kb.GetAllFacts().end())
  {
    toDel_f = false;
    facts = &(it1->second);
  }
  else facts = new KnowledgeBase::FactVec();

  map<std::string, KnowledgeBase::ClauseVec>::const_iterator it2 =
                                                  kb.GetAllClauses().find(sig);
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
  for (deque<SubAnswer>::iterator i = m_subAnswers.begin();
                                                    i!=m_subAnswers.end(); i++)
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

  // if position exceeds sum of all facts and clauses then no next result can
  // be found
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
    const Fact& fact = *fit;

    v_map = o_v_map;

    if (Unify::mgu(question, *(fact.arg), v_map))
    {
      m_position++;
      fit++;
      m_isFactAnswer = true;
      return true;
    }
    fit++;
  }

  // after facts, search in clauses
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
          nextTail.partAnswer = kb.GetAnswer(**nextTail.nextPremiss,
                                             nextTail.headMap,
                                             tail.partAnswer->Visited());
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
