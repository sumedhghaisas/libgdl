#ifndef LIBGDL_PROPNET_VIEW_NODE_HPP_INCLUDED
#define LIBGDL_PROPNET_VIEW_NODE_HPP_INCLUDED

#include <libgdl/core/util/to_string.hpp>

#include "node.hpp"

namespace libgdl
{
namespace propnet
{

class EntryManager;

namespace node_types
{

struct ViewNode : public Node
{
  ViewNode(const std::string& name, size_t id)
    : Node(name, Node::Type::VIEW)
  {}

  std::string UName() const
  {
    std::string out = "VIEW_" + ToString(id);
    return out;
  }

  std::tuple<bool, size_t> CodeGen(EntryManager& em, size_t visit_stamp);

  size_t id;
};

}
}
}


#endif // LIBGDL_PROPNET_VIEW_NODE_HPP_INCLUDED
