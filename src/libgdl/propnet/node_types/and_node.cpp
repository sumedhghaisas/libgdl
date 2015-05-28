#include "and_node.hpp"

#include <list>
#include <iostream>

#include <libgdl/propnet/propnet.hpp>

using namespace std;
using namespace libgdl;
using namespace libgdl::propnet;
using namespace libgdl::propnet::node_types;
using namespace libgdl::propnet::crystallization;
using namespace libgdl::propnet::compilation;
using namespace libgdl::propnet::compilation::entry_types;

size_t Node::node_count = 0;

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

bool AndNode::CrystalInitialize(const PropNet& pn,
                                const std::map<const Node*, size_t>& id_map,
                                signed short* data,
                                AState& s,
                                MoveSet* m_set,
                                size_t* goals,
                                std::set<const Node*>& initialized)
{
  if(initialized.find(this) != initialized.end())
    return holding_value;

  for(auto it : in_degree)
  {
    bool temp = it->CrystalInitialize(pn, id_map, data, s, m_set, goals, initialized);
    CrystalConfig::AndPolicyCrystalInitialize(temp, data[id_map.find(this)->second]);
  }

  holding_value = CrystalConfig::GetCrystalBoolValue(data[id_map.find(this)->second]);

  initialized.insert(this);

  return holding_value;
}

void AndNode::RegisterToPropnet(PropNet& pn, Node* to_reg) const
{
  pn.AddAndNode(to_reg);
}
