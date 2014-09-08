/**
 * @file knowledgebase.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of KnowledgeBase class.
 */
#ifndef _LIBGDL_GDLREASONER_KNOWLEDGEBASE_HPP_INCLUDED
#define _LIBGDL_GDLREASONER_KNOWLEDGEBASE_HPP_INCLUDED

#include <map>
#include <vector>
#include <string>
#include <list>

#include <libgdl/core.hpp>
#include <libgdl/gdlparser/kif.hpp>
#include <libgdl/gdlreasoner/logicbase/answer_types.hpp>
#include <libgdl/gdlreasoner/logicbase/unify.hpp>

namespace libgdl
{

class GDL;

namespace gdlreasoner
{
/**
 * This class represents knowledge base which stores information and performs
 * reasoning. Knowledge base performs back chaining for answer extraction.
 * KnowledgeBase can be initialized with KIF object.
 * Sample usage -
 *
 * @code
 * KIF kif;
 * kif.AddFile("3puzzle.kif");
 * kif.Parse();
 * KnowledgeBase kb(kif);
 * list<Argument*> result = kb.Ask(Argument("(role ?x)"));
 * @endcode
 *
 * @see KIF
 */
class KnowledgeBase
{
  //! Some useful typedefs
  typedef logicbase::Unify::VariableMap VariableMap;
  typedef logicbase::Answer Answer;

 public:
  typedef std::list<Fact> FactList;
  typedef std::list<Clause> ClauseList;

  //! data types to store knowledge
  //! improves access time
  typedef std::map<size_t, FactList> FactMap;
  typedef std::map<size_t, ClauseList> ClauseMap;

  //! constructs empty knowledge base
  KnowledgeBase() : c_id(0), symbol_table(new SymbolTable()) {}
  //! construct knowledge base with knowledge from KIF object
  KnowledgeBase(gdlparser::KIF& kif);

  //! Asks knowledge base given question
  //! returns list of answers
  std::list<Argument*> Ask(const Argument& question,
                           bool checkForDoubles = true) const;

  //! get the answer to the question than substitutions
  Answer* GetAnswer(const Argument& question,
                    const VariableMap& v_map,
                    const std::set<size_t>& visited) const;

  //! returns if the question is satisfiable
  bool IsSatisfiable(const Argument& question) const;

  //! adds given knowledge to knowledge base
  //! returns index of this clause in the list
  //! this added clause can be delete by passing this index along with the clause
  size_t Tell(const Clause& c);
  size_t Tell(Clause&& c);

  //! adds given knowledge to knowledge base
  //! returns index of this fact in the list
  //! this added fact can be delete by passing this index along with the clause
  size_t Tell(const Fact& f);
  size_t Tell(Fact&& f);

  size_t Tell(const std::string& str);

  //! erase given knowledge from knowledge base
  //! index represents the index of the clause in appropriate ClauseVec
  bool Erase(const Clause& c, size_t index);

  //! erase given knowledge from knowledge base
  //! index represents the index of the fact in appropriate ClauseVec
  bool Erase(const Fact& f, size_t index);

  //! returns clauses associated with given signature
  //! returns NULL if no clauses are found
  //! signature is represented by head relation name followed by its arity
  //! separated by '/'
  const FactList* GetFacts(size_t sig) const;
  const ClauseList* GetClauses(size_t sig) const;

  //! returns all the facts in this knowledgebase in stored form
  const FactMap& GetAllFacts() const { return m_facts; }
  //! returns all the clauses in the knowledgebase in stored form
  const ClauseMap& GetAllClauses() const { return m_clauses; }

  const SymbolTable* GetSymbolTable() const
  {
    return symbol_table;
  }
  SymbolTable*& GetSymbolTable()
  {
    return symbol_table;
  }

  Log& GetLog() { return log; }

 private:
  friend GDL;
  //! all the facts stored in map for faster lookup
  FactMap m_facts;
  //! all the clauses stored in map for faster lookup
  ClauseMap m_clauses;

  //! clause id to assign to new clauses
  size_t c_id;

  SymbolTable* symbol_table;

  //! logging stream
  mutable Log log;
}; // CLASS KNOWLEDGEBASE

}; // namespace gdlreasoner
}; // namespace libgdl

std::ostream& operator<<(std::ostream& stream,
                         const libgdl::gdlreasoner::KnowledgeBase& kb);

#endif // _LIBGDL_GDLREASONER_KNOWLEDGEBASE_HPP_INCLUDED
