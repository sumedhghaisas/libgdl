#include "input_node.hpp"

#include <list>
#include <iostream>

#include <libgdl/propnet/propnet.hpp>

using namespace std;
using namespace libgdl::propnet;
using namespace libgdl::propnet::node_types;
using namespace libgdl::propnet::compilation::entry_types;
using namespace libgdl::propnet::crystallization;
using namespace libgdl::propnet::compilation;

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

bool InputNode::CrystalInitialize(const PropNet& pn,
                                  const std::map<const Node*, CrystalData>& crystal_data_map,
                                  PropNetPayLoad& payload,
                                  std::set<const Node*>& initialized)
{
  if(initialized.find(this) != initialized.end())
    return holding_value;

  holding_value = false;
  if(in_id == 0)
  {
    holding_value = true;
  }

  SimPolicyInitializeUpdate(holding_value, payload.data[crystal_data_map.find(this)->second.id]);

  initialized.insert(this);

  return holding_value;
}

void InputNode::RegisterToPropnet(PropNet& pn, Node* to_reg) const
{
  pn.AddInputNode(to_reg, r_id, in_id);
}
