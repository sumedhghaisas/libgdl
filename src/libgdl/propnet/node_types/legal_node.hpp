#ifndef LIBGDL_PROPNET_LEGAL_NODE_HPP_INCLUDED
#define LIBGDL_PROPNET_LEGAL_NODE_HPP_INCLUDED

#include <libgdl/core/util/to_string.hpp>

#include "sim_node.hpp"

namespace libgdl
{
namespace propnet
{
namespace node_types
{

struct LegalNode : public SimNode
{
  LegalNode(const std::string& name, size_t r_id, size_t id, const SimType& sim_type = SimType::OR)
    : SimNode(name, Node::Type::LEGAL, sim_type), r_id(r_id), id(id)
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

  std::tuple<bool, size_t> CodeGen(compilation::EntryManager& em, size_t visit_map);

  bool CrystalInitialize(const PropNet& pn,
                         const std::map<const Node*, size_t>& id_map,
                         PropNetPayLoad& payload,
                         std::set<const Node*>& initialized);

  void CrystalUpdate(signed short val,
                     AState& top,
                     signed short& mem,
                     size_t* legal_size,
                     size_t* goals,
                     bool& terminal) const
  {
    if(val == CrystalConfig::CrystalIncrementVal)
    {
      legal_size[r_id]++;
    }
    else
    {
      legal_size[r_id]--;
    }
  }

  void CrystalUpdate(signed short val,
                     AState& top,
                     signed short& mem,
                     MoveSet* m_set,
                     size_t* goals,
                     bool& terminal) const
  {
    if(val == CrystalConfig::CrystalIncrementVal)
    {
      m_set[r_id].insert(id);
    }
    else
    {
      m_set[r_id].erase(m_set[r_id].find(id));
    }
  }

  Node* GetCopy_only_info() const
  {
    return new LegalNode(name, r_id, id, sim_type);
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
