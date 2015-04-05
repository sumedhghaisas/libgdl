#include "goal_node.hpp"

#include <list>
#include <iostream>

#include "../entry_manager.hpp"
#include "../entry_types/or_entry.hpp"

#include "../propnet.hpp"

using namespace std;
using namespace libgdl::propnet;
using namespace libgdl::propnet::node_types;
using namespace libgdl::propnet::entry_types;

tuple<bool, size_t> GoalNode::CodeGen(EntryManager& em, size_t v_stamp)
{
  if(visit_stamp != v_stamp)
  {
    visit_stamp = v_stamp;
    isVisited = false;
  }

  if(!isVisited)
  {
    //size_t out = em.GetNewID();

    list<tuple<bool, size_t>> in_ids;

    entry_ret = (*in_degree.begin())->CodeGen(em, v_stamp);
    em.AddStamp(get<1>(entry_ret), (size_t)-1);

    //em.AddEntry(new OrEntry(out, in_ids));
    isVisited = true;
  }

  return entry_ret;
}

bool GoalNode::InitializeValue(const PropNet& pn, AState& s, Set<size_t>* m_set, size_t* goals)
{
  holding_value = false;
  num_true = 0;
  for(auto it : in_degree)
  {
    bool temp = it->InitializeValue(pn, s, m_set, goals);
    if(temp) num_true++;
    holding_value = holding_value || temp;
  }
  if(holding_value)
    goals[r_id] = id;
  return holding_value;
}

bool GoalNode::CrystalInitialize(const PropNet& pn, const std::map<const Node*, size_t>& id_map, signed short* data, AState& s, Set<size_t>* m_set, size_t* goals, std::set<const Node*>& initialized)
{
  if(initialized.find(this) != initialized.end())
    return holding_value;

  holding_value = (*in_degree.begin())->CrystalInitialize(pn, id_map, data, s, m_set, goals, initialized);

  if(holding_value)
    data[id_map.find(this)->second] += 0x0001;

  initialized.insert(this);

  return holding_value;
}

void GoalNode::Update(bool value, AState& base, AState& top, AMove& m, Set<size_t>* m_set, size_t* goals)
{
  if(value)
  {
    //num_true++;
    //holding_value = true;
    goals[r_id] = id;
    return;
  }

  //--num_true;

#ifdef LIBGDL_DFP_TEST
  if(num_true < 0 || !holding_value)
  {
    cout << "Something wrong in DFP" << endl;
    cout << Name() << endl;
    exit(1);
  }
  node_count++;
#endif // LIBGDL_DFP_TEST

  //if(!num_true)
  //{
    //holding_value = false;
  //}
}

void GoalNode::RegisterToPropnet(PropNet& pn, Node* to_reg) const
{
  pn.AddGoalNode(to_reg, r_id, id);
}
