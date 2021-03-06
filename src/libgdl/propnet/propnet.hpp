#ifndef LIBGDL_PROPNET_HPP_INCLUDED
#define LIBGDL_PROPNET_HPP_INCLUDED

#include <boost/unordered_map.hpp>
#include <map>
#include <vector>
#include <list>
#include <set>

#include <libgdl/core.hpp>
#include <libgdl/core/data_types/a_state.hpp>
#include <libgdl/core/data_types/a_move.hpp>
#include <libgdl/reasoners/gdlreasoner/kif_flattener.hpp>
#include <libgdl/gdlparser/kif.hpp>

#include "node_types.hpp"
#include "propnet_payload.hpp"

namespace libgdl
{

namespace propnet
{

struct CrystalNode
{
  bool type;
  unsigned char out_size;
  unsigned short data_id;
  unsigned short out_edges[0];
};

class PropNet
{
  template<typename T1, typename T2>
  using Map = std::map<T1, T2>;

  typedef node_types::Node Node;

  const static size_t PayloadStackSize = 1000;

 public:
  typedef AState StateType;
  typedef AMove MoveType;
  typedef boost::unordered_set<unsigned short> MoveSet;

  struct PropNetPayLoad2
  {
    StateType top;
    StateType base;
    MoveType base_move;
    MoveSet* m_set = NULL;
    size_t* goals = NULL;
    signed short* data = NULL;
    int* t_stack = NULL;
    bool terminal;
  };

  typedef PropNetPayLoad PayLoadType;
  typedef PropNetPayLoad2 PayLoadType2;

  explicit PropNet(Log log = GLOBAL_LOG) : log(log) {}

  PropNet(const PropNet& pn);

   ~PropNet();

  void Initialize(const std::string& filename);
  void Initialize(gdlparser::KIF& kif);
  void Initialize(const PropNet& pn);

  bool InitializeWithDOT(const gdlparser::KIF& kif,
                         const std::string& dot_filename);

  void InitState(StateType& init);

  void SplitGoalNet(PropNet& pn);

  void SplitTerminalNet(PropNet& pn);

  void Finalize();

  std::string CreateGetGoalMachineCode();
  std::string CreateIsTerminalMachineCode();

  PayLoadType* GetPayLoadInstance() const;
  PayLoadType2* GetPayLoadInstance2() const;

  inline void GetRandomLegalMove(const PayLoadType& payload, AMove& m) const;

  void AddFact(const core::Fact& f);
  void AddClause(const core::Clause& c);

  bool PrintPropnet(const std::string& filename) const;

  void GetPropNetBaseMask(StateType& s);

  bool IsInitProp(size_t id) const
  {
    for(auto it : init_props)
      if(it == id) return true;
    return false;
  }

  void InitializeToRoles(size_t num_roles)
  {
    for(size_t i = 0;i < num_roles;i++)
    {
      goal_nodes.emplace_back();
      input_nodes.emplace_back();
      legal_nodes.emplace_back();
    }
  }

  void ProcessDOTtoken(const std::string& token,
                       std::list<std::pair<std::string, std::string>>& edges,
                       std::map<std::string, std::string>& t_edges,
                       std::map<std::string, Node*>& nodes,
                       std::map<std::string, Node*>& n_nodes);

  void AddAndNode(Node* n) { and_nodes.push_back(n); }

  void AddBaseNode(Node* n, size_t id) { base_nodes[id] = n; }

  void AddGoalNode(Node* n, size_t r_id, size_t id) { goal_nodes[r_id][id] = n; }

  void AddInputNode(Node* n, size_t r_id, size_t id) { input_nodes[r_id][id] = n; }

  void AddLegalNode(Node* n, size_t r_id, size_t id) { legal_nodes[r_id][id] = n; }

  void AddNextNode(Node* n, size_t id) { next_nodes[id] = n; }

  void AddNotNode(Node* n) { not_nodes.push_back(n); }

  void AddOrNode(Node* n) { or_nodes.push_back(n); }

  void AddTerminalNode(Node* n)
  {
    terminal = n;
  }

  void AddViewNode(Node* n, const std::string& name) { view_nodes[name] = n; }

////////////////////////////////////////////////////////////////////////////////
/// Propnet simulation functions
////////////////////////////////////////////////////////////////////////////////

  inline void Update(const MoveType& move, PayLoadType& payload) const;

  inline bool Update(const StateType& state, PayLoadType& payload) const;

  inline void Update2(const MoveType& move, PayLoadType2& payload) const;

