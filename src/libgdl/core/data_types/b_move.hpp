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


#include <libgdl/core/symbol_table/symbol_table.hpp>

#include "argument.hpp"

namespace libgdl
{

/**
 * Represents moves of players in a game.
 * Moves are stores as vector of arguments ordered by the order of roles(role id).
 *
 * @see GDL
 */
struct BMove
{
  //! Constructs move from a string
  //! Can be used in single player games
  //!
  //! \param str string representation of the move
  //! \param symbol_table SymbolTable to decode
  //! \param log Logging stream
  //!
  //!
  BMove(const std::string& str,
        core::SymbolTable symbol_table,
        Log log = GLOBAL_LOG);

  //! Constructs move from a strings
  //! Can be used in 2 player games
  //!
  //! \param str1 String representation of player 1 move
  //! \param str2 String representation of player 2 move
  //! \param symbol_table SymbolTable to decode
  //! \param log Logging stream
  //!
  //!
  BMove(const std::string& str1,
        const std::string& str2,
        core::SymbolTable symbol_table,
        Log log = GLOBAL_LOG);

  //! Constructs move from vector of arguments (deep copy)
  //!
  //! \param m vector of moves
  //!
  //!
  BMove(const std::vector<core::Argument*>& m);

  //! Copy-constructor (deep copy)
  //!
  //! \param m Move to copy from
  //!
  //!
  BMove(const BMove& m) noexcept;

  //! Move-constructor
  //!
  //! \param m Move to move from
  //!
  //!
  BMove(BMove&& m) noexcept;

  //! Destructor
  ~BMove();

  //! Copy-assignment operator
  BMove& operator=(const BMove& m);
  //! Move assignment constructor
  BMove& operator=(BMove&& arg) { swap(*this, arg); return *this; }

  //! Comparison operators
  bool operator==(const BMove& m) const
  {
    return (hash == m.hash);
  }
  //! Comparison operators
  bool operator!=(const BMove& m) const { return !(*this == m); }

  //! Swap function
  //!
  //! \param m1
  //! \param m2
  //! \return void
  //!
  //!
  friend inline void swap(BMove& m1, BMove& m2)
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
}; // struct BMove

}; // namespace libgdl

#endif // MOVE_HPP_INCLUDED
