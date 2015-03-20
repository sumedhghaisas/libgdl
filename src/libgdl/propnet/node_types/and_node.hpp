#ifndef LIBGDL_PROPNET_AND_NODE_HPP_INCLUDED
#define LIBGDL_PROPNET_AND_NODE_HPP_INCLUDED

#include <libgdl/core/util/to_string.hpp>

#include "node.hpp"

namespace libgdl
{
namespace propnet
{

class EntryManager;

namespace node_types
{

struct AndNode : public Node
{
  AndNode(const std::string& name, size_t id)
    : Node(name, Node::Type::AND), id(id)
  {}

  std::string UName() const
  {
    return "AND_" + ToString(id);
  }

  bool InitializeValue(const PropNet&, AState& s, std::set<size_t>* m_set, size_t* goals);

  std::tuple<bool, size_t> CodeGen(EntryManager& em, size_t v_stamp);

  void Update(bool value, AState& base, AState& top, AMove& m, std::set<size_t>* m_set, size_t* goals);

  Node* GetCopy_only_info() const
  {
    return new AndNode(name, id);
  }

  void RegisterToPropnet(PropNet& pn, Node* to_reg) const;

  size_t id;
  size_t num_false;
};

}
}
}

#endif // LIBGDL_PROPNET_AND_NODE_HPP_INCLUDED
