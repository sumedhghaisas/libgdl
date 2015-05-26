#ifndef LIBGDL_PROPNET_NODE_TYPES_SIM_NODE_HPP_INCLUDED
#define LIBGDL_PROPNET_NODE_TYPES_SIM_NODE_HPP_INCLUDED

#include <string>

#include "node.hpp"

namespace libgdl
{
namespace propnet
{
namespace node_types
{

struct SimNode : public Node
{
  enum class SimType { OR, AND };

  SimNode(const std::string& name,
          const Type& type)
    : Node(name, type) {}

  CrystalConfig::Type GetCrystalType() const
  {
    if(type == Type::LEGAL || type == Type::GOAL || type == Type::TERMINAL || type == Type::NEXT)
    {
      if(sim_type == SimType::OR)
        return CrystalConfig::Type::OR_UPDATE;
      else return CrystalConfig::Type::AND_UPDATE;
    }
    else
    {
      if(sim_type == SimType::OR)
        return CrystalConfig::Type::OR;
      else return CrystalConfig::Type::AND;
    }
  }

  void SimAnd()
  {
    sim_type = SimType::AND;
  }

  void SimPolicyInitializeUpdate(bool temp, signed short& to_up)
  {
    if(sim_type == SimType::OR)
      CrystalConfig::OrPolicyCrystalInitialize(temp, to_up);
    else
    {
      CrystalConfig::AndPolicyCrystalInitialize(temp, to_up);
    }
  }

  Node* MergeWithChild(PropNet& pn);

  SimType sim_type = SimType::OR;
};

}
}
}


#endif // LIBGDL_PROPNET_NODE_TYPES_SIM_NODE_HPP_INCLUDED
