/**
 * @file move.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of Move data structure which is used to represent the actions
 * performed by players.
 */
#ifndef MOVE_HPP_INCLUDED
#define MOVE_HPP_INCLUDED

#include <vector>
#include <list>
#include <string>
#include <boost/unordered_map.hpp>
#include <boost/functional/hash.hpp>
#include <boost/intrusive_ptr.hpp>

#include <libgdl/core/symbol_table/symbol_table.hpp>

#include "argument.hpp"
#include "intrusive_list.hpp"

namespace libgdl
{

/**
 * Represents moves of players in a game.
 * Moves are stores as vector of arguments ordered by the order of roles(role id).
 *
 * @see GDL
 */
struct Move
{
  //! Constructs move from a string
  //! Can be used in single player games
  //!
  //! \param str string representation of the move
  //! \param symbol_table SymbolTable to decode
  //! \param log Logging stream
  //!
  //!
  Move(const std::string& str,
       core::SymbolTable& symbol_table,
       Log log = GLOBAL_LOG);

  //! Constructs move from vector of arguments (deep copy)
  //!
  //! \param m vector of moves
  //!
  //!
  Move(const std::vector<core::Argument*>& m);

  //! Copy-constructor (deep copy)
  //!
  //! \param m Move to copy from
  //!
  //!
  Move(const Move& m) noexcept;

  //! Move-constructor
  //!
  //! \param m Move to move from
  //!
  //!
  Move(Move&& m) noexcept;

  //! Destructor
  ~Move();

  //! Copy-assignment operator
  Move& operator=(const Move& m);
  //! Move assignment constructor
  Move& operator=(Move&& arg) { swap(*this, arg); return *this; }

  //! Comparison operators
  bool operator==(const Move& m) const
  {
    return (hash == m.hash);
  }
  //! Comparison operators
  bool operator!=(const Move& m) const { return !(*this == m); }

  //! Swap function
  //!
  //! \param m1
  //! \param m2
  //! \return void
  //!
  //!
  friend inline void swap(Move& m1, Move& m2)
  {
    using std::swap;

    swap(m1.moves, m2.moves);
    swap(m1.hash, m2.hash);
  }

  //! Get hash of this move
  //!
  //! \return size_t
  //!
  //!
  size_t Hash() const { return hash; }

  //! Returns string representation of this Move using the symbol table
  //! This function is used by SymbolDecodeStream to print Move
  //!
  //! \param symbol_table const SymbolTable&
  //! \return std::string
  //!
  //! @see SymbolDecodeStream
  //!
  std::string DecodeToString(const core::SymbolTable& symbol_table) const;

  //! moves represented as vector of arguments
  std::vector<core::Argument*> moves;
  //! hash value
  size_t hash;
}; // struct Move

/**
 * Represents the list of moves which supports intrusive pointers.
 * This is used in storing list of legal moves in GDL.
 * This class is implemented in such a way that the intrusive pointer part is
 * totally abstracted from the user. For user MoveList can act like a normal
 * class rather than a smart pointer.
 *
 * @see IntrusiveList, Move, GDL
 */
class MoveList : public boost::intrusive_ptr<core::IntrusiveList<Move> >
{
  template<typename T>
  using IntrusiveList = core::IntrusiveList<T>;

 public:
  //! Creates MoveList from IntrusiveList pointer
  //!
  //! \param m pointer to list
  //!
  //!
  MoveList(IntrusiveList<Move>* m)
    : boost::intrusive_ptr<IntrusiveList<Move> >(m) {}

  //! implements its own iterator
  typedef IntrusiveList<Move>::iterator iterator;
  //! implements its own const_iterator
  typedef IntrusiveList<Move>::const_iterator const_iterator;

  //! Returns const_iterator pointing to the beginning of the list
  //!
  //! \return const_iterator
  //!
  //!
  const_iterator begin() const { return get()->begin(); }

  //! Returns iterator pointing to the beginning of the list
  //!
  //! \return iterator
  //!
  //!
  iterator begin() { return get()->begin(); }

  //! Returns const_iterator pointing to the end of the list
  //!
  //! \return const_iterator
  //!
  //!
  const_iterator end() const { return get()->end(); }

  //! Returns iterator pointing to the end of the list
  //!
  //! \return iterator
  //!
  //!
  iterator end() { return get()->end(); }

  //! Returns size of the list
  //!
  //! \return size_t
  //!
  //!
  size_t size() { return get()->size(); }

  //! Returns string representation of this MoveList using the symbol table
  //! This function is used by SymbolDecodeStream to print MoveList
  //!
  //! \param symbol_table const SymbolTable&
  //! \return std::string
  //!
  //! @see SymbolDecodeStream
  //!
  std::string DecodeToString(const core::SymbolTable& symbol_table) const;
}; // class MoveList

}; // namespace libgdl

#endif // MOVE_HPP_INCLUDED
