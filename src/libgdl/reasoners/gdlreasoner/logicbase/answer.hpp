/**
 * @file answer.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of Answer class.
 */
#ifndef _LIBGDL_GDLREASONER_ANSWER_HPP_INCLUDED
#define _LIBGDL_GDLREASONER_ANSWER_HPP_INCLUDED

#include <set>
#include <deque>

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
 * This class implements lazy evaluation of answers to the given question.
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

  /**
   * Represents the sub-answer generated when subset of total premisses are
   * considered. Also stores information about the next premiss to be answered.
   */
  struct SubAnswer
  {
    //! Points to the next premiss to be answered
    std::vector<Argument*>::const_iterator nextPremiss;
    //! Answer* instance at this point
    Answer * partAnswer;
    //! Head map until this
    VariableMap headMap;
  }; // struct SubAnswer

 public:
  //! Type of the this answering
  enum Type {CLAUSE, DISTINCT, GROUND, NOT, OR, CACHE, DECODER};

  //! Constructs empty Answer
  //!
  //! \param t Type of this Answer
  //! \param question Question to be answered
  //! \param o_v_map The already existing mapping
  //! \param kb Knowledge which is responsible for this query
  //! \param v Already visited clauses
  //! \param ans External answer to evaluate(for ground question answering)
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

  VariableMap AdjustToQuestion(const Argument* arg, const Argument* adj_to,
                             const VariableMap& v_map);

  /// Returns the variable mapping in which this solution is valid
  /// Returns variable map of last viable solution after next() returns false
  VariableMap GetVariableMap();
//  {
//    if(t == CACHE)
//    {
//      auto temp_it = maps_it;
//      temp_it--;
//      VariableMap v_map = AdjustToQuestion(sub_struct, &question, *(temp_it));
//      Argument* temp = Unify::GetSubstitutedArgument(&question, v_map);
//
//      core::SymbolDecodeStream sds(kb.GetSymbolTable());
//      sds << *temp << std::endl;
//
//      return v_map;
//    }
//    else if(t == DECODER)
//    {
//      return to_ret;
//    }
//
//    return Unify::DecodeSubstitutions(v_map, &question, o_v_map, to_del);
//  }

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
  //! FactList iterator
  std::list<Fact>::const_iterator fit;
  //! ClauseList iterator
  std::list<Clause>::const_iterator cit;
  //! Subanswers to evaluate
  std::deque<SubAnswer> m_subAnswers;
  //! Is currently on an answer
  bool m_onAnAnswer;
  //! Answer is based on facts
  bool m_isFactAnswer;
  //! Facts to consider while answering
  const std::list<Fact>* facts;
  //! Clauses to consider while answering
  const std::list<Clause>* clauses;

  //! If the list of facts needs to be deleted
  bool toDel_f;
  //! If the list of clauses needs to be deleted
  bool toDel_c;

  //! Generated argument for recursive query satisfaction
  Argument* extra;
  //! Mapping which maps the given question to generated question
  VariableMap e_map;
  //! Is generated question used for answering
  bool isExtra;

  /* Distinct Answer */
  /// Is the result returned
  bool m_returnedResult;
  /// Is the current result distinct
  bool m_distinct;

  /* Ground Answer */
  //! Answer to given ground question
  Answer* ans;
  //! Stores if the answer is already returned
  bool isAnswerReturned;

  /* Not Answer */
  //! Represents sub-answer
  Answer* m_subAnswer;
  //! Is it valid NOT
  bool m_not;

  /* Or Answer */
  //! Current sub Answer
  Answer * m_currentAnswer;
  //! Current argument in 'or'
  size_t current_arg;

  /* Cache answer */
  const std::list<VariableMap>* maps;
  const Argument* sub_struct;
  std::list<VariableMap>::const_iterator maps_it;

  /* Decoder answer */
  Answer* to_dec;
  VariableMap to_ret;
  bool to_cache;
  std::list<VariableMap>* cache_maps;
  Argument* cache_q;
  VariableMap conv_map;
}; // class Answer

}; // namespace logicbase
}; // namespace gdlreasoner
}; // namespace libgdl


#endif // _LIBGDL_GDLREASONER_ANSWER_HPP_INCLUDED
