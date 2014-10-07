/**
 * @file answer.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of Answer class.
 */
#ifndef _LIBGDL_GDLREASONER_ANSWER_HPP_INCLUDED
#define _LIBGDL_GDLREASONER_ANSWER_HPP_INCLUDED

#include <set>

#include <libgdl/core.hpp>
#include "unify.hpp"

namespace libgdl
{
namespace gdlreasoner
{
//! Forward declaration
class KnowledgeBase;

namespace logicbase
{

/**
 * Parent class for all the answer types returned by knowledge base query.
 * Answer typically implements 'next' function which in turn computes the next
 * viable result. GetVariableMap returns the current variable map
 * (map of values against variables).
 *
 * @see KnowledgeBase
 */
class Answer
{
  //! For simplicity
  typedef libgdl::core::Argument Argument;
  typedef libgdl::core::Clause Clause;
  typedef libgdl::core::Fact Fact;
  typedef libgdl::core::VariableMap VariableMap;
  typedef std::map<size_t, std::list<Fact> > FactMap;
  typedef std::map<size_t, std::list<Clause> > ClauseMap;

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

 public:
  enum Type {CLAUSE, DISTINCT, GROUND, NOT, OR};

  //! Constructs empty Answer
  //!
  //! \param question Question to be answered
  //! \param o_v_map The already existing mapping
  //! \param kb Knowledge which is responsible for this query
  //! \param v Already visited clauses
  //! \return
  //!
  //!
  Answer(const Type& t,
         const Argument& question,
         const VariableMap& o_v_map,
         const KnowledgeBase & kb,
         const std::set<size_t>& v,
         Answer* ans);


  //! Destructor
  ~Answer();

  //! Go to the next result
  //! Returns false if no next result is available
  bool next();

  /// Returns the variable mapping in which this solution is valid
  /// Returns variable map of last viable solution after next() returns false
  inline VariableMap GetVariableMap()
  {
    return Unify::DecodeSubstitutions(v_map, &question, o_v_map, to_del);
  }

  //! Returns visited clause set
  inline const std::set<size_t>& Visited() { return visited; }

 private:
  /* Common */

  //! Variable map of the current result
  VariableMap v_map;
  //! KnowledgeBase responsible for providing the answer
  const KnowledgeBase& kb;
  /// Original question (for hinting purposes)
  const Argument& question;
  //! Set of visited clauses
  std::set<size_t> visited;
  //! Default variable map
  const VariableMap o_v_map;
  //! Arguments which will be deleted at Answer deletion
  std::list<Argument*> to_del;
  //! Type
  const Type t;

  /* Clause Answer */
  //! Position of the fact for next answer to check
  size_t m_position;

  std::list<Fact>::const_iterator fit;
  std::list<Clause>::const_iterator cit;
  std::deque<SubAnswer> m_subAnswers;
  //! Length of current clause
  size_t m_clauseLength;
  //! is currently on an answer
  bool m_onAnAnswer;
  //! Answer is based on facts
  bool m_isFactAnswer;

  const std::list<Fact>* facts;
  const std::list<Clause>* clauses;

  bool toDel_f;
  bool toDel_c;

  Argument* extra;
  VariableMap e_map;
  bool isExtra;

  /* Distinct Answer */
  /// is the result returned
  bool m_returnedResult;
  /// is the current result distinct
  bool m_distinct;

  /* Ground Answer */
  //! answer to given ground question
  Answer* ans;
  //! stores if the answer is already returned
  bool isAnswerReturned;

  /* Not Answer */
  //! represents sub-answer
  Answer* m_subAnswer;
  //! is the result returned
  //bool m_returnedResult;
  //! is it valid NOT
  bool m_not;

  /* Or Answer */
  //! Current sub Answer
  Answer * m_currentAnswer;
  //! current argument in 'or'
  size_t current_arg;
}; // class Answer

}; // namespace logicbase
}; // namespace gdlreasoner
}; // namespace libgdl


#endif // _LIBGDL_GDLREASONER_ANSWER_HPP_INCLUDED
