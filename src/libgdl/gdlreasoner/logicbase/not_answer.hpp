/**
 * @file not_answer.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of NotAnswer class.
 */
#ifndef _LIBGDL_GDLREASONER_NOT_ANSWER_HPP_INCLUDED
#define _LIBGDL_GDLREASONER_NOT_ANSWER_HPP_INCLUDED

#include <libgdl/core.hpp>
#include "answer.hpp"

namespace libgdl
{
namespace gdlreasoner
{
namespace logicbase
{

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
  NotAnswer (const Argument& q, const VariableMap& m,
             const KnowledgeBase & kb,
             const std::set<size_t>& v);
  //! destructor
  ~NotAnswer() { delete m_subAnswer; }

  //! Go to next result
  bool next ();

 protected:
  //! represents sub-answer
  Answer* m_subAnswer;
  //! is the result returned
  bool m_returnedResult;
  //! is it valid NOT
  bool m_not;
}; // class NotAnswer

}; // namespace logicbase
}; // namespace gdlreasoner
}; // namespace libgdl

#endif // _LIBGDL_GDLREASONER_NOT_ANSWER_HPP_INCLUDED
