#include "base_node.hpp"

#include <libgdl/propnet/propnet.hpp>

using namespace std;
using namespace libgdl::propnet;
using namespace libgdl::propnet::node_types;
using namespace libgdl::propnet::compilation;
using namespace libgdl::propnet::compilation::entry_types;
using namespace libgdl::propnet::crystallization;

tuple<bool, size_t> BaseNode::CodeGen(EntryManager& em, size_t v_stamp)
{
  if(visit_stamp != v_stamp)
  {
    visit_stamp = v_stamp;
    isVisited = false;
  }

  if(!isVisited)
  {
    size_t out = em.GetNewID();

    em.AddEntry(new LoadEntry(out, id));
    entry_ret = tuple<bool, size_t>(true, out);
    isVisited = true;
  }

  return entry_ret;
}

bool BaseNode::CrystalInitialize(const PropNet& pn,
                                 const std::map<const Node*, CrystalData>& crystal_data_map,
                                 PropNetPayLoad& payload,
                                 std::set<const Node*>& initialized)
{
  if(initialized.find(this) != initialized.end())
    return holding_value;

  holding_value = true;

  if(pn.IsInitProp(id))
    holding_value = true;
  else holding_value = false;

  SimPolicyInitializeUpdate(holding_value, payload.data[crystal_data_map.find(this)->second.id]);

  initialized.insert(this);

  return holding_value;
}

void BaseNode::RegisterToPropnet(PropNet& pn, Node* to_reg) const
{
  pn.AddBaseNode(to_reg, id);
}
