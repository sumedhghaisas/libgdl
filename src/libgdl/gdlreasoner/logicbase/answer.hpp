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
 protected:
   //! For simplicity
  typedef libgdl::core::Argument Argument;
  typedef libgdl::core::Clause Clause;
  typedef libgdl::core::Fact Fact;
  typedef libgdl::core::VariableMap VariableMap;
  typedef std::map<size_t, std::list<Fact> > FactMap;
  typedef std::map<size_t, std::list<Clause> > ClauseMap;

 public:
  //! Constructs empty Answer
  //!
  //! \param question Question to be answered
  //! \param o_v_map The already existing mapping
  //! \param kb Knowledge which is responsible for this query
  //! \param v Already visited clauses
  //! \return
  //!
  //!
  Answer(const Argument& question,
         const VariableMap& o_v_map,
         const KnowledgeBase & kb,
         const std::set<size_t>& v)
      : kb (kb), question (question), visited(v), o_v_map(o_v_map) {}

  //! virtual destructor
  virtual ~Answer()
  {
    for(std::list<Argument*>::iterator it = to_del.begin();it != to_del.end();
                                                                          it++)
      delete *it;
  }

  //! Go to the next result
  //! Returns false if no next result is available
  virtual bool next () = 0;

  /// Returns the variable mapping in which this solution is valid
  /// Returns variable map of last viable solution after next() returns false
  inline virtual VariableMap GetVariableMap()
  {
    return Unify::DecodeSubstitutions(v_map, &question, o_v_map, to_del);
  }

  //! Returns visited clause set
  virtual inline const std::set<size_t>& Visited() { return visited; }

 protected:
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
}; // class Answer

}; // namespace logicbase
}; // namespace gdlreasoner
}; // namespace libgdl


#endif // _LIBGDL_GDLREASONER_ANSWER_HPP_INCLUDED
