#include "next_node.hpp"

#include <list>
#include <iostream>

#include "../entry_manager.hpp"
#include "../entry_types/save_entry.hpp"

using namespace std;
using namespace libgdl::propnet;
using namespace libgdl::propnet::node_types;
using namespace libgdl::propnet::entry_types;

tuple<bool, size_t> NextNode::CodeGen(EntryManager& em, size_t v_stamp)
{
  if(visit_stamp != v_stamp)
  {
    visit_stamp = v_stamp;
    isVisited = false;
  }

  if(!isVisited)
  {
    size_t out = em.GetNewID();

    list<tuple<bool, size_t>> in_ids;

    for(auto n : in_degree)
    {
      auto t_entry = n->CodeGen(em, v_stamp);
      em.AddStamp(get<1>(t_entry), out);
      in_ids.push_back(t_entry);
    }

    em.AddEntry(new SaveEntry(out, id, in_ids));

    entry_ret = tuple<bool, size_t>(true, out);
    isVisited = true;
  }

  return entry_ret;
}
