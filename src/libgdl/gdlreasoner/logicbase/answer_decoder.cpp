/**
 * @file answer_decoder.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation of Answer Decoder class.
 */
#include "answer_decoder.hpp"

using namespace libgdl::gdlreasoner::logicbase;

//! TODO : add cache system here
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
      // we can give a hint to the knowledge base that we found a question with
      // only one solution
      // m_kb.onlyOneHint (m_originalQuestion, m_questionMap);
    }
    else
    {
      //m_kb.listHint(m_originalQuestion, maps);
    }
  }

  return result;
}
