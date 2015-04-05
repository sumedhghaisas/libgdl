#ifndef LIBGDL_PROPNET_TERMINAL_NODE_HPP_INCLUDED
#define LIBGDL_PROPNET_TERMINAL_NODE_HPP_INCLUDED

#include "node.hpp"

namespace libgdl
{
namespace propnet
{

class EntryManager;

namespace node_types
{

struct TerminalNode : public Node
{
  TerminalNode(const std::string& name)
    : Node(name, Node::Type::TERMINAL)
  {}

  std::tuple<bool, size_t> CodeGen(EntryManager& em, size_t visit_stamp);

  bool InitializeValue(const PropNet&, AState& s, Set<size_t>* m_set, size_t* goals);

  bool CrystalInitialize(const PropNet& pn, const std::map<const Node*, size_t>& id_map, signed short* data, AState& s, Set<size_t>* m_set, size_t* goals, std::set<const Node*>& initialized);

  void Update(bool value, AState& base, AState& top, AMove& m, Set<size_t>* m_set, size_t* goals);

  Node* GetCopy_only_info() const
  {
    return new TerminalNode(name);
  }

  void RegisterToPropnet(PropNet& pn, Node* to_reg) const;

  size_t num_true;
};

}
}
}


#endif // LIBGDL_PROPNET_TERMINAL_NODE_HPP_INCLUDED
