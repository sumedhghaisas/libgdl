#include "base_node.hpp"

#include "../entry_manager.hpp"
#include "../entry_types/load_entry.hpp"

#include "../propnet.hpp"

using namespace std;
using namespace libgdl::propnet;
using namespace libgdl::propnet::node_types;
using namespace libgdl::propnet::entry_types;

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

bool BaseNode::InitializeValue(const PropNet& pn, AState& s, MoveSet* m_set, size_t* goals)
{
  if(pn.IsInitProp(id))
    holding_value = true;
  else holding_value = false;

  return holding_value;
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
    data[id_map.find(this)->second] += 0x0001;

  initialized.insert(this);

  return holding_value;
}

void BaseNode::Update(bool value, AState& base, AState& top, AMove& m, MoveSet* m_set, size_t* goals)
{
  holding_value = value;
  base.Set(id, value);
  for(auto it : out_degree)
  {
    it->Update(value, base, top, m, m_set, goals);
  }

#ifdef LIBGDL_DFP_TEST
  node_count++;
#endif // LIBGDL_DFP_TEST
}

void BaseNode::RegisterToPropnet(PropNet& pn, Node* to_reg) const
{
  pn.AddBaseNode(to_reg, id);
}
