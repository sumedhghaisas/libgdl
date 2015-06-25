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
  enum class SimType { OR, AND, NOT, NOR, NAND };

  SimNode(const std::string& name,
          const Type& type,
          const SimType& sim_type = SimType::OR)
    : Node(name, type), sim_type(sim_type) {}

  CrystalConfig::Type GetCrystalType() const
  {
    if(type == Type::LEGAL || type == Type::GOAL || type == Type::TERMINAL || type == Type::NEXT)
    {
      if(sim_type == SimType::OR)
        return CrystalConfig::Type::OR_UPDATE;
      else if(sim_type == SimType::AND)
        return CrystalConfig::Type::AND_UPDATE;
      else if(sim_type == SimType::NOT)
        return CrystalConfig::Type::NOT_UPDATE;
      else if(sim_type == SimType::NAND)
        return CrystalConfig::Type::NAND_UPDATE;
      else if(sim_type == SimType::NOR)
        return CrystalConfig::Type::NOR_UPDATE;
    }
    else
    {
      if(sim_type == SimType::OR)
        return CrystalConfig::Type::OR;
      else if(sim_type == SimType::AND)
        return CrystalConfig::Type::AND;
      else if(sim_type == SimType::NOT)
        return CrystalConfig::Type::NOT;
      else if(sim_type == SimType::NAND)
        return CrystalConfig::Type::NAND;
      else if(sim_type == SimType::NOR)
        return CrystalConfig::Type::NOR;
    }
  }

  void SimPolicyInitializeUpdate(bool temp, signed short& to_up)
  {
    if(sim_type == SimType::OR || sim_type == SimType::NOR)
      CrystalConfig::OrPolicyCrystalInitialize(temp, to_up);
    else if(sim_type == SimType::AND || sim_type == SimType::NAND)
      CrystalConfig::AndPolicyCrystalInitialize(temp, to_up);
    else if(sim_type == SimType::NOT)
      CrystalConfig::NotPolicyCrystalInitialize(temp, to_up);
  }

  Node* MergeWithChild(PropNet& pn);

  SimType sim_type = SimType::OR;
};

}
}
}


#endif // LIBGDL_PROPNET_NODE_TYPES_SIM_NODE_HPP_INCLUDED
