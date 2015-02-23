#ifndef LIBGDL_PROPNET_MEMORY_MANAGER_HPP_INCLUDED
#define LIBGDL_PROPNET_MEMORY_MANAGER_HPP_INCLUDED

#include <map>
#include <set>

namespace libgdl
{
namespace propnet
{

class MemoryManager
{
 public:
  MemoryManager()
    : next_new_l(0) {}

  size_t GetLocation(size_t entry_id) const
  {
    return entry_map.find(entry_id)->second;
  }

  size_t RequestLocation(size_t entry_id)
  {
    size_t out = 0;
    if(!open_l.empty())
    {
      out = *open_l.begin();
      open_l.erase(open_l.begin());
    }
    else out = next_new_l++;

    entry_map[entry_id] = out;
    return out;
  }

  void OpenLocation(size_t entry_id)
  {
    open_l.insert(entry_map.find(entry_id)->second);
  }

  size_t GetRequiredMemory()
  {
    return next_new_l;
  }

 private:

  std::set<size_t> open_l;
  std::set<size_t> closed_l;

  std::map<size_t, size_t> entry_map;

  size_t next_new_l;
};

}
}


#endif // LIBGDL_PROPNET_MEMORY_MANAGER_HPP_INCLUDED
