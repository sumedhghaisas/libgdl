/**
 * @file or_clause_answer.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of OrClauseAnswer class.
 */
#ifndef _LIBGDL_GDLREASONER_OR_CLAUSE_ANSWER_HPP_INCLUDED
#define _LIBGDL_GDLREASONER_OR_CLAUSE_ANSWER_HPP_INCLUDED

#include <libgdl/core.hpp>
#include "answer.hpp"

namespace libgdl
{
namespace gdlreasoner
{
namespace logicbase
{

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
                 const VariableMap& m,
                 const KnowledgeBase & kb,
                 const std::set<size_t>& v)
    : Answer(question, m, kb, v), m_currentAnswer(NULL), current_arg(0) {}


  //! destructor
  ~OrClauseAnswer() { delete m_currentAnswer; }

  //! go te next result
  bool next ();

 protected:
  //! Current sub Answer
  Answer * m_currentAnswer;
  //! current argument in 'or'
  size_t current_arg;
}; // class OrClauseAnswer

}; // namespace logicbase
}; // namespace gdlreasoner
}; // namespace libgdl


#endif // _LIBGDL_GDLREASONER_OR_CLAUSE_ANSWER_HPP_INCLUDED
