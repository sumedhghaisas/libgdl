/**
 * @file answer_decoder.hpp
 * @author Sumedh Ghaisas
 *
 * Definition of Answer class.
 */
#ifndef _LIBGDL_GDLREASONER_ANSWER_DECODER_HPP_INCLUDED
#define _LIBGDL_GDLREASONER_ANSWER_DECODER_HPP_INCLUDED

#include "answer.hpp"

namespace libgdl
{
namespace gdlreasoner
{
namespace logicbase
{


class AnswerDecoder : public Answer
{
public:
  /// Constructs an Answer Decoder from given answer and question
  AnswerDecoder (Answer* ans, const Argument& question, const VariableMap& v_map,
                 const KnowledgeBase & kb)
    : Answer(question, v_map, kb, std::set<size_t>()) , m_wasTrueTimes(0)
  {
    m_answer = ans;
  }

  /// Deletes question encoder along with the hidden answer
  ~AnswerDecoder () { delete m_answer; }

  inline VariableMap GetVariableMap() { return v_map; }

  inline const std::set<size_t>& Visited() { return m_answer->Visited(); }

  /// Go to the next result
  bool next ();

 private:
  /// answer to decode
  Answer* m_answer;
  /// How often it was true (for caching purposes)
  int m_wasTrueTimes;
}; // class AnswerDecoder

}; // namespace logicbase
}; // namespace gdlreasoner
}; // namespace libgdl


#endif // _LIBGDL_GDLREASONER_ANSWER_DECODER_HPP_INCLUDED
