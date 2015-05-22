#include "legal_node.hpp"

#include <list>
#include <iostream>

#include "../entry_manager.hpp"
#include "../entry_types/or_entry.hpp"

#include "../propnet.hpp"

using namespace std;
using namespace libgdl::propnet;
using namespace libgdl::propnet::node_types;
using namespace libgdl::propnet::entry_types;
using namespace libgdl::propnet::crystallization;

tuple<bool, size_t> LegalNode::CodeGen(EntryManager& em, size_t v_stamp)
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

    em.AddEntry(new OrEntry(out, in_ids));

    entry_ret = tuple<bool, size_t>(true, out);
    isVisited = true;
  }

  return entry_ret;
}

bool LegalNode::InitializeValue(const PropNet& pn, AState& s, MoveSet* m_set, size_t* goals)
{
  holding_value = true;
  num_true = 0;
  if(!in_degree.empty())
  {
    bool temp = (*in_degree.begin())->InitializeValue(pn, s, m_set, goals);
    if(temp) num_true++;
    holding_value = temp;
  }
  if(holding_value)
    m_set[r_id].insert(id);
  return holding_value;
}

bool LegalNode::CrystalInitialize(const PropNet& pn, const std::map<const Node*, size_t>& id_map, signed short* data, AState& s, MoveSet* m_set, size_t* goals, std::set<const Node*>& initialized)
{
  if(initialized.find(this) != initialized.end())
    return holding_value;

  holding_value = false;

  for(auto it : in_degree)
  {
    bool temp = it->CrystalInitialize(pn, id_map, data, s, m_set, goals, initialized);
    if(temp)
      data[id_map.find(this)->second] += CrystalData::CrystalIncrementVal;

    holding_value = holding_value || temp;
  }

  if(in_degree.size() == 0)
    holding_value = true;

  if(holding_value)
  {
    m_set[r_id].insert(id);
  }

  initialized.insert(this);

  return holding_value;
}

void LegalNode::Update(bool value, AState& base, AState& top, AMove& m, MoveSet* m_set, size_t* goals)
{
  if(value && !holding_value)
  {
    //num_true++;
    //if(holding_value)
      //return;
    holding_value = true;
    m_set[r_id].insert(id);
    return;
  }
  else if(!value)
  {
    holding_value = false;
    m_set[r_id].erase(m_set[r_id].find(id));
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

  //}
}

void LegalNode::RegisterToPropnet(PropNet& pn, Node* to_reg) const
{
  pn.AddLegalNode(to_reg, r_id, id);
}
