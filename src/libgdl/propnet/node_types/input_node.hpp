#ifndef LIBGDL_PROPNET_INPUT_NODE_HPP_INCLUDED
#define LIBGDL_PROPNET_INPUT_NODE_HPP_INCLUDED

#include <libgdl/core/util/to_string.hpp>

#include "node.hpp"

namespace libgdl
{
namespace propnet
{

class EntryManager;

namespace node_types
{

struct InputNode : public Node
{
  InputNode(const std::string& name, size_t r_id, size_t in_id)
    : Node(name, Node::Type::INPUT), r_id(r_id), in_id(in_id)
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

  std::tuple<bool, size_t> CodeGen(EntryManager& em, size_t visit_map);

  size_t r_id;
  size_t in_id;
};

}
}
}

#endif // LIBGDL_PROPNET_INPUT_NODE_HPP_INCLUDED