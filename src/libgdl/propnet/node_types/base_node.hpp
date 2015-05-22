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

  bool InitializeValue(const PropNet&, AState& s, MoveSet* m_set, size_t* goals);

  bool CrystalInitialize(const PropNet& pn,
                         const std::map<const Node*, size_t>& id_map,
                         signed short* data,
                         AState& s,
                         MoveSet* m_set,
                         size_t* goals,
                         std::set<const Node*>& initialized);

  void Update(bool value,
              AState& base,
              AState& top,
              AMove& m,
              MoveSet* m_set,
              size_t* goals);

  void CreateCopy(PropNet& pn, Node* parent, std::map<Node*, Node*>& node_map);

  Node* GetCopy_only_info() const
  {
    return new BaseNode(name, id);
  }

  void RegisterToPropnet(PropNet& pn, Node* to_reg) const;

  size_t id;
};

}
}
}


#endif // LIBGDL_PROPNET_BASE_NODE_HPP_INCLUDED
