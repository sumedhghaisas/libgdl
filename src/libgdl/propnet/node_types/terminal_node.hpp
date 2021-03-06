#ifndef LIBGDL_PROPNET_TERMINAL_NODE_HPP_INCLUDED
#define LIBGDL_PROPNET_TERMINAL_NODE_HPP_INCLUDED

#include "sim_node.hpp"

namespace libgdl
{
namespace propnet
{

namespace node_types
{

struct TerminalNode : public SimNode
{
  TerminalNode(const std::string& name, const SimType& sim_type = SimType::OR)
    : SimNode(name, Node::Type::TERMINAL, sim_type)
  {}

  std::tuple<bool, size_t> CodeGen(compilation::EntryManager& em, size_t visit_stamp);

  bool CrystalInitialize(const PropNet& pn,
                         const std::map<const Node*, CrystalData>& crystal_data_map,
                         PropNetPayLoad& payload,
                         std::set<const Node*>& initialized);

  void CrystalUpdate(signed short val,
                     AState& top,
                     signed short& mem,
                     size_t* legal_size,
                     size_t* goals,
                     bool& terminal) const
  {
    if(CrystalConfig::GetCrystalBoolValue(mem))
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
    if(CrystalConfig::GetCrystalBoolValue(mem))
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
