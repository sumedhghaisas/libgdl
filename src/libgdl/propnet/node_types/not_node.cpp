#include "not_node.hpp"

#include "../entry_manager.hpp"
#include "../entry_types/not_entry.hpp"

#include "../propnet.hpp"

using namespace std;
using namespace libgdl::propnet;
using namespace libgdl::propnet::node_types;
using namespace libgdl::propnet::entry_types;

tuple<bool, size_t> NotNode::CodeGen(EntryManager& em, size_t v_stamp)
{
  if(visit_stamp != v_stamp)
  {
    visit_stamp = v_stamp;
    isVisited = false;
  }

  if(!isVisited)
  {
    //size_t out = em.GetNewID();

    tuple<bool, size_t> in = (*in_degree.begin())->CodeGen(em, v_stamp);

    //em.AddStamp(get<1>(in), out);

    //em.AddEntry(new NotEntry(out, in));

    entry_ret = tuple<bool, size_t>(!std::get<0>(in), std::get<1>(in));

    isVisited = true;
  }

  return entry_ret;
}

bool NotNode::InitializeValue(const PropNet& pn, AState& s, MoveSet* m_set, size_t* goals)
{
  holding_value = !(*in_degree.begin())->InitializeValue(pn, s, m_set, goals);
  return holding_value;
}

bool NotNode::CrystalInitialize(const PropNet& pn, const std::map<const Node*, size_t>& id_map, signed short* data, AState& s, MoveSet* m_set, size_t* goals, std::set<const Node*>& initialized)
{
  if(initialized.find(this) != initialized.end())
    return holding_value;

  holding_value = !(*in_degree.begin())->CrystalInitialize(pn, id_map, data, s, m_set, goals, initialized);

  if(holding_value)
    data[id_map.find(this)->second] = 0xffff;

  initialized.insert(this);

  return holding_value;
}

void NotNode::Update(bool value, AState& base, AState& top, AMove& m, MoveSet* m_set, size_t* goals)
{
#ifdef LIBGDL_DFP_TEST
  if(holding_value != value)
  {
    cout << "Something wrong in DFP" << endl;
    cout << Name() << endl;
    exit(1);
  }
  node_count++;
#endif // LIBGDL_DFP_TEST

  holding_value = !holding_value;
  for(auto it : out_degree)
    it->Update(holding_value, base, top, m, m_set, goals);
}

void NotNode::RegisterToPropnet(PropNet& pn, Node* to_reg) const
{
  pn.AddNotNode(to_reg);
}
