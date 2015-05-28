#ifndef LIBGDL_PROPNET_BASE_NODE_HPP_INCLUDED
#define LIBGDL_PROPNET_BASE_NODE_HPP_INCLUDED

#include <libgdl/core/util/to_string.hpp>

#include "sim_node.hpp"

namespace libgdl
{
namespace propnet
{

namespace node_types
{

struct BaseNode : public SimNode
{
  BaseNode(const std::string& name, size_t id, const SimType& sim_type = SimType::OR)
    : SimNode(name, Node::Type::BASE, sim_type), id(id)
  {}

  std::string UName() const
  {
    std::string out = "BASE_" + ToString(id);
    return out;
  }

  std::tuple<bool, size_t> CodeGen(compilation::EntryManager& em, size_t v_stamp);

  bool CrystalInitialize(const PropNet& pn,
                         const std::map<const Node*, size_t>& id_map,
                         PropNetPayLoad& payload,
                         std::set<const Node*>& initialized);

  void CreateCopy(PropNet& pn, Node* parent, std::map<Node*, Node*>& node_map);

  Node* GetCopy_only_info() const
  {
    return new BaseNode(name, id, sim_type);
  }

  void RegisterToPropnet(PropNet& pn, Node* to_reg) const;

  size_t id;
};

}
}
}


#endif // LIBGDL_PROPNET_BASE_NODE_HPP_INCLUDED
