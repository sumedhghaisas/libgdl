#include "and_node.hpp"

#include <list>
#include <iostream>

#include "../entry_manager.hpp"
#include "../entry_types/and_entry.hpp"

using namespace std;
using namespace libgdl::propnet;
using namespace libgdl::propnet::node_types;
using namespace libgdl::propnet::entry_types;

tuple<bool, size_t> AndNode::CodeGen(EntryManager& em, size_t v_stamp)
{
  if(visit_stamp != v_stamp)
  {
    visit_stamp = v_stamp;
    isVisited = false;
  }

  if(!isVisited)
  {
    if(in_degree.size() > 1)
    {
      size_t out = em.GetNewID();

      list<tuple<bool, size_t>> in_ids;

      for(auto n : in_degree)
      {
        auto t_entry = n->CodeGen(em, v_stamp);
        em.AddStamp(get<1>(t_entry), out);
        in_ids.push_back(t_entry);
      }

      em.AddEntry(new AndEntry(out, in_ids));

      entry_ret = tuple<bool, size_t>(true, out);
    }
    else if(in_degree.size() == 1)
    {
      entry_ret = (*in_degree.begin())->CodeGen(em, v_stamp);
    }
    else
    {
      std::cout << "No inputs given to " << name << std::endl;
      exit(1);
    }
    isVisited = true;
    visit_stamp = v_stamp;
  }

  return entry_ret;
}
