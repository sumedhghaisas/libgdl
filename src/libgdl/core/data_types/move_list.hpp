#ifndef LIBGDL_CORE_DATA_TYPES_OVE_LIST_HPP_INCLUDED
#define LIBGDL_CORE_DATA_TYPES_OVE_LIST_HPP_INCLUDED

#include <iostream>
#include <list>
#include <vector>

namespace libgdl
{

class MoveList : public std::list<Move>
{
 public:
  MoveList(const std::list<size_t>* result, size_t n_roles)
  {
    std::list<size_t>::const_iterator* it = new std::list<size_t>::const_iterator[n_roles];
    for(size_t i = 0;i < n_roles;i++)
      it[i] = result[i].begin();

    while(true)
    {
      std::list<size_t> moves;
      for(size_t i = 0;i < n_roles;i++)
      {
        moves.push_back(*it[i]);
      }
      push_back(Move(moves));

      it[0]++;
      size_t index = 1;
      if(it[0] == result[0].end())
      {
        it[0] = result[0].begin();

        while(true)
        {
          if(index == n_roles ||
            (it[index] == (--result[index].end()) && index == n_roles - 1))
          {
            delete[] it;
            return;
          }
          else if(it[index] == (--result[index].end()))
          {
            it[index] = result[index].begin();
            index++;
          }
          else
          {
            it[index]++;
            break;
          }
        }
      }
    }
  }
};

std::ostream& operator<<(std::ostream& stream, const MoveList& ml)
{
  stream << "MoveList {" << std::endl;

  for(auto it : ml)
  {
    stream << it << std::endl;
  }

  stream << "}";
  return stream;
}

}


#endif // LIBGDL_CORE_DATA_TYPES_OVE_LIST_HPP_INCLUDED
