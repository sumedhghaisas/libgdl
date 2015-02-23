#ifndef LIBGDL_PROPNET_BASE_NODE_HPP_INCLUDED
#define LIBGDL_PROPNET_BASE_NODE_HPP_INCLUDED

#include <libgdl/core/util/to_string.hpp>

#include "node.hpp"

namespace libgdl
{
namespace propnet
{

class EntryManager;

namespace node_types
{

struct BaseNode : public Node
{
  BaseNode(const std::string& name, size_t id)
    : Node(name, Node::Type::BASE), id(id)
  {}

  std::string UName() const
  {
    std::string out = "BASE_" + ToString(id);
    return out;
  }

  std::tuple<bool, size_t> CodeGen(EntryManager& em, size_t v_stamp);

  size_t id;
};

}
}
}


#endif // LIBGDL_PROPNET_BASE_NODE_HPP_INCLUDED
