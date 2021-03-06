#include "terminal_node.hpp"

#include <list>
#include <iostream>

#include <libgdl/propnet/propnet.hpp>

using namespace std;
using namespace libgdl::propnet;
using namespace libgdl::propnet::node_types;
using namespace libgdl::propnet::compilation::entry_types;
using namespace libgdl::propnet::compilation;

tuple<bool, size_t> TerminalNode::CodeGen(EntryManager& em, size_t v_stamp)
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
        em.AddStamp(get<1>(t_entry), (size_t)-1);
        in_ids.push_back(t_entry);
      }

      em.AddEntry(new OrEntry(out, in_ids));

      entry_ret = tuple<bool, size_t>(true, out);
    }
    else if(in_degree.size() == 1)
    {
      entry_ret = (*in_degree.begin())->CodeGen(em, v_stamp);
      if(!get<0>(entry_ret))
      {
        size_t out = em.GetNewID();
        em.AddStamp(get<1>(entry_ret), out);

        em.AddEntry(new NotEntry(out, entry_ret));

        entry_ret = tuple<bool, size_t>(true, out);
      }
      em.AddStamp(get<1>(entry_ret), (size_t)-1);
    }
    else
    {
      std::cout << LOGID << "No inputs given to " << name << std::endl;
      exit(1);
    }
    isVisited = true;
  }

  return entry_ret;
}

bool TerminalNode::CrystalInitialize(const PropNet& pn,
                                     const std::map<const Node*, CrystalData>& crystal_data_map,
                                     PropNetPayLoad& payload,
                                     std::set<const Node*>& initialized)
{
  if(initialized.find(this) != initialized.end())
    return holding_value;

  for(auto it : in_degree)
  {
    bool temp = it->CrystalInitialize(pn, crystal_data_map, payload, initialized);
    SimPolicyInitializeUpdate(temp, payload.data[crystal_data_map.find(this)->second.id]);
  }

  holding_value = CrystalConfig::GetCrystalBoolValue(payload.data[crystal_data_map.find(this)->second.id]);

  initialized.insert(this);

  return holding_value;
}

void TerminalNode::RegisterToPropnet(PropNet& pn, Node* to_reg) const
{
  pn.AddTerminalNode(to_reg);
}
