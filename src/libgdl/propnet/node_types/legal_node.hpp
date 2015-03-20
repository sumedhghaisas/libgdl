#ifndef LIBGDL_PROPNET_LEGAL_NODE_HPP_INCLUDED
#define LIBGDL_PROPNET_LEGAL_NODE_HPP_INCLUDED

#include <libgdl/core/util/to_string.hpp>

#include "node.hpp"

namespace libgdl
{
namespace propnet
{
namespace node_types
{

struct LegalNode : public Node
{
  LegalNode(const std::string& name, size_t r_id, size_t id)
    : Node(name, Node::Type::LEGAL), r_id(r_id), id(id)
  {}

  std::string Name() const
  {
    return "LEGAL_" + ToString(r_id) + "_" + name;
  }

  std::string UName() const
  {
    std::string out = "LEGAL_" + ToString(r_id) + "_" + ToString(id);
    return out;
  }

  std::tuple<bool, size_t> CodeGen(EntryManager& em, size_t visit_map);

  bool InitializeValue(const PropNet&, AState& s, std::set<size_t>* m_set, size_t* goals);

  void Update(bool value, AState& base, AState& top, AMove& m, std::set<size_t>* m_set, size_t* goals);

  Node* GetCopy_only_info() const
  {
    return new LegalNode(name, r_id, id);
  }

  void RegisterToPropnet(PropNet& pn, Node* to_reg) const;

  size_t r_id;
  size_t id;

  size_t num_true;
};

}
}
}


#endif // LIBGDL_PROPNET_LEGAL_NODE_HPP_INCLUDED
