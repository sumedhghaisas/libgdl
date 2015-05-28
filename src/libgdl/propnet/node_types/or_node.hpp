#ifndef LIBGDL_PROPNET_OR_NODE_HPP_INCLUDED
#define LIBGDL_PROPNET_OR_NODE_HPP_INCLUDED

#include <libgdl/core/util/to_string.hpp>

#include "node.hpp"

namespace libgdl
{
namespace propnet
{

namespace node_types
{

struct OrNode : public Node
{
  OrNode(const std::string& name, size_t id, bool isNOR = false)
    : Node(name, Node::Type::OR), id(id), isNOR(isNOR)
  {}

  std::string UName() const
  {
    return "OR_" + ToString(id);
  }

  std::tuple<bool, size_t> CodeGen(compilation::EntryManager& em, size_t visit_stamp);

  bool CrystalInitialize(const PropNet& pn,
                         const std::map<const Node*, size_t>& id_map,
                         signed short* data,
                         AState& s,
                         MoveSet* m_set,
                         size_t* goals,
                         std::set<const Node*>& initialized);

  Node* MergeWithChild(PropNet& pn)
  {
    if(in_degree.size() == 1);
      //std::cout << "Yeah!" << std::endl;
    return NULL;
  }

  CrystalConfig::Type GetCrystalType() const
  {
    if(isNOR)
      return CrystalConfig::Type::NOR;
    return CrystalConfig::Type::OR;
  }

  Node* GetCopy_only_info() const
  {
    return new OrNode(name, id, isNOR);
  }

  void RegisterToPropnet(PropNet& pn, Node* to_reg) const;

  size_t id;

  bool isNOR = false;
};

}
}
}


#endif // LIBGDL_PROPNET_OR_NODE_HPP_INCLUDED
