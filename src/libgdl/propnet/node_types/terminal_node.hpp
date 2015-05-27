#ifndef LIBGDL_PROPNET_TERMINAL_NODE_HPP_INCLUDED
#define LIBGDL_PROPNET_TERMINAL_NODE_HPP_INCLUDED

#include "sim_node.hpp"

namespace libgdl
{
namespace propnet
{

class EntryManager;

namespace node_types
{

struct TerminalNode : public SimNode
{
  TerminalNode(const std::string& name, const SimType& sim_type = SimType::OR)
    : SimNode(name, Node::Type::TERMINAL, sim_type)
  {}

  std::tuple<bool, size_t> CodeGen(EntryManager& em, size_t visit_stamp);

  bool CrystalInitialize(const PropNet& pn,
                         const std::map<const Node*, size_t>& id_map,
                         signed short* data,
                         AState& s,
                         MoveSet* m_set,
                         size_t* goals,
                         std::set<const Node*>& initialized);

  void CrystalUpdate(signed short val,
                     AState& top,
                     signed short& mem,
                     size_t* legal_size,
                     size_t* goals,
                     bool& terminal) const
  {
    if(val == CrystalConfig::CrystalIncrementVal)
      terminal = true;
    else terminal = false;
  }

  void CrystalUpdate(signed short val,
                     AState& top,
                     signed short& mem,
                     MoveSet* legal_size,
                     size_t* goals,
                     bool& terminal) const
  {
    if(val == CrystalConfig::CrystalIncrementVal)
      terminal = true;
    else terminal = false;
  }

  Node* GetCopy_only_info() const
  {
    return new TerminalNode(name, sim_type);
  }

  void RegisterToPropnet(PropNet& pn, Node* to_reg) const;

  size_t num_true;
};

}
}
}


#endif // LIBGDL_PROPNET_TERMINAL_NODE_HPP_INCLUDED
