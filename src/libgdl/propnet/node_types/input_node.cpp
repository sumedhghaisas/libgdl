#include "input_node.hpp"

#include <list>
#include <iostream>

#include "../entry_manager.hpp"
#include "../entry_types/load_input_entry.hpp"

#include "../propnet.hpp"

using namespace std;
using namespace libgdl::propnet;
using namespace libgdl::propnet::node_types;
using namespace libgdl::propnet::entry_types;

tuple<bool, size_t> InputNode::CodeGen(EntryManager& em, size_t v_stamp)
{
  if(visit_stamp != v_stamp)
  {
    visit_stamp = v_stamp;
    isVisited = false;
  }

  if(!isVisited)
  {
    size_t out = em.GetNewID();

    em.AddEntry(new LoadInputEntry(out, r_id, in_id));
    entry_ret = tuple<bool, size_t>(true, out);
    isVisited = true;
  }

  return entry_ret;
}

bool InputNode::InitializeValue(const PropNet&, AState& s, std::set<size_t>* m_set, size_t* goals)
{
  if(in_id == 0)
  {
    holding_value = true;
    return true;
  }
  holding_value = false;
  return false;
}

void InputNode::Update(bool value, AState& base, AState& top, AMove& m, set<size_t>* m_set, size_t* goals)
{
  holding_value = value;
  m.Set(r_id, in_id);
  for(auto it : out_degree)
    it->Update(value, base, top, m, m_set, goals);
}

void InputNode::RegisterToPropnet(PropNet& pn, Node* to_reg)
{
  pn.AddInputNode(to_reg, r_id, in_id);
}
