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
