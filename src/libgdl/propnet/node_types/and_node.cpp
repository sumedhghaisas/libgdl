#include "and_node.hpp"

#include <list>
#include <iostream>

#include "../entry_manager.hpp"
#include "../entry_types/and_entry.hpp"

#include "../propnet.hpp"

using namespace std;
using namespace libgdl;
using namespace libgdl::propnet;
using namespace libgdl::propnet::node_types;
using namespace libgdl::propnet::entry_types;

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

bool AndNode::InitializeValue(const PropNet& pn, AState& s, std::set<size_t>* m_set, size_t* goals)
{
  holding_value = true;
  num_false = 0;
  for(auto it : in_degree)
  {
    bool temp = it->InitializeValue(pn, s, m_set, goals);
    if(!temp) num_false++;
    holding_value = holding_value && temp;
  }
  return holding_value;
}

bool AndNode::CrystalInitialize(const PropNet& pn, const std::map<const Node*, size_t>& id_map, signed short* data, AState& s, std::set<size_t>* m_set, size_t* goals, std::set<const Node*>& initialized)
{
  if(initialized.find(this) != initialized.end())
    return holding_value;

  holding_value = true;
  for(auto it : in_degree)
  {
    bool temp = it->CrystalInitialize(pn, id_map, data, s, m_set, goals, initialized);
    if(!temp)
    {
      data[id_map.find(this)->second] += 0xffff;
    }
    holding_value = holding_value & temp;
  }

  initialized.insert(this);

  return holding_value;
}

void AndNode::Update(bool value, AState& base, AState& top, AMove& m, set<size_t>* m_set, size_t* goals)
{
  if(!value)
  {
    num_false++;
    if(!holding_value)
      return;
    holding_value = false;
    for(auto it : out_degree)
      it->Update(false, base, top, m, m_set, goals);
    return;
  }

  --num_false;

#ifdef LIBGDL_DFP_TEST
  if(num_false < 0 || holding_value)
  {
    cout << "Something wrong in DFP" << endl;
    cout << Name() << endl;
    exit(1);
  }
  node_count++;
#endif // LIBGDL_DFP_TEST

  if(!num_false)
  {
    holding_value = true;
    for(auto it : out_degree)
      it->Update(true, base, top, m, m_set, goals);
  }
}

void AndNode::RegisterToPropnet(PropNet& pn, Node* to_reg) const
{
  pn.AddAndNode(to_reg);
}
