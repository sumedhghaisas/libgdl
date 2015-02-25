#ifndef LIBGDL_PROPNET_ENTRY_MANAGER_HPP_INCLUDED
#define LIBGDL_PROPNET_ENTRY_MANAGER_HPP_INCLUDED

#include <list>
#include <map>
#include <iostream>

#include "memory_manager.hpp"

#include "entry_types/entry.hpp"

namespace libgdl
{
namespace propnet
{

namespace entry_types
{

struct Entry;

}

class EntryManager
{
 public:
  EntryManager()
    : current_id(0) {}

  ~EntryManager()
  {
    for(auto it : l_entries)
    {
      for(auto it2 : it)
      {
        delete it2;
      }
    }
  }

  size_t GetNewID()
  {
    return current_id++;
  }

  void StartNewList()
  {
    l_entries.push_back(std::list<entry_types::Entry*>());
  }

  void AddEntry(entry_types::Entry* en)
  {
    (--l_entries.end())->push_back(en);
  }

  void AddStamp(size_t e_id, size_t s_id)
  {
    entry_stamps[e_id] = s_id;
  }

  void InitializeIterator()
  {
    l_entries_it = l_entries.begin();
    mm = MemoryManager();
  }

  std::list<size_t> CodeGen(std::ostream& ss,
                            std::ostream& ss2,
                            std::list<size_t> to_get_l);

  size_t GetRequiredMemory()
  {
    return mm.GetRequiredMemory();
  }

 private:
  std::list<std::list<entry_types::Entry*>> l_entries;

  std::map<size_t, size_t> entry_stamps;

  std::map<size_t, std::list<size_t>> cl_lists;

  size_t current_id;

  std::list<std::list<entry_types::Entry*>>::iterator l_entries_it;

  MemoryManager mm;
};

}
}


#endif // LIBGDL_PROPNET_ENTRY_MANAGER_HPP_INCLUDED
