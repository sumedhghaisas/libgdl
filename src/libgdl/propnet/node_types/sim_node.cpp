#include "sim_node.hpp"

#include "../propnet.hpp"

using namespace std;
using namespace libgdl;
using namespace libgdl::propnet;
using namespace libgdl::propnet::node_types;

Node* SimNode::MergeWithChild(PropNet& pn)
{
  if(in_degree.size() == 1)
  {
    Node* c = *in_degree.begin();

    if(c->IsOr() && c->out_degree.size() == 1)
    {
      if(sim_type == SimType::NOT)
        sim_type = SimType::NOR;
      else sim_type = SimType::OR;

      in_degree.clear();
      for(auto temp : c->in_degree)
      {
        temp->RemoveOutDegree(c);
        temp->AddOut(this);
        AddIn(temp);
      }
      c->in_degree.clear();
      c->out_degree.clear();
      pn.del.insert(c);
      delete c;
      return this;
    }
    else if(c->IsAnd() && c->out_degree.size() == 1)
    {
      if(sim_type == SimType::NOT)
        sim_type = SimType::NAND;
      else sim_type = SimType::AND;

      in_degree.clear();
      for(auto temp : c->in_degree)
      {
        temp->RemoveOutDegree(c);
        temp->AddOut(this);
        AddIn(temp);
      }
      c->in_degree.clear();
      c->out_degree.clear();
      pn.del.insert(c);
      delete c;
      return this;
    }
    else if(c->IsNot() && c->out_degree.size() == 1)
    {
      sim_type = SimType::NOT;

      in_degree.clear();
      for(auto temp : c->in_degree)
      {
        temp->RemoveOutDegree(c);
        temp->AddOut(this);
        AddIn(temp);
      }
      c->in_degree.clear();
      c->out_degree.clear();
      pn.del.insert(c);
      delete c;
      return this;
    }
  }

  return NULL;
}
