#ifndef LIBGDL_PROPNET_NODE_HPP_INCLUDED
#define LIBGDL_PROPNET_NODE_HPP_INCLUDED

#include <list>
#include <string>
#include <tuple>
#include <set>
#include <map>

#include <libgdl/core.hpp>
#include <libgdl/core/data_types/a_state.hpp>
#include <libgdl/core/data_types/a_move.hpp>

#include <libgdl/propnet/propnet_payload.hpp>
#include <libgdl/propnet/crystallization/crystal_data.hpp>
#include <libgdl/propnet/compilation/entry_manager.hpp>
#include <libgdl/propnet/compilation/entry_types.hpp>

#include <boost/unordered_set.hpp>

//#define LIBGDL_DFP_TEST

namespace libgdl
{
namespace propnet
{

class PropNet;

namespace compilation
{

class EntryManager;

}

namespace node_types
{

struct Node
{
  typedef boost::unordered_set<unsigned short> MoveSet;
  typedef crystallization::CrystalData CrystalData;
  typedef crystallization::CrystalConfig CrystalConfig;

  enum class Type{BASE, INPUT, VIEW, OR, NEXT, LEGAL, TERMINAL, GOAL, AND, NOT};

  enum class SimType { OR, AND };

  Node(const std::string& name,
       const Type& type)
    : name(name), type(type), isVisited(false), visit_stamp(0), run_stamp(0),
    holding_value(true), mark(0)
  {}

  virtual ~Node() {}

  virtual std::tuple<bool, size_t> CodeGen(compilation::EntryManager& em,
                                           size_t visit_stamp) = 0;

  virtual bool CrystalInitialize(const PropNet& pn,
                                 const std::map<const Node*, size_t>& crystal_data_map,
                                 PropNetPayLoad& payload,
                                 std::set<const Node*>& initialized) = 0;

  virtual Node* GetCopy_only_info() const = 0;

  virtual void RegisterToPropnet(PropNet& pn, Node* to_reg) const = 0;

  virtual CrystalConfig::Type GetCrystalType() const = 0;

  virtual void CrystalUpdate(signed short val,
                             AState& top,
                             signed short& mem,
                             size_t* legal_size,
                             size_t* goals,
                             bool& terminal) const
  {
    std::cout << LOGID << "Unexpected error occured!" << std::endl;
    exit(1);
  }

  virtual void CrystalUpdate(signed short val,
                             AState& top,
                             signed short& mem,
                             MoveSet* m_set,
                             size_t* goals,
                             bool& terminal) const
  {
    std::cout << LOGID << "Unexpected error occured!" << std::endl;
    exit(1);
  }

  size_t Crystallize(std::map<const Node*, CrystalData>& data_map,
                     std::list<const Node*>& crystal_node_order,
                     size_t& current_index) const;

  Node* CreateCopy(PropNet& pn, Node* parent,
                   std::map<const Node*, Node*>& node_map) const;

  void DFSMark(size_t cm);

  void RemoveOutDegree(Node* out);
  void RemoveInDegree(Node* out);

  void DeleteIfNotMarked(Node* parent, std::set<Node*>& del, size_t cm);

  virtual Node* MergeWithChild(PropNet& pn) = 0;

  Node* Clone(Node* parent,
              PropNet& pn,
              std::map<const Node*, Node*>& copy_map) const;

  void AddIn(Node* n)
  {
    in_degree.push_back(n);
  }

  void AddOut(Node* n)
  {
    out_degree.push_back(n);
  }

  inline bool IsLegal() const
  {
    if(type == Type::LEGAL)
      return true;
    return false;
  }

  bool IsOr() const
  {
    if(type == Type::OR)
      return true;
    else return false;
  }

  bool IsAnd() const
  {
    if(type == Type::AND)
      return true;
    return false;
  }

  bool IsNot() const
  {
    if(type == Type::NOT)
      return true;
    return false;
  }

  virtual std::string Name() const
  {
    return name;
  }

  virtual std::string UName() const
  {
    std::string out = "";
    if(type == Type::BASE)
    {
      out += "BASE_";
    }

    out += name;
    return out;
  }

  std::string name;
  std::list<Node*> in_degree;
  std::list<Node*> out_degree;

  Type type;
  SimType sim_type = SimType::OR;

  bool isVisited;
  std::tuple<bool, size_t> entry_ret;
  size_t visit_stamp;

  size_t run_stamp;
  bool holding_value;

  static size_t node_count;

  size_t mark;
};

}
}
}

#endif // LIBGDL_PROPNET_NODE_HPP_INCLUDED
