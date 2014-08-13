/**
 * @file clause_answer.hpp
 * @author Sumedh Ghaisas
 *
 * Definition of ClauseAnswer class.
 */
#ifndef _LIBGDL_GDLREASONER_CLAUSE_ANSWER_HPP_INCLUDED
#define _LIBGDL_GDLREASONER_CLAUSE_ANSWER_HPP_INCLUDED

#include <vector>
#include <list>
#include <deque>

#include "answer.hpp"

namespace libgdl
{
namespace gdlreasoner
{
namespace logicbase
{

/**
 * Version of QueryAnswer which shall used when asking against facts AND clauses.
 *
 * @see KnowledgeBase
 */
class ClauseAnswer : public Answer
{
 public:
  //! Protected constructor ( can be accessed by KnowledgeBase)
  ClauseAnswer (const Argument& question, const VariableMap& v_map,
                const KnowledgeBase & kb,
                const std::set<size_t>& v);

  //! virtual empty destructor
  ~ClauseAnswer ();

  //! go to the next result
  bool next ();

 protected:
  //! Represents the sub-answer generated when subset of total premisses are
  //! considered. Also stores information about the next premiss to be answered.
  struct SubAnswer
  {
    //! points to the next premiss to be answered
    std::vector<Argument*>::const_iterator nextPremiss;
    //! Answer* instance at this point
    Answer * partAnswer;
    //! Head map until this
    VariableMap headMap;
  }; // struct SubAnswer

  //! Position of the fact for next answer to check
  size_t m_position;

  std::list<libgdl::Fact>::const_iterator fit;
  std::list<libgdl::Clause>::const_iterator cit;
  std::deque<SubAnswer> m_subAnswers;
  //! Length of current clause
  size_t m_clauseLength;
  //! is currently on an answer
  bool m_onAnAnswer;
  //! Answer is based on facts
  bool m_isFactAnswer;

  const std::list<libgdl::Fact>* facts;
  const std::list<libgdl::Clause>* clauses;

  bool toDel_f;
  bool toDel_c;
}; // class ClauseAnswer

}; // namespace logicbase
}; // namespace gdlreasoner
}; // namespace libgdl


#endif // _LIBGDL_GDLREASONER_CLAUSE_ANSWER_HPP_INCLUDED
