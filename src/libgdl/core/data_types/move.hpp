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

#include "argument.hpp"

namespace libgdl
{

struct Move
{
  Move() {};
  Move(const Move& m)
  {
    for(size_t i = 0;i < m.moves.size();i++)
      moves.push_back(new Argument(*m.moves[i]));
  }

  ~Move()
  {
    Clear();
  }

  void AddAction(const std::string& action)
  {
    moves.push_back(new Argument(action));
  }

  void AddAction(const std::vector<std::string>& actions)
  {
    for(size_t i = 0;i < actions.size();i++)
      moves.push_back(new Argument(actions[i]));
  }

  void Clear()
  {
    for(size_t i = 0;i < moves.size();i++)
      delete moves[i];
    moves.clear();
  }

  std::vector<Argument*> moves;
}; // struct Move

}; // namespace libgdl


#endif // MOVE_HPP_INCLUDED
