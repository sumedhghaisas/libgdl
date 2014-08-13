/**
 * @file ground_question_answer.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of GroundQuestionAnswer class.
 */
#ifndef _LIBGDL_GROUNDQUESTIONANSWER_GROUND_QUESTION_ANSWER_HPP_INCLUDED
#define _LIBGDL_GROUNDQUESTIONANSWER_GROUND_QUESTION_ANSWER_HPP_INCLUDED

#include <libgdl/core.hpp>
#include "answer.hpp"

namespace libgdl
{
namespace gdlreasoner
{
namespace logicbase
{

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
  GroundQuestionAnswer(Answer* ans, const Argument& q, const VariableMap& m,
                       const KnowledgeBase& kb,
                       const std::set<size_t>& v)
    : Answer(q, m, kb, v), ans(ans) { isAnswerReturned = false; }

  //! destructor
  ~GroundQuestionAnswer() { delete ans; }

  //! Go to next result
  bool next();

 private:
  //! answer to given ground question
  Answer* ans;
  //! stores if the answer is already returned
  bool isAnswerReturned;
}; // class GroundQuestionAnswer

}; // namespace logicbase
}; // namespace gdlreasoner
}; // namespace libgdl

#endif // _LIBGDL_GROUNDQUESTIONANSWER_GROUND_QUESTION_ANSWER_HPP_INCLUDED
