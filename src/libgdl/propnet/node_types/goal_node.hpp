#ifndef LIBGDL_PROPNET_GOAL_NODE_HPP_INCLUDED
#define LIBGDL_PROPNET_GOAL_NODE_HPP_INCLUDED

#include <libgdl/core/util/to_string.hpp>

#include "sim_node.hpp"

namespace libgdl
{
namespace propnet
{
namespace node_types
{

struct GoalNode : public SimNode
{
  GoalNode(const std::string& name, size_t r_id, size_t id, const SimType& sim_type = SimType::OR)
    : SimNode(name, Node::Type::GOAL, sim_type), r_id(r_id), id(id)
  {}

  std::string Name() const
  {
    return "GOAL_" + ToString(r_id) + "_" + name;
  }

  std::string UName() const
  {
    std::string out = "GOAL_" + ToString(r_id) + "_" + ToString(id);
    return out;
  }

  std::tuple<bool, size_t> CodeGen(compilation::EntryManager& em, size_t visit_map);

  bool CrystalInitialize(const PropNet& pn,
                         const std::map<const Node*, CrystalData>& crystal_data_map,
                         PropNetPayLoad& payload,
                         std::set<const Node*>& initialized);

  void CrystalUpdate(signed short val,
                     AState& top,
                     signed short& mem,
                     size_t* legal_size,
                     size_t* goals,
                     bool& terminal) const
  {
    if(val == CrystalConfig::CrystalIncrementVal)
    {
      goals[r_id] = id;
    }
  }

  Node* GetCopy_only_info() const
  {
    return new GoalNode(name, r_id, id, sim_type);
  }

  void RegisterToPropnet(PropNet& pn, Node* to_reg) const;

  size_t r_id;
  size_t id;

  size_t num_true;
};

}
}
}


#endif // LIBGDL_PROPNET_GOAL_NODE_HPP_INCLUDED