  inline bool Update2(const StateType& state, PayLoadType2& payload) const;

  inline void Update3(MoveSet::const_iterator* move, PayLoadType2& payload) const;

////////////////////////////////////////////////////////////////////////////////
/// Getter-setter functions
////////////////////////////////////////////////////////////////////////////////

  size_t GetNumComponents() const;
  size_t GetNumAndComponents() const;
  size_t GetNumOrComponents() const;
  size_t GetNumNotComponenets() const;

  size_t GetCrystalDataSize() const
  {
    return crystal_data_init_size;
  }

  inline size_t GetCrystalTerminalID() const
  {
    return terminal_crystal_id;
  }

  Node* GetTerminalNode()
  {
    return terminal;
  }

  const Map<size_t, Node*>& BaseNodes() const
  {
    return base_nodes;
  }

  const std::vector<Map<size_t, Node*>>& InputNodes() const
  {
    return input_nodes;
  }

  const std::vector<Map<size_t, Node*>>& LegalNodes() const
  {
    return legal_nodes;
  }

  const Map<std::string, Node*>& ViewNodes() const
  {
    return view_nodes;
  }

  size_t BaseSize() const
  {
    return base_size;
  }

  size_t RoleSize() const
  {
    return role_size;
  }

  bool IsCrystallized() const
  {
    return isCrystalized;
  }

  size_t GetRoleID(const std::string& role) const
  {
    return roles_ids.find(role)->second;
  }

  void OptimizeWithNodeMerge();

  PropNet* OptimizeWithRoleMask(const StateType& mask);

  static size_t debug_time;

 private:
  friend Node;
  friend node_types::SimNode;
  friend node_types::NotNode;

  void Crystallize(std::map<const Node*, crystallization::CrystalData>& data_map,
                   std::list<const Node*>& crystal_node_order);

  void FillCrystal(std::map<const Node*, crystallization::CrystalData>& crystal_data_map,
                   std::list<const Node*>& crystal_node_order,
                   std::map<unsigned short, unsigned short>& id_to_mem);

  void InitializePrintFunctions() const;

////////////////////////////////////////////////////////////////////////////////
/// Store propnet as node graph
////////////////////////////////////////////////////////////////////////////////

  std::map<std::string, size_t> roles_ids;

  Map<std::string, Node*> view_nodes;

  std::vector<Map<std::string, size_t>> str_input_nodes;
  std::vector<Map<size_t, Node*>> input_nodes;
  std::vector<Map<size_t, Node*>> legal_nodes;

  std::vector<Map<size_t, Node*>> goal_nodes;
  Node* terminal = NULL;

  Map<std::string, size_t> str_base_nodes;
  Map<size_t, Node*> base_nodes;
  Map<size_t, Node*> next_nodes;

  std::list<Node*> and_nodes;
  std::list<Node*> not_nodes;
  std::list<Node*> or_nodes;

  std::list<size_t> init_props;

  std::set<Node*> del;

////////////////////////////////////////////////////////////////////////////////
/// Helpers for propnet creation and storage
////////////////////////////////////////////////////////////////////////////////

  void CreatePropNet(gdlreasoner::KIFFlattener& kf);

  node_types::Node* CreateNode(core::SymbolTable sym, const core::Argument* arg);

  size_t c_r_id = 0;
  size_t c_and_id = 0;
  size_t c_not_id = 0;
  size_t c_or_id = 0;
  size_t c_view_id = 0;
  size_t c_base_id = 0;
  std::vector<size_t> c_input_id;

  core::SymbolTable sym;

////////////////////////////////////////////////////////////////////////////////
/// CRYSTAL Simulator data
////////////////////////////////////////////////////////////////////////////////

  unsigned short* arr_propnet = NULL;

  unsigned short* base_crystal_ids = NULL;
  unsigned short** input_crystal_ids = NULL;
  unsigned short** legal_memory_ids = NULL;

  size_t crystal_data_init_size = 0;

  size_t terminal_crystal_id = 0;

////////////////////////////////////////////////////////////////////////////////
/// Configuration parameters and common important variables
////////////////////////////////////////////////////////////////////////////////

  StateType base_mask;
  size_t base_size;
  size_t role_size;
  bool isCrystalized = false;
  PayLoadType default_payload;
  PayLoadType2 default_payload2;

  mutable Log log;
};

}
}

#include "propnet_impl.hpp"

#endif // LIBGDL_PROPNET_HPP_INCLUDED
