#include "entry_manager.hpp"

#include "entry_types/entry.hpp"

using namespace std;
using namespace libgdl::propnet;

list<size_t> EntryManager::CodeGen(CodeHandler& ch,
                                   list<size_t> to_get_l)
{
  for(auto entry : *l_entries_it)
  {
    size_t entry_id = entry->id;

    auto it = cl_lists.find(entry_id);
    if(it != cl_lists.end())
    {
      for(auto cl_e_id : it->second)
      {
        mm.OpenLocation(cl_e_id);
      }
    }

    entry->CodeGen(mm, ch);

    cl_lists[entry_stamps[entry_id]].push_back(entry_id);
  }

  list<size_t> out;

  for(auto to_get : to_get_l)
  {
    out.push_back(mm.GetLocation(to_get));
  }

  l_entries_it++;
  return out;
}
