/**
 * @file gdlreasoner.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of GDLReasoner.
 */
#ifndef _LIBGDL_REASONERS_GDLREASONER_HPP_INCLUDED
#define _LIBGDL_REASONERS_GDLREASONER_HPP_INCLUDED

#include <list>

#include <libgdl/core.hpp>
#include <libgdl/reasoners/gdlreasoner/kif_flattener.hpp>
#include <libgdl/gdlparser/kif.hpp>

#include "knowledgebase.hpp"

namespace libgdl
{
namespace gdlreasoner
{

/**
 * This class acts as the connection between KnowledgeBase and GDL abstraction.
 * This class implement functions by which GDL(State Machine Abstraction) can
 * use KnowledgeBase for its operations.
 *
 * @see KnowledgeBase, GDL
 */
class GDLReasoner
{
  //! For simplicity
  typedef core::Argument Argument;
  typedef core::Fact Fact;
  typedef core::SymbolTable SymbolTable;

 public:
  //! Default constructible for template substitution
  //!
  //! \return GDLReasoner():
  //!
  //!
  GDLReasoner()
    : init(NULL) {}

  //! Constructs GDLReasoner from classes which are supported by KnowledgeBase
  //!
  //! \tparam T Class of the object passed
  //! \param obj Object to initialize from
  //! \param log Logging stream
  //! \return
  //!
  //!
  template<class T>
  GDLReasoner(T& obj,
              const Log& log = GLOBAL_LOG);

  //! Copy constructor
  //!
  //! \param gdlr Object to copy fom
  //!
  //!
  GDLReasoner(const GDLReasoner& gdlr);

  //! Move constructor
  //!
  //! \param gdlr Object to move from
  //!
  //!
  GDLReasoner(GDLReasoner&& gdlr);

  //! Destructor
  ~GDLReasoner();

  //! Swap function
  //!
  //! \param gdlr1 Object1
  //! \param gdlr2 Object2
  //! \return void
  //!
  //!
  friend void swap(GDLReasoner& gdlr1, GDLReasoner& gdlr2)
  {
    using std::swap;

    swap(gdlr1.init, gdlr2.init);
    swap(gdlr1.roles, gdlr2.roles);
    swap(gdlr1.base_rules, gdlr2.base_rules);
  }

  //! Copy-Assignment operator
  //!
  //! \param gdlr Object to copy from
  //! \return GDLReasoner&
  //!
  //!
  GDLReasoner& operator=(const GDLReasoner& gdlr);

  //! Move assignment operator
  //!
  //! \param gdlr Object to move from
  //! \return GDLReasoner&
  //!
  //!
  GDLReasoner& operator=(GDLReasoner&& gdlr);

  //! Returns if the current knowledge satisfies terminal predicate
  //!
  //! \return bool*
  //!
  //!
  inline bool* IsTerminal() const;

  //! Returns the legal moves based on the current knowledge
  //!
  //! \return MoveList*
  //!
  //!
  inline MoveList* GetLegalMoves() const;

  //! Returns next state based on the current knowledge
  //!
  //! \return State*
  //!
  //!
  inline State* GetNextState() const;

  //! Returns the goal value associated with given role ID based on the current
  //! knowledge
  //!
  //! \param rid Role ID
  //! \return size_t*
  //!
  //!
  inline size_t* GetGoal(size_t rid);

  //! Apply the given state to knowledge base
  //!
  //! \param state State to apply
  //! \return void
  //!
  //!
  inline void ApplyState(const State& state);

  //! Apply the given actions to knowledge base
  //!
  //! \param moves Actions to apply
  //! \return void
  //!
  //!
  inline void ApplyActions(const Move& moves);


  //! Remove the state representation from the knowledge base
  //!
  //! \return void
  //!
  //!
  inline void RemoveState();

  //! Remove the knowledge of actions from knowledge base
  //!
  //! \return void
  //!
  //!
  inline void RemoveActions();

  //! Get initial state
  //!
  //! \return const State&
  //!
  //!
  const State& InitState() const
  {
    return *init;
  }

  //! Get SymbolTable
  SymbolTable GetSymbolTable() const
  {
    return base_rules.GetSymbolTable();
  }

  //! Get roles of this game
  const std::list<Argument*>& Roles() const
  {
    return roles;
  }

 private:
  //! KnowledgeBase storing the current known knowledge
  mutable KnowledgeBase base_rules;
  //! Initial state
  State* init;
  //! Roles of the game
  std::list<Argument*> roles;
  //! Logging stream
  mutable Log log;
}; // class GDLReasoner

}; // namespace libgdl
}; // namespace libgdl

#include "gdlreasoner_impl.hpp"

#endif // _LIBGDL_REASONERS_GDLREASONER_HPP_INCLUDED
