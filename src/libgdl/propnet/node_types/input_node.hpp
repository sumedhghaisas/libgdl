#ifndef LIBGDL_PROPNET_INPUT_NODE_HPP_INCLUDED
#define LIBGDL_PROPNET_INPUT_NODE_HPP_INCLUDED

#include <libgdl/core/util/to_string.hpp>

#include "sim_node.hpp"

namespace libgdl
{
namespace propnet
{

namespace node_types
{

struct InputNode : public SimNode
{
  InputNode(const std::string& name, size_t r_id, size_t in_id, const SimType& sim_type = SimType::OR)
    : SimNode(name, Node::Type::INPUT, sim_type), r_id(r_id), in_id(in_id)
  {}

  std::string Name() const
  {
    return ToString(r_id) + "_" + name;
  }

  std::string UName() const
  {
    std::string out = "INPUT_" + ToString(r_id) + "_" + ToString(in_id);
    return out;
  }

  std::tuple<bool, size_t> CodeGen(compilation::EntryManager& em, size_t visit_map);

  bool CrystalInitialize(const PropNet& pn,
                         const std::map<const Node*, size_t>& id_map,
                         PropNetPayLoad& payload,
                         std::set<const Node*>& initialized);

  Node* GetCopy_only_info() const
  {
    return new InputNode(name, r_id, in_id, sim_type);
  }

  void RegisterToPropnet(PropNet& pn, Node* to_reg) const;

  size_t r_id;
  size_t in_id;
};

}
}
}

#endif // LIBGDL_PROPNET_INPUT_NODE_HPP_INCLUDED
