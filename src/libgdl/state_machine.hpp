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

#include <boost/unordered_set.hpp>

#define D_META_SIM_TIME 1000000

namespace libgdl
{

class StateMachine
{
  template<typename T>
  using Set = boost::unordered::unordered_set<T>;

  typedef std::function<bool(const AState&)> IsTerminal_t;
  typedef std::function<MoveList<AMove>(const AState& s)> GetLegalMoves_l_t;
  typedef std::function<AState(const AState&, const AMove&)> GetNextState_t;
  typedef std::function<const size_t*(const AState&)> GetGoals_t;

  typedef void (GetGoals_m_t)(const AState& s, size_t* goals, bool* buff);

 public:
  explicit StateMachine(int argc, char* argv[]);

  ~StateMachine();

  GetLegalMoves_l_t GetLegalMoves_l;

  GetNextState_t GetNextState;

  GetGoals_t GetGoals;

  const size_t* Simulate(const AState& s);
  const size_t* Simulate2(const AState& s);
  inline const size_t* Simulate3(const AState& s);
  inline const size_t* Simulate4(const AState& s);
  inline const size_t* Simulate5(const AState& s);
  inline const size_t* Simulate6(const AState& s);

  AState InitState() const
  {
    return init.Clone();
  }

  size_t GetRoleNum() const
  {
    return role_size;
  }

  //inline void UpdateCrystal_base(const AState& state, const AState& mask, AState& top, AState& base, std::set<size_t>* m_set, size_t* goals);

  //inline void UpdateCrystal_move(const AMove& move, AMove& base, AState& top, std::set<size_t>* m_set, size_t* goals);

  static size_t stack_time;
  size_t n_count = 0;

 private:
  void FinalizeInitialPropNet();

  void SeparateGoalNet(bool compile_goal_net);

  void MetaGame(size_t simulation_time);

  void CheckZeroSumGame();

  void MetaGame_multi_player(size_t simulation_time);

  void SeparateRolePropNets();

  size_t base_size;
  size_t role_size;

  AState init;

  //! Initial propnet
  propnet::PropNet initial_pn;
  propnet::PropNet::PayLoadType* initial_pn_payload = NULL;

  //! Function with initial propnet
  bool IsTerminal_initial_dfp(const AState& s);
  MoveList<AMove> GetLegalMoves_l_initial_dfp(const AState& s);
  AState GetNextState_initial_dfp(const AState& s, const AMove& m);
  const size_t* GetGoal_initial_dfp(const AState& s);

  //! Goal propnet
  propnet::PropNet goal_pn;
  propnet::PropNet::PayLoadType* goal_pn_payload;

  GetGoals_m_t* GetGoals_m = NULL;

  bool* GetGoals_buff = NULL;

  //! Functions with goal net
  const size_t* GetGoal_goal_dfp(const AState& s);
  const size_t* GetGoals_goal_m(const AState& s);

////////////////////////////////////////////////////////////////////////////////
/// Role propnets
////////////////////////////////////////////////////////////////////////////////
  //! BASIC
  propnet::PropNet** role_propnets = NULL;
  propnet::PropNet::PayLoadType** role_propnet_payloads = NULL;

////////////////////////////////////////////////////////////////////////////////
/// Propnet Characteristics
////////////////////////////////////////////////////////////////////////////////

  bool is_goal_propnet_used = false;
  bool is_goal_propnet_compiled = false;
  bool is_propnet_role_separated = false;

////////////////////////////////////////////////////////////////////////////////
/// Game characteristics
////////////////////////////////////////////////////////////////////////////////

  bool isZeroSumGame = false;
  bool isAlternatingMoves = false;

////////////////////////////////////////////////////////////////////////////////
/// Stack which are used for simulations
////////////////////////////////////////////////////////////////////////////////

  unsigned short* n_stack = new unsigned short[10000];
  signed short* v_stack = new signed short[10000];

  AState temp = AState("");
  AMove m = AMove("");

   //! Logging stream
  mutable Log log;
};

}

#include "state_machine_impl.hpp"


#endif // LIBGDL_STATE_MACHINE_HPP_INCLUDED
