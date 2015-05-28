#ifndef LIBGDL_PROPNET_NOT_NODE_HPP_INCLUDED
#define LIBGDL_PROPNET_NOT_NODE_HPP_INCLUDED

#include <libgdl/core/util/to_string.hpp>

#include "node.hpp"

namespace libgdl
{
namespace propnet
{

namespace node_types
{

struct NotNode : public Node
{
  NotNode(const std::string& name, size_t id)
    : Node(name, Node::Type::NOT), id(id)
  {}

  std::string UName() const
  {
    return "NOT_" + ToString(id);
  }

  std::tuple<bool, size_t> CodeGen(compilation::EntryManager& em, size_t visit_stamp);

  bool CrystalInitialize(const PropNet& pn,
                         const std::map<const Node*, CrystalData>& crystal_data_map,
                         PropNetPayLoad& payload,
                         std::set<const Node*>& initialized);

  CrystalConfig::Type GetCrystalType() const
  {
    return CrystalConfig::Type::NOT;
  }

  Node* MergeWithChild(PropNet& pn);

  Node* GetCopy_only_info() const
  {
    return new NotNode(name, id);
  }

  void RegisterToPropnet(PropNet& pn, Node* to_reg) const;

  size_t id;
};

}
}
}


#endif // LIBGDL_PROPNET_NOT_NODE_HPP_INCLUDED
