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

struct Move
{
  Move(const std::string& str,
       core::SymbolTable& symbol_table,
       Log log = std::cout);

  Move(const std::vector<core::Argument*>& m);

  Move(const Move& m);

  Move(Move&& m);

  ~Move();

  Move& operator=(const Move& m);
  //! move assignment constructor
  Move& operator=(Move&& arg) { swap(*this, arg); return *this; }

  bool operator==(const Move& m) const
  {
    return (hash == m.hash);
  }
  bool operator!=(const Move& m) const { return !(*this == m); }

  friend inline void swap(Move& m1, Move& m2)
  {
    using std::swap;

    swap(m1.moves, m2.moves);
    swap(m1.hash, m2.hash);
  }

  size_t Hash() const { return hash; }

  std::string DecodeToString(const core::SymbolTable& symbol_table) const;

  std::vector<core::Argument*> moves;

  size_t hash;
}; // struct Move

class MoveList : public boost::intrusive_ptr<core::IntrusiveList<Move> >
{
  template<typename T>
  using IntrusiveList = core::IntrusiveList<T>;
 public:
  MoveList(IntrusiveList<Move>* m)
    : boost::intrusive_ptr<IntrusiveList<Move> >(m) {}

  typedef IntrusiveList<Move>::iterator iterator;
  typedef IntrusiveList<Move>::const_iterator const_iterator;

  const_iterator begin() const { return get()->begin(); }
  iterator begin() { return get()->begin(); }

  const_iterator end() const { return get()->end(); }
  iterator end() { return get()->end(); }

  size_t size() { return get()->size(); }

  std::string DecodeToString(const core::SymbolTable& symbol_table) const;
};

}; // namespace libgdl

#endif // MOVE_HPP_INCLUDED
