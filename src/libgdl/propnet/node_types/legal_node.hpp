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

  bool InitializeValue(const PropNet&, AState& s, Set<size_t>* m_set, size_t* goals);

  bool CrystalInitialize(const PropNet& pn, const std::map<const Node*, size_t>& id_map, signed short* data, AState& s, Set<size_t>* m_set, size_t* goals, std::set<const Node*>& initialized);

  void Update(bool value, AState& base, AState& top, AMove& m, Set<size_t>* m_set, size_t* goals);

  void CrystalUpdate(signed short val, AState& top, Set<size_t>* m_set, size_t* goals) const
  {
    if(val == 0x0001)
    {
      m_set[r_id].insert(id);
    }
    else m_set[r_id].erase(m_set[r_id].find(id));
  }

  void CrystalUpdate(signed short val, AState& top, signed short& mem, size_t* legal_size, size_t* goals) const
  {
    mem += val;
    if(val == 0x0001)
    {
      legal_size[r_id]++;
    }
    else
    {
      legal_size[r_id]--;
    }
  }

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
