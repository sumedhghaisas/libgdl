/**
 * @file not_answer.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation of NotAnswer class.
 */
#include "not_answer.hpp"

#include <libgdl/gdlreasoner/knowledgebase.hpp>

using namespace libgdl::gdlreasoner::logicbase;

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
