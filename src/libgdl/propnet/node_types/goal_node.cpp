#include "goal_node.hpp"

#include <list>
#include <iostream>

#include "../entry_manager.hpp"
#include "../entry_types/or_entry.hpp"
#include "../entry_types/not_entry.hpp"

#include "../propnet.hpp"

using namespace std;
using namespace libgdl::propnet;
using namespace libgdl::propnet::node_types;
using namespace libgdl::propnet::entry_types;
using namespace libgdl::propnet::crystallization;

tuple<bool, size_t> GoalNode::CodeGen(EntryManager& em, size_t v_stamp)
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

bool GoalNode::CrystalInitialize(const PropNet& pn, const std::map<const Node*, size_t>& id_map, signed short* data, AState& s, MoveSet* m_set, size_t* goals, std::set<const Node*>& initialized)
{
  if(initialized.find(this) != initialized.end())
    return holding_value;

  holding_value = false;

  for(auto it : in_degree)
  {
    bool temp = it->CrystalInitialize(pn, id_map, data, s, m_set, goals, initialized);
    CrystalConfig::OrPolicyCrystalInitialize(temp, data[id_map.find(this)->second]);
    holding_value = holding_value || temp;
  }

  if(holding_value)
  {
    goals[r_id] = id;
  }

  initialized.insert(this);

  return holding_value;
}

void GoalNode::RegisterToPropnet(PropNet& pn, Node* to_reg) const
{
  pn.AddGoalNode(to_reg, r_id, id);
}
