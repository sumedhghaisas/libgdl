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
#include "istate.hpp"
#include "imove.hpp"

#include <libgdl/core.hpp>

#include <boost/unordered_set.hpp>

namespace libgdl
{
class StateMachine;

namespace propnet
{

struct CrystalNode
{
  bool type;
  unsigned char out_size;
  unsigned short offset;
};

class PropNet
{
  template<typename T>
  using Set = boost::unordered::unordered_set<T>;

  template<typename T1, typename T2>
  using Map = std::map<T1, T2>;

  typedef node_types::Node Node;

 public:
  explicit PropNet(Log log = GLOBAL_LOG);

  PropNet(const PropNet& pn);

   ~PropNet();

  void Initialize(const std::string& filename);
  void Initialize(gdlparser::KIF& kif);

  bool InitializeWithDOT(const gdlparser::KIF& kif,
                         const std::string& dot_filename);

  void PrimaryRun(AState& s, Set<size_t>* m_set, size_t* goals);

  void AddFact(const core::Fact& f);
  void AddClause(const core::Clause& c);

  bool PrintPropnet(const std::string& filename) const;

  void GenerateStateMachineCode();

  void GenerateMoveCode(std::ostream& stream);

  void GenerateStateCode(std::ostream& stream);

  void GenerateStateMachine();

  std::map<const Node*, size_t> Crystallize(signed short*& data_init, AState& top, Set<size_t>* m_set, size_t* goals);

  std::string CreateGetGoalMachineCode();

  void SplitGoalNet(PropNet& pn);

  size_t BaseSize() const
  {
    return base_size;
  }

  size_t RoleSize() const
  {
    return role_size;
  }

  template<typename StateType>
  void InitState(StateType& init);

  template<typename StateType>
  void GetPropNetBaseMask(StateType& s);

  bool IsInitProp(size_t id) const
  {
    for(auto it : init_props)
      if(it == id) return true;
    return false;
  }

  Map<size_t, Node*>& BaseNodes()
  {
    return base_nodes;
  }

  const std::vector<Map<size_t, Node*>>& InputNodes() const
  {
    return input_nodes;
  }

  const Map<std::string, Node*>& ViewNodes() const
  {
    return view_nodes;
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

  void AddTerminalNode(Node* n) { terminal = n; }

  void AddViewNode(Node* n, const std::string& name) { view_nodes[name] = n; }

  Node* GetTerminalNode()
  {
    return terminal;
  }

  inline void CrystalUpdate_base(const AState& state, AState& base, AState& top, Set<size_t>* m_set, size_t* goals, signed short* data, unsigned short* n_stack, signed short* v_stack) const;

  inline void CrystalUpdate_input(const AMove& move, AMove& base, AState& top, Set<size_t>* m_set, size_t* goals, signed short* data, unsigned short* n_stack, signed short* v_stack) const;

  inline void CrystalUpdate_input2(const AMove& move, AMove& base, AState& top, bool** m_set, size_t* legal_size, size_t* goals, signed short* data, unsigned short* n_stack, signed short* v_stack) const;

  inline void CrystalUpdate_base2(const AState& state, AState& base, AState& top, bool** m_set, size_t* legal_size, size_t* goals, signed short* data, unsigned short* n_stack, signed short* v_stack) const;

  inline void UpdateNormal_base(const AState& state, AState& base, AState& top, Set<size_t>* m_set, size_t* goals) const;

  inline void UpdateNormal_input(const AMove& move, AState& base, AState& top, AMove& m, Set<size_t>* m_set, size_t* goals) const;

  size_t terminal_crystal_id = 0;

  size_t data_init_size = 0;

  size_t GetNumComponents() const
  {
    size_t out = 0;

    for(auto it : base_nodes)
      if(del.find(it.second) == del.end())
        out++;

    for(auto it : input_nodes)
      for(auto it2 : it)
        if(del.find(it2.second) != del.end())
          out++;

    for(auto it : goal_nodes)
      for(auto it2 : it)
        if(del.find(it2.second) == del.end())
          out++;

    for(auto it : legal_nodes)
      for(auto it2 : it)
        if(del.find(it2.second) != del.end())
          out++;

    for(auto it : and_nodes)
      if(del.find(it) == del.end())
        out++;

    for(auto it : or_nodes)
      if(del.find(it) == del.end())
        out++;

    for(auto it : not_nodes)
      if(del.find(it) == del.end())
        out++;

    for(auto it : view_nodes)
      if(del.find(it.second) == del.end())
        out++;

    for(auto it : next_nodes)
      if(del.find(it.second) == del.end())
        out++;

    out++;

    return out;
  }

  static size_t n_count;

 private:
  void GenerateSeriesFunctions(size_t mark_index);

  void InitializePrintFunctions() const;


////////////////////////////////////////////////////////////////////////////////
/// Helpers for propnet creation and storage
////////////////////////////////////////////////////////////////////////////////

  void CreatePropNet(gdlreasoner::KIFFlattener& kf);

  node_types::Node* CreateNode(core::SymbolTable sym, const core::Argument* arg);

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

  size_t c_r_id = 0;
  size_t c_and_id = 0;
  size_t c_not_id = 0;
  size_t c_or_id = 0;
  size_t c_view_id = 0;
  size_t c_base_id = 0;
  std::vector<size_t> c_input_id;

  core::SymbolTable sym;

  signed short** role_data_init;

////////////////////////////////////////////////////////////////////////////////
/// Normal simulator data
////////////////////////////////////////////////////////////////////////////////

  Node** arr_base_nodes = NULL;
  Node*** arr_input_nodes = NULL;

////////////////////////////////////////////////////////////////////////////////
/// Crystallization DATA
////////////////////////////////////////////////////////////////////////////////

  std::map<const propnet::node_types::Node*, size_t> id_map;
  CrystalNode* cry = NULL;
  unsigned short* out_degree = NULL;
  size_t* base_crystal_ids = NULL;
  size_t** input_crystal_ids = NULL;

////////////////////////////////////////////////////////////////////////////////
/// Configuration parameters and common important variables
////////////////////////////////////////////////////////////////////////////////

  AState base_mask;
  size_t base_size;
  size_t role_size;
  bool isCrystalized = false;

  unsigned short* t_data = new unsigned short[10000];

  mutable Log log;
};

}
}

#include "propnet_impl.hpp"

#endif // LIBGDL_PROPNET_HPP_INCLUDED
