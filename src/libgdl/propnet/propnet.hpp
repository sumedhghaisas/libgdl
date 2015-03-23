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

namespace libgdl
{
class StateMachine;

namespace propnet
{

class PropNet
{
  template<typename T1, typename T2>
  using Map = std::map<T1, T2>;

  typedef node_types::Node Node;

 public:
  explicit PropNet(Log log = GLOBAL_LOG);

  void Initialize(const std::string& filename);
  void Initialize(gdlparser::KIF& kif);

  void AddFact(const core::Fact& f);
  void AddClause(const core::Clause& c);

  ~PropNet();

  bool PrintPropnet(const std::string& filename) const;

  void GenerateStateMachineCode();

  void GenerateMoveCode(std::ostream& stream);

  void GenerateStateCode(std::ostream& stream);

  void GenerateStateMachine();

  std::string CreateGetGoalMachineCode();

  void SplitGoalNet(PropNet& pn);

  size_t BaseSize() const
  {
    return base_nodes.size();
  }

  size_t RoleSize() const
  {
    return roles_ids.size();
  }

  template<typename StateType>
  void InitState(StateType& init);

  template<typename StateType>
  void PrintState(std::ostream& stream, const StateType& s);

  template<typename MoveType>
  void PrintMove(std::ostream& stream, const MoveType& m);

  template<typename CType>
  void PrintMoveCollection(std::ostream& stream, const CType& mc);

  template<typename StateType>
  void GetPropNetBaseMask(StateType& s);

  void InitializeRun(AState& s, AState& base_mask, std::set<size_t>* m_set, size_t* goals);

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

  std::vector<Map<size_t, Node*>>& InputNodes()
  {
    return input_nodes;
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

  bool InitializeWithDOT(const gdlparser::KIF& kif,
                         const std::string& dot_filename);

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

 private:
  void CreatePropNet(gdlreasoner::KIFFlattener& kf);

  void GenerateSeriesFunctions(size_t mark_index);

  node_types::Node* CreateNode(core::SymbolTable sym, const core::Argument* arg);

  std::map<std::string, size_t> roles_ids;

  Map<std::string, Node*> view_nodes;
  Map<size_t, Node*> next_nodes;
  std::vector<Map<size_t, Node*>> input_nodes;
  std::vector<Map<size_t, Node*>> legal_nodes;
  std::vector<Map<size_t, Node*>> goal_nodes;
  Node* terminal;

  Map<size_t, Node*> base_nodes;

  std::list<Node*> and_nodes;
  std::list<Node*> not_nodes;
  std::list<Node*> or_nodes;

  IState istate;
  IMove imove;

  std::list<size_t> init_props;

  std::set<Node*> del;

  size_t c_r_id;
  size_t c_and_id;
  size_t c_not_id;
  size_t c_or_id;
  size_t c_view_id;

  core::SymbolTable sym;

  mutable Log log;
};

}
}

#include "propnet_impl.hpp"

#endif // LIBGDL_PROPNET_HPP_INCLUDED
