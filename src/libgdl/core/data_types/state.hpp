/**
 * @file state.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of State which is used to represent the state of the game.
 */
#ifndef _LIBGDL_GDL_STATES_HPP_INCLUDED
#define _LIBGDL_GDL_STATES_HPP_INCLUDED

#include <atomic>
#include <list>
#include <string>
#include <boost/unordered_map.hpp>
#include <boost/intrusive_ptr.hpp>

#include <libgdl/core/data_types/argument.hpp>
#include <libgdl/core/symbol_table/symbol_table.hpp>

namespace libgdl
{
namespace core
{

/**
 * Represents a raw state of the game. This representation of the state stores
 * the base propositions of the game true in this current state. This
 * representation can be applied to the knowledge base faster but can cause a
 * little memory overhead. RawState supports intrusive pointers which is used
 * by State
 *
 * @see State
 */
struct RawState
{
  //! Generates a state out of a existing vector of base_propositions
  //!
  //! \param props base_proposition true in the current state
  //!
  //!
  RawState(const std::list<Argument*> & props);

  //! Copy-constructor
  //!
  //! \param s state to copy from
  //!
  //!
  RawState(const RawState& s) noexcept;

  //! Copy-assignment operator
  RawState& operator=(const RawState& state);

  //! Destructor
  ~RawState()
  {
    for(std::list<Argument*>::const_iterator it = facts.begin();it != facts.end();it++)
      delete *it;
  }

  //! Returns the hash value of this state
  //! States with identical set of true base proposition will have
  //! same value of hash.
  //!
  //! \return const size_t&
  //!
  //!
  const size_t& GetHash() const { return hash; }

  //! comparison operators for State class
  bool operator==(const RawState& s) const
  {
    if (hash != s.GetHash()) return false;
    return true;
  }
  //! comparison operators for State class
  bool operator!=(const RawState& s) const {return !(*this == s); }

  //! Returns true base propositions
  const std::list<Argument*>& GetProps() const { return facts; }

  //! Computes and stores hash
  void calcHash();

  //! Returns string representation of this RawState using the symbol table
  //! This function is used by SymbolDecodeStream to print RawState
  //!
  //! \param symbol_table const SymbolTable&KIF kif(true, 1, TEST_LOG);
  //! \return std::string
  //!
  //! @see SymbolDecodeStream
  //!
  std::string DecodeToString(const SymbolTable& symbol_table) const;

  //! Hash value of the state
  size_t hash;
  //! list of true base propositions
  std::list<Argument*> facts;

  //! Returns reference count
  size_t ref_count() { return count; }
  //! Atomic reference count
  std::atomic_size_t count;
}; // struct RawState

//! Intrusive pointer release function for RawState
//! Decrements reference count of RawState object
//!
//! \param p object to decrement
//!
//!
inline void intrusive_ptr_release(core::RawState* p)
{
   if (--p->count == 0u)
        delete p;
}

//! Intrusive pointer add function for RawState
//! Increments reference count
//!
//! \param p object to increment
//!
//!
inline void intrusive_ptr_add_ref(core::RawState* p)
{
  ++p->count;
}

}; // namespace core

/**
 * State represents state of the game. State is a intrusive pointer
 * implementation which wraps RawState. State is implemented in such a way as to
 * abstract the intrusive pointer from user.
 *
 * @see KnowledgeBase, RawState
 */
struct State : public boost::intrusive_ptr<core::RawState>
{
  //! Constructs State from RawState pointer
  //!
  //! \param s RawState pointer
  //!
  //!
  State(core::RawState* s)
    : boost::intrusive_ptr<core::RawState>(s) {}

  //! \brief
  //!
  //! \return const size_t& GetHash() const { return
  //!
  //!
  const size_t& GetHash() const { return get()->GetHash(); }

  //! Get base propositions which are true in this state
  //!
  //! \return const std::list<core::Argument*>&
  //!
  //!
  const std::list<core::Argument*>& GetProps() const { return get()->facts; }

  //! Set base propositions which are true in this state
  //!
  //! \return std::list<core::Argument*>& GetProps() { return
  //!
  //!
  std::list<core::Argument*>& GetProps() { return get()->facts; }

  //! Comparison operator for State
  bool operator==(const State& s) { return *get() == *s.get(); }
  //! Comparison operator for State
  bool operator!=(const State& s) { return *get() != *s.get(); }

  //! Returns string representation of this State using the symbol table
  //! This function is used by SymbolDecodeStream to print State
  //!
  //! \param symbol_table const SymbolTable&
  //! \return std::string
  //!
  //! @see SymbolDecodeStream
  //!
  std::string DecodeToString(const core::SymbolTable& symbol_table) const;
}; // struct State

}; // namespace libgdl

#endif // _LIBGDL_GDL_STATES_HPP_INCLUDED
