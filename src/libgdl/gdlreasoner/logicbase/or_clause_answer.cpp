/**
 * @file or_clause_answer.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation of OrClauseAnswer class.
 */
#include "or_clause_answer.hpp"

#include <libgdl/gdlreasoner/knowledgebase.hpp>

using namespace libgdl::gdlreasoner::logicbase;

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
