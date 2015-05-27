#ifndef LIBGDL_PROPNET_NEXT_NODE_HPP_INCLUDED
#define LIBGDL_PROPNET_NEXT_NODE_HPP_INCLUDED

#include <libgdl/core/util/to_string.hpp>

#include "sim_node.hpp"

namespace libgdl
{
namespace propnet
{
namespace node_types
{

struct NextNode : public SimNode
{
  NextNode(const std::string& name, size_t id, const SimType& sim_type = SimType::OR)
    : SimNode(name, Node::Type::NEXT, sim_type), id(id)
  {}

  std::string Name() const
  {
    return "NEXT_" + name;
  }

  std::string UName() const
  {
    return "NEXT_" + ToString(id);
  }

  std::tuple<bool, size_t> CodeGen(EntryManager& em, size_t visit_map);

  bool CrystalInitialize(const PropNet& pn,
                         const std::map<const Node*, size_t>& id_map,
                         signed short* data,
                         AState& s,
                         MoveSet* m_set,
                         size_t* goals,
                         std::set<const Node*>& initialized);

  void CrystalUpdate(signed short val,
                     AState& top,
                     signed short& n_val,
                     size_t* m_set_size,
                     size_t* goals,
                     bool& terminal) const
  {
    if(val == CrystalConfig::CrystalIncrementVal)
    {
      top.Set(id, true);
    }
    else top.Set(id, false);
  }

  void CrystalUpdate(signed short val,
                     AState& top,
                     signed short& n_val,
                     MoveSet* m_set_size,
                     size_t* goals,
                     bool& terminal) const
  {
    if(val == CrystalConfig::CrystalIncrementVal)
    {
      top.Set(id, true);
    }
    else top.Set(id, false);
  }

  Node* GetCopy_only_info() const
  {
    return new NextNode(name, id, sim_type);
  }

  void RegisterToPropnet(PropNet& pn, Node* to_reg) const;

  size_t id;

  size_t num_true;
};

}
}
}


#endif // LIBGDL_PROPNET_NEXT_NODE_HPP_INCLUDED
