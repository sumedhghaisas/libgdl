#ifndef LIBGDL_PROPNET_NEXT_NODE_HPP_INCLUDED
#define LIBGDL_PROPNET_NEXT_NODE_HPP_INCLUDED

#include <libgdl/core/util/to_string.hpp>

#include "node.hpp"

namespace libgdl
{
namespace propnet
{
namespace node_types
{

struct NextNode : public Node
{
  NextNode(const std::string& name, size_t id)
    : Node(name, Node::Type::NEXT), id(id)
  {}

  std::string Name() const
  {
    return "NEXT_" + name;
  }

  std::string UName() const
  {
    return "NEXT_" + ToString(id);
  }

  std::tuple<bool, size_t> CodeGen(EntryManager& em, size_t visit_map);

  size_t id;
};

}
}
}


#endif // LIBGDL_PROPNET_NEXT_NODE_HPP_INCLUDED
