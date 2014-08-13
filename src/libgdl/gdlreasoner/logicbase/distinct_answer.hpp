/**
 * @file distinct_answer.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of DistinctAnswer class.
 */
#ifndef _LIBGDL_GDLREASONER_DISTINCT_ANSWER_HPP_INCLUDED
#define _LIBGDL_GDLREASONER_DISTINCT_ANSWER_HPP_INCLUDED

#include <libgdl/core.hpp>
#include "answer.hpp"

namespace libgdl
{
namespace gdlreasoner
{
namespace logicbase
{

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
  DistinctAnswer (const Argument& q, const VariableMap& m,
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
}; // class DistinctAnswer

}; // namespace logicbase
}; // namespace gdlreasoner
}; // namespace libgdl


#endif // _LIBGDL_GDLREASONER_DISTINCT_ANSWER_HPP_INCLUDED
