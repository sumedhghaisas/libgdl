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

  bool InitializeValue(const PropNet&, AState& s, std::set<size_t>* m_set, size_t* goals);

  void Update(bool value, AState& base, AState& top, AMove& m, std::set<size_t>* m_set, size_t* goals);

  Node* GetCopy_only_info()
  {
    return new NotNode(name, id);
  }

  void RegisterToPropnet(PropNet& pn, Node* to_reg);

  size_t id;
};

}
}
}


#endif // LIBGDL_PROPNET_NOT_NODE_HPP_INCLUDED
