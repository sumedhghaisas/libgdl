#ifndef LIBGDL_STATE_MACHINE_HPP_INCLUDED
#define LIBGDL_STATE_MACHINE_HPP_INCLUDED

#include <set>
#include <functional>

#include <boost/program_options.hpp>

#include <libgdl/core.hpp>
#include <libgdl/core/util/template_utils.hpp>
#include <libgdl/core/data_types/a_state.hpp>
#include <libgdl/core/data_types/a_move.hpp>
#include <libgdl/core/data_types/move_list.hpp>
#include <libgdl/core/data_types/move_vector.hpp>

#include <libgdl/propnet/propnet.hpp>
#include <libgdl/propnet/node_types.hpp>


#define D_META_SIM_TIME 1000000

namespace libgdl
{

class StateMachine
{
  typedef std::function<bool(const AState&)> IsTerminal_t;
  typedef std::function<MoveList<AMove>(const AState& s)> GetLegalMoves_l_t;
  typedef std::function<AState(const AState&, const AMove&)> GetNextState_t;
  typedef std::function<const size_t*(const AState&)> GetGoals_t;

  typedef void (GetGoals_m_t)(const AState& s, size_t* goals, bool* buff);

 public:
  explicit StateMachine(int argc, char* argv[]);

  IsTerminal_t IsTerminal;

  GetLegalMoves_l_t GetLegalMoves_l;

  GetNextState_t GetNextState;

  GetGoals_t GetGoals;

  const size_t* Simulate(const AState& s);
  const size_t* Simulate2(const AState& s);
  const size_t* Simulate3(const AState& s);

  void PrintMoveList(std::ostream& stream, const MoveList<AMove>& ml)
  {
    initial_pn.PrintMoveCollection(stream, ml);
  }

  void PrintMoveVector(std::ostream& stream, const MoveVector<AMove>& ml)
  {
    initial_pn.PrintMoveCollection(stream, ml);
  }

  void PrintState(std::ostream& stream, const AState& s)
  {
    initial_pn.PrintState(stream, s);
  }

  void PrintMove(std::ostream& stream, const AMove& m)
  {
    initial_pn.PrintMove(stream, m);
  }

  AState InitState() const
  {
    return init.Clone();
  }

  size_t GetRoleNum() const
  {
    return role_size;
  }

  void UpdateCrystal_base(const AState& state, const AState& mask, AState& top, AState& base, std::set<size_t>* m_set, size_t* goals);

  void UpdateCrystal_move(const AMove& move, AMove& base, AState& top, std::set<size_t>* m_set, size_t* goals);

 private:
  void SetInitialPropNet();

  void SeparateGoalNet(bool compile_goal_net);

  void MetaGame(size_t simulation_time);

  void CheckZeroSumGame();

  void MetaGame_multi_player(size_t simulation_time);

  void SeparateRolePropNets();

  size_t base_size;
  size_t role_size;

  AState init;

  size_t* goals;

  //! Initial propnet
  propnet::PropNet initial_pn;
  AState initial_pn_base_mask;
  AState initial_pn_base;
  AState initial_pn_top;
  AMove initial_pn_base_move;
  propnet::node_types::Node** initial_pn_base_nodes;
  propnet::node_types::Node*** initial_pn_input_nodes;
  std::set<size_t>* initial_pn_legals;

  std::map<const propnet::node_types::Node*, size_t> id_map;
  size_t terminal_crystal_id = 0;

  //! Function with initial propnet
  bool IsTerminal_initial_dfp(const AState& s);
  MoveList<AMove> GetLegalMoves_l_initial_dfp(const AState& s);
  AState GetNextState_initial_dfp(const AState& s, const AMove& m);
  const size_t* GetGoal_initial_dfp(const AState& s);

  //! Goal propnet
  propnet::PropNet goal_pn;
  AState goal_pn_base_mask;
  AState goal_pn_top;
  AState goal_pn_base;

  propnet::node_types::Node** goal_pn_base_nodes = NULL;
  GetGoals_m_t* GetGoals_m = NULL;
  bool* GetGoals_buff = NULL;

  //! Functions with goal net
  const size_t* GetGoal_goal_dfp(const AState& s);
  const size_t* GetGoals_goal_m(const AState& s);

  //! Role propnets
  propnet::PropNet** role_propnets = NULL;
  AState* role_pn_base_mask = NULL;
  AState* role_pn_base = NULL;
  AState* role_pn_top = NULL;
  AMove* role_pn_base_move = NULL;
  propnet::node_types::Node*** role_pn_base_nodes = NULL;
  propnet::node_types::Node**** role_pn_input_nodes = NULL;
  std::set<size_t>** role_pn_legals = NULL;

  bool is_goal_propnet_used = false;
  bool is_goal_propnet_compiled = false;
  bool is_propnet_role_separated = false;

  bool isZeroSumGame = false;
  bool isAlternatingMoves = false;

  AState* alt_role_masks = NULL;

   //! Logging stream
  mutable Log log;

};

}


#endif // LIBGDL_STATE_MACHINE_HPP_INCLUDED
