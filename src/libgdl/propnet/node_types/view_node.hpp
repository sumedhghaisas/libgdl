#ifndef LIBGDL_PROPNET_VIEW_NODE_HPP_INCLUDED
#define LIBGDL_PROPNET_VIEW_NODE_HPP_INCLUDED

#include <libgdl/core/util/to_string.hpp>

#include "sim_node.hpp"

namespace libgdl
{
namespace propnet
{

namespace node_types
{

struct ViewNode : public SimNode
{
  ViewNode(const std::string& name, size_t id, const SimType& sim_type = SimType::OR)
    : SimNode(name, Node::Type::VIEW, sim_type), id(id)
  {}

  std::string UName() const
  {
    std::string out = "VIEW_" + ToString(id);
    return out;
  }

  std::tuple<bool, size_t> CodeGen(compilation::EntryManager& em, size_t visit_stamp);

  bool CrystalInitialize(const PropNet& pn,
                         const std::map<const Node*, size_t>& id_map,
                         signed short* data,
                         AState& s,
                         MoveSet* m_set,
                         size_t* goals,
                         std::set<const Node*>& initialized);

  Node* GetCopy_only_info() const
  {
    return new ViewNode(name, id, sim_type);
  }

  void RegisterToPropnet(PropNet& pn, Node* to_reg) const;

  size_t id;

  size_t num_true;
};

}
}
}


#endif // LIBGDL_PROPNET_VIEW_NODE_HPP_INCLUDED
