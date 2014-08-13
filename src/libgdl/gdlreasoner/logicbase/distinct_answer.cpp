/**
 * @file distinct_answer.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation of DistinctAnswer class.
 */
#include "distinct_answer.hpp"

using namespace libgdl::gdlreasoner::logicbase;

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
