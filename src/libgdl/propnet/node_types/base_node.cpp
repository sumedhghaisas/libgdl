#include "base_node.hpp"

#include "../entry_manager.hpp"
#include "../entry_types/load_entry.hpp"

#include "../propnet.hpp"

using namespace std;
using namespace libgdl::propnet;
using namespace libgdl::propnet::node_types;
using namespace libgdl::propnet::entry_types;
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

bool BaseNode::CrystalInitialize(const PropNet& pn, const std::map<const Node*, size_t>& id_map, signed short* data, AState& s, MoveSet* m_set, size_t* goals, std::set<const Node*>& initialized)
{
  if(initialized.find(this) != initialized.end())
    return holding_value;

  holding_value = true;

  if(pn.IsInitProp(id))
    holding_value = true;
  else holding_value = false;

  if(holding_value)
    data[id_map.find(this)->second] += CrystalConfig::CrystalIncrementVal;

  initialized.insert(this);

  return holding_value;
}

void BaseNode::RegisterToPropnet(PropNet& pn, Node* to_reg) const
{
  pn.AddBaseNode(to_reg, id);
}
