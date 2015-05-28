#ifndef LIBGDL_PROPNET_AND_NODE_HPP_INCLUDED
#define LIBGDL_PROPNET_AND_NODE_HPP_INCLUDED

#include <libgdl/core/util/to_string.hpp>

#include "node.hpp"

namespace libgdl
{
namespace propnet
{

namespace node_types
{

struct AndNode : public Node
{
  AndNode(const std::string& name, size_t id, bool isNand = false)
    : Node(name, Node::Type::AND), id(id), isNand(isNand)
  {}

  std::string UName() const
  {
    return "AND_" + ToString(id);
  }

  bool CrystalInitialize(const PropNet& pn,
                         const std::map<const Node*, size_t>& id_map,
                         signed short* data,
                         AState& s,
                         MoveSet* m_set,
                         size_t* goals,
                         std::set<const Node*>& initialized);

  std::tuple<bool, size_t> CodeGen(compilation::EntryManager& em, size_t v_stamp);

  Node* MergeWithChild(PropNet& pn)
  {
    if(in_degree.size() == 1)
      std::cout << "Yeah!" << std::endl;
    return NULL;
  }

  CrystalConfig::Type GetCrystalType() const
  {
    if(isNand)
      return CrystalConfig::Type::NAND;
    return CrystalConfig::Type::AND;
  }

  Node* GetCopy_only_info() const
  {
    return new AndNode(name, id, isNand);
  }

  void RegisterToPropnet(PropNet& pn, Node* to_reg) const;

  size_t id;
  size_t num_false;

  bool isNand = false;
};

}
}
}

#endif // LIBGDL_PROPNET_AND_NODE_HPP_INCLUDED
