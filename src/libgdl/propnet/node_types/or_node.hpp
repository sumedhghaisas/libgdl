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

  size_t id;
};

}
}
}


#endif // LIBGDL_PROPNET_OR_NODE_HPP_INCLUDED
