#ifndef LIBGDL_PROPNET_OR_NODE_HPP_INCLUDED
#define LIBGDL_PROPNET_OR_NODE_HPP_INCLUDED

#include <libgdl/core/util/to_string.hpp>

#include "node.hpp"

namespace libgdl
{
namespace propnet
{

class EntryManager;

namespace node_types
{

struct OrNode : public Node
{
  OrNode(const std::string& name, size_t id)
    : Node(name, Node::Type::OR), id(id)
  {}

  std::string UName() const
  {
    return "OR_" + ToString(id);
  }

  std::tuple<bool, size_t> CodeGen(EntryManager& em, size_t visit_stamp);

  bool CrystalInitialize(const PropNet& pn,
                         const std::map<const Node*, size_t>& id_map,
                         signed short* data,
                         AState& s,
                         MoveSet* m_set,
                         size_t* goals,
                         std::set<const Node*>& initialized);

  Node* GetCopy_only_info() const
  {
    return new OrNode(name, id);
  }

  void RegisterToPropnet(PropNet& pn, Node* to_reg) const;

  size_t id;

  size_t num_true;
};

}
}
}


#endif // LIBGDL_PROPNET_OR_NODE_HPP_INCLUDED
