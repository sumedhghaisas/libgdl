#include "not_node.hpp"

#include <libgdl/propnet/propnet.hpp>

using namespace std;
using namespace libgdl::propnet;
using namespace libgdl::propnet::node_types;
using namespace libgdl::propnet::compilation::entry_types;
using namespace libgdl::propnet::compilation;

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

bool NotNode::CrystalInitialize(const PropNet& pn,
                                const std::map<const Node*, CrystalData>& crystal_data_map,
                                PropNetPayLoad& payload,
                                std::set<const Node*>& initialized)
{
  if(initialized.find(this) != initialized.end())
    return holding_value;

  holding_value = !(*in_degree.begin())->CrystalInitialize(pn, crystal_data_map, payload, initialized);

  CrystalConfig::NotPolicyCrystalInitialize(holding_value, payload.data[crystal_data_map.find(this)->second.id]);

  initialized.insert(this);

  return holding_value;
}

void NotNode::RegisterToPropnet(PropNet& pn, Node* to_reg) const
{
  pn.AddNotNode(to_reg);
}

Node* NotNode::MergeWithChild(PropNet& pn)
{
  if(in_degree.size() != 1)
    return NULL;
  Node* c = *in_degree.begin();
  if(c->IsOr() && c->out_degree.size() == 1)
  {
    OrNode* or_n = static_cast<OrNode*>(c);
    or_n->isNOR = !or_n->isNOR;

    c->out_degree.clear();
    for(auto temp : out_degree)
    {
      temp->RemoveInDegree(this);
      temp->AddIn(c);
      c->AddOut(temp);
    }

    in_degree.clear();
    out_degree.clear();
    pn.del.insert(this);
    delete this;
    return c;
  }
  else if(c->IsAnd() && c->out_degree.size() == 1)
  {
    AndNode* and_n = static_cast<AndNode*>(c);
    and_n->isNand = !and_n->isNand;

    c->out_degree.clear();
    for(auto temp : out_degree)
    {
      temp->RemoveInDegree(this);
      temp->AddIn(c);
      c->AddOut(temp);
    }

    in_degree.clear();
    out_degree.clear();
    pn.del.insert(this);
    delete this;
    return c;
  }

  return NULL;
}
