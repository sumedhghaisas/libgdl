#ifndef LIBGDL_PROPNET_IMOVE_HPP_INCLUDED
#define LIBGDL_PROPNET_IMOVE_HPP_INCLUDED

#include <map>
#include <string>
#include <vector>
#include <iostream>

namespace libgdl
{
namespace propnet
{

struct IMove
{
  typedef std::map<std::string, size_t> Map;

  void AddRole()
  {
    map_list.emplace_back();
    c_m_ids.emplace_back(0);
  }

  size_t AddMove(const std::string& str, size_t r_id)
  {
    Map& m = map_list[r_id];
    auto it = m.find(str);
    if(it == m.end())
    {
      m[str] = c_m_ids[r_id]++;
      return c_m_ids[r_id] - 1;
    }

    return it->second;
  }

  void PrintMap()
  {
    for(auto it : map_list)
    {
      for(auto it2 : it)
      {
        std::cout << it2.first << " " << it2.second << std::endl;
      }
      std::cout << std::endl;
    }
  }

  void CodeGen(std::ostream& stream);

  std::vector<Map> map_list;
  std::vector<size_t> c_m_ids;
};

}
}


#endif // LIBGDL_PROPNET_IMOVE_HPP_INCLUDED
