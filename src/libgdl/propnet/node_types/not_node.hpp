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

  bool InitializeValue(const PropNet&, AState& s, MoveSet* m_set, size_t* goals);

  bool CrystalInitialize(const PropNet& pn, const std::map<const Node*, size_t>& id_map, signed short* data, AState& s, MoveSet* m_set, size_t* goals, std::set<const Node*>& initialized);

  void Update(bool value, AState& base, AState& top, AMove& m, MoveSet* m_set, size_t* goals);

  Node* GetCopy_only_info() const
  {
    return new NotNode(name, id);
  }

  void RegisterToPropnet(PropNet& pn, Node* to_reg) const;

  size_t id;
};

}
}
}


#endif // LIBGDL_PROPNET_NOT_NODE_HPP_INCLUDED
