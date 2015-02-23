#ifndef LIBGDL_PROPNET_NOT_NODE_HPP_INCLUDED
#define LIBGDL_PROPNET_NOT_NODE_HPP_INCLUDED

#include <libgdl/core/util/to_string.hpp>

#include "node.hpp"

namespace libgdl
{
namespace propnet
{

class EntryManager;

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

  std::tuple<bool, size_t> CodeGen(EntryManager& em, size_t visit_stamp);

  size_t id;
};

}
}
}


#endif // LIBGDL_PROPNET_NOT_NODE_HPP_INCLUDED
