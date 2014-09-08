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

class KnowledgeBase;

namespace logicbase
{

/**
 * Parent class for all the answers types returned by knowledge base query.
 * Answer typically implements next which in turn computes the next viable result.
 * getQuestionMap returns the current variable map (map of values against variables).
 *
 * @see KnowledgeBase
 */
class Answer
{
 protected:
  typedef libgdl::core::Argument Argument;
  typedef libgdl::core::Clause Clause;
  typedef libgdl::core::Fact Fact;
  typedef libgdl::core::VariableMap VariableMap;
  typedef std::map<size_t, std::list<Fact> > FactMap;
  typedef std::map<size_t, std::list<Clause> > ClauseMap;

 public:
  Answer(const Argument& question,
         const VariableMap& o_v_map,
         const KnowledgeBase & kb,
         const std::set<size_t>& v)
      : kb (kb), question (question), visited(v), o_v_map(o_v_map) {}

  virtual ~Answer()
  {
    for(std::list<Argument*>::iterator it = to_del.begin();it != to_del.end();it++)
      delete *it;
  }

  //! go to the next result
  //! returns false if no next result is available
  virtual bool next () = 0;

  /// returns the variable mapping in which this solution is valid
  /// returns variable map of last viable solution after next() returns false
  inline virtual VariableMap GetVariableMap()
  {
    return Unify::DecodeSubstitutions(v_map, &question, o_v_map, to_del);
  }

  virtual inline const std::set<size_t>& Visited() { return visited; }

 protected:
  //! variable map of the current result
  VariableMap v_map;
  //! KnowledgeBase
  const KnowledgeBase& kb;
  /// Original question (for hinting purposes)s
  const Argument& question;
  //! set of visited clauses
  std::set<size_t> visited;
  //! default variable map
  const VariableMap o_v_map;

  std::list<Argument*> to_del;
};

}; // namespace logicbase
}; // namespace gdlreasoner
}; // namespace libgdl


#endif // _LIBGDL_GDLREASONER_ANSWER_HPP_INCLUDED
