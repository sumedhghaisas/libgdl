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
#include <libgdl/gdlreasoner/logicbase/answer.hpp>
#include <libgdl/gdlreasoner/logicbase/unify.hpp>

namespace libgdl
{

class GDL;

namespace gdlreasoner
{

class KIFFlattener;

/**
 * This class represents knowledge base which stores information and performs
 * reasoning. This implementation of Knowledge base performs back chaining for
 * answer extraction. KnowledgeBase can be initialized with KIF object.
 * The knowledge is stored in map so that the lookup is faster. The logic
 * part is handles by the class Unify.
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
 * Result will contain all the roles of 3puzzle.
 *
 * @see KIF, Unify
 */
class KnowledgeBase
{
  //! Some useful imports
  typedef logicbase::Answer Answer;
  typedef libgdl::core::VariableMap VariableMap;
  typedef core::Argument Argument;
  typedef core::Clause Clause;
  typedef core::Fact Fact;
  typedef core::SymbolTable SymbolTable;

 public:
  //! List of fact
  typedef std::list<Fact> FactList;
  //! List of Clauses
  typedef std::list<Clause> ClauseList;

  //! Map of all facts
  typedef std::map<size_t, FactList> FactMap;
  //! Map of all clauses
  typedef std::map<size_t, ClauseList> ClauseMap;

  //! Constructs an empty knowledge base
  //!
  //! \param log Logging stream
  //!
  //!
  KnowledgeBase(const Log& log = std::cout)
    : c_id(0), log(log) {}

  //! Constructs knowledge base with knowledge from KIF object
  //! The knowledge is transferred hence the KIF object is cleaned after this.
  //!
  //! \param kif KIF object
  //! \param log Logging stream
  //!
  //!
  KnowledgeBase(gdlparser::KIF& kif, const Log& log = std::cout);

  //! Constructs knowledge base from flattened knowledge
  //! Knowledge is transferred hence the KIFFlattener object is cleaned
  //!
  //! \param kiff KIFFlattener object
  //! \param log Logging stream
  //!
  //!
  KnowledgeBase(KIFFlattener& kiff, const Log& log = std::cout);

  //! Asks knowledge base given question and returns the list of answers
  //! The answers are returned as pointer to 'Argument'
  //! Delete the answer for yourself
  //! If checkForDoubles is true then duplicate answers will be removed
  //!
  //! \param question Question to be asked
  //! \param checkForDoubles To check for duplicate
  //! \return
  //!
  //!
  std::list<Argument*> Ask(const Argument& question,
                           bool checkForDoubles = true);

  //! Asks knowledge base given question and returns the list of answers
  //! The answers are returned as pointer to 'Argument'
  //! Delete the answer for yourself
  //! If checkForDoubles is true then duplicate answers will be removed
  //!
  //! \param question Question to be asked
  //! \param checkForDoubles To check for duplicate
  //! \return
  //!
  //!
  std::list<Argument*> Ask(const std::string& question,
                           bool checkForDoubles = true);

  //! Ask knowledge base given question and corresponding 'Answer' handler
  //! is returned. The answers can be extracted by using the API of 'Answer'.
  //! 'Answer' handler returns only one answer at a time. This function can be
  //! used to extract only certain number of answers at a time.
  //!
  //! \param question Question to answer
  //! \param v_map VariableMap of previous substitutions
  //! \param visited Set of visited clauses
  //! \return Answer*
  //!
  //!
  Answer* GetAnswer(const Argument& question,
                    const VariableMap& v_map,
                    const std::set<size_t>& visited) const;

  //! Returns true if the question is satisfiable
  //!
  //! \param question Question to be asked to the knowledge base
  //! \return bool
  //!
  //!
  bool IsSatisfiable(const Argument& question);

  //! Returns true if the question is satisfiable
  //!
  //! \param question Question to be asked to the knowledge base
  //! \return bool
  //!
  //!
  bool IsSatisfiable(const std::string& question);

