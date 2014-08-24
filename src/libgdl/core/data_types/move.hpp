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

#include "argument.hpp"

namespace libgdl
{

struct Move
{
  Move(const std::string& str)
  {
    moves.push_back(new Argument(str));
    hash = 0;
    boost::hash_combine(hash, str);
  }

  Move(const std::vector<Argument*>& m,
       const boost::unordered_map<std::string, size_t>& id_map)
  {
    hash = 0;
    for(std::vector<Argument*>::const_iterator it = m.begin();it != m.end();it++)
    {
      moves.push_back(new Argument(**it));
      size_t temp = (*it)->Hash(id_map);
      boost::hash_combine(hash, temp);
    }
  }

  Move(const Move& m)
  {
    for(size_t i = 0;i < m.moves.size();i++)
      moves.push_back(new Argument(*m.moves[i]));
    hash = m.hash;
  }

  Move& operator=(const Move& m)
  {
    for(size_t i = 0;i < moves.size();i++)
      delete moves[i];
    moves.clear();

    for(size_t i = 0;i < m.moves.size();i++)
      moves.push_back(new Argument(*m.moves[i]));
    return *this;
  }

  ~Move()
  {
    for(size_t i = 0;i < moves.size();i++)
      delete moves[i];
    moves.clear();
  }

  size_t Hash() const { return hash; }

  std::vector<Argument*> moves;
  size_t hash;
}; // struct Move

}; // namespace libgdl

inline std::ostream& operator<<(std::ostream& s, const libgdl::Move& m)
{
  for(size_t i = 0;i < m.moves.size();i++)
  {
    s << *m.moves[i] << std::endl;
  }
  return s;
}


#endif // MOVE_HPP_INCLUDED
