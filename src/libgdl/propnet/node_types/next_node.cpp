#include "next_node.hpp"

#include <list>
#include <iostream>

#include "../entry_manager.hpp"
#include "../entry_types/save_entry.hpp"
#include "../entry_types/or_entry.hpp"

#include "../propnet.hpp"

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
    size_t or_entry_id = em.GetNewID();

    list<tuple<bool, size_t>> in_ids;

    for(auto n : in_degree)
    {
      auto t_entry = n->CodeGen(em, v_stamp);
      em.AddStamp(get<1>(t_entry), or_entry_id);
      in_ids.push_back(t_entry);
    }
    em.AddEntry(new OrEntry(or_entry_id, in_ids));

    size_t out = em.GetNewID();
    em.AddStamp(or_entry_id, out);

    in_ids.clear();
    in_ids.emplace_back(true, or_entry_id);

    em.AddEntry(new SaveEntry(out, id, in_ids));

    entry_ret = tuple<bool, size_t>(true, out);
    isVisited = true;
  }

  return entry_ret;
}

bool NextNode::InitializeValue(const PropNet& pn, AState& s, std::set<size_t>* m_set, size_t* goals)
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
    s.Set(id, true);
  return holding_value;
}

void NextNode::Update(bool value, AState& base, AState& top, AMove& m, set<size_t>* m_set, size_t* goals)
{
  if(value)
  {
    num_true++;
    if(holding_value)
      return;
    holding_value = true;
    top.Set(id, true);
    return;
  }

  --num_true;

#ifdef LIBGDL_DFP_TEST
  if(num_true < 0 || !holding_value)
  {
    cout << "Something wrong in DFP" << endl;
    cout << Name() << endl;
    exit(1);
  }
#endif // LIBGDL_DFP_TEST

  if(!num_true)
  {
    holding_value = false;
    top.Set(id, false);
  }
}

void NextNode::RegisterToPropnet(PropNet& pn, Node* to_reg) const
{
  pn.AddNextNode(to_reg, id);
}