  //! Adds given knowledge to knowledge base
  //! Returns index of this clause in the list
  //! This added clause can be delete by passing this index along with the
  //! clause to Erase function
  //!
  //! \param c Clause to be added
  //! \return size_t
  //!
  //!
  size_t Tell(const Clause& c);

  //! Adds given knowledge to knowledge base
  //! Returns index of this clause in the list
  //! This added clause can be delete by passing this index along with the
  //! clause to Erase function
  //!
  //! \param c Clause to be added
  //! \return size_t
  //!
  //!
  size_t Tell(Clause&& c);

  //! Adds given knowledge to knowledge base
  //! Returns index of this fact in the list
  //! This added fact can be delete by passing this index along with the
  //! fact to Erase function
  //!
  //! \param f Fact to be added
  //! \return size_t
  //!
  //!
  size_t Tell(const Fact& f);

  //! Adds given knowledge to knowledge base
  //! Returns index of this fact in the list
  //! This added fact can be delete by passing this index along with the
  //! fact to Erase function
  //!
  //! \param f Fact to be added
  //! \return size_t
  //!
  //!
  size_t Tell(Fact&& f);

  //! Adds given knowledge to knowledge base
  //! Returns index of this knowledge in the list
  //! This added knowledge can be delete by passing this index along with the
  //! knowledge to Erase function
  //!
  //! \param str String representation of the knowledge to be added
  //! \return size_t
  //!
  //!
  size_t Tell(const std::string& str);

  //! Erase given knowledge from knowledge base
  //! Index represents the index of the clause in appropriate ClauseVec
  //! This index is received when this clause is added to the knowledge base
  //!
  //! \param c Clause to be deleted
  //! \param index Index of the clause in the list
  //! \return bool
  //!
  //!
  bool Erase(const Clause& c, size_t index);

  //! Erase given knowledge from knowledge base
  //! Index represents the index of the fact in appropriate FactVec
  //! This index is received when this fact is added to the knowledge base
  //!
  //! \param f Fact to be deleted
  //! \param index Index of the fact in the list
  //! \return bool
  //!
  //!
  bool Erase(const Fact& f, size_t index);

  //! Returns facts associated with given signature
  //! Returns NULL if no facts are found
  //! Signature represents SymbolTable ID of the fact command
  //!
  //! \param sig Signature of the fact
  //! \return const FactList*
  //!
  //!
  const FactList* GetFacts(size_t sig) const;

  //! Returns clauses associated with given signature
  //! Returns NULL if no clauses are found
  //! Signature represents SymbolTable ID of the head command
  //!
  //! \param sig Signature of the clause
  //! \return const FactList*
  //!
  //!
  const ClauseList* GetClauses(size_t sig) const;

  //! Returns all the facts in this knowledgebase in stored form
  const FactMap& GetAllFacts() const
  {
    return m_facts;
  }
  //! Returns all the clauses in the knowledgebase in stored form
  const ClauseMap& GetAllClauses() const
  {
    return m_clauses;
  }

  //! Get the symbol table associated with knowledge base
  SymbolTable GetSymbolTable() const
  {
    return symbol_table;
  }

  //! Get-set logging stream
  Log& GetLog()
  {
    return log;
  }

 private:
  friend GDL;

  //! all the facts stored in map for faster lookup
  FactMap m_facts;
  //! all the clauses stored in map for faster lookup
  ClauseMap m_clauses;

  //! Each new clause is assigned a unique clause ID for checking recursive
  //! dependency. This variable stores the current ID to be assigned.
  size_t c_id;

  //! Symbol table for encoding
  SymbolTable symbol_table;

  //! Logging stream
  mutable Log log;
}; // class KnowledgeBase

}; // namespace gdlreasonerDGraph
}; // namespace libgdl

std::ostream& operator<<(std::ostream& stream,
                         const libgdl::gdlreasoner::KnowledgeBase& kb);

#endif // _LIBGDL_GDLREASONER_KNOWLEDGEBASE_HPP_INCLUDED
