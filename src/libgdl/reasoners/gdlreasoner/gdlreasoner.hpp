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

class GDLReasoner
{
  typedef core::Argument Argument;
  typedef core::Fact Fact;
  typedef core::SymbolTable SymbolTable;
 public:
  GDLReasoner() : init(NULL) {}

  template<class T>
  GDLReasoner(T& obj,
              const Log& log = std::cout);

  GDLReasoner(const GDLReasoner& gdlr);

  GDLReasoner(GDLReasoner&& gdlr);

  ~GDLReasoner();

  friend void swap(GDLReasoner& gdlr1, GDLReasoner& gdlr2)
  {
    using std::swap;

    swap(gdlr1.init, gdlr2.init);
    swap(gdlr1.roles, gdlr2.roles);
    swap(gdlr1.base_rules, gdlr2.base_rules);
  }

  GDLReasoner& operator=(const GDLReasoner& gdlr);
  GDLReasoner& operator=(GDLReasoner&& gdlr);


  inline bool* IsTerminal() const;

  inline MoveList* GetLegalMoves() const;

  inline State* GetNextState() const;

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

  const State& InitState() const
  {
    return *init;
  }

  SymbolTable GetSymbolTable() const
  {
    return base_rules.GetSymbolTable();
  }

  const std::list<Argument*>& Roles() const
  {
    return roles;
  }

 private:
  mutable KnowledgeBase base_rules;

  State* init;

  std::list<Argument*> roles;

  mutable Log log;
}; // class GDLReasoner

}; // namespace libgdl
}; // namespace libgdl

#include "gdlreasoner_impl.hpp"

#endif // _LIBGDL_REASONERS_GDLREASONER_HPP_INCLUDED
