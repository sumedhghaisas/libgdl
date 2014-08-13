/**
 * @file ground_question_answer.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation of GroundQuestionAnswer class.
 */
#include "ground_question_answer.hpp"

using namespace libgdl::gdlreasoner::logicbase;

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
