#ifndef LIBGDL_PROPNET_LEGAL_NODE_HPP_INCLUDED
#define LIBGDL_PROPNET_LEGAL_NODE_HPP_INCLUDED

#include <libgdl/core/util/to_string.hpp>

#include "node.hpp"

namespace libgdl
{
namespace propnet
{
namespace node_types
{

struct LegalNode : public Node
{
  LegalNode(const std::string& name, size_t r_id, size_t id)
    : Node(name, Node::Type::LEGAL), r_id(r_id), id(id)
  {}

  std::string Name() const
  {
    return "LEGAL_" + ToString(r_id) + "_" + name;
  }

  std::string UName() const
  {
    std::string out = "LEGAL_" + ToString(r_id) + "_" + ToString(id);
    return out;
  }

  std::tuple<bool, size_t> CodeGen(EntryManager& em, size_t visit_map);

  size_t r_id;
  size_t id;
};

}
}
}


#endif // LIBGDL_PROPNET_LEGAL_NODE_HPP_INCLUDED
