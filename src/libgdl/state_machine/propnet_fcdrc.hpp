#ifndef LIBGDL_STATE_MACHINE_HPP_INCLUDED
#define LIBGDL_STATE_MACHINE_HPP_INCLUDED

#include <set>
#include <functional>

#include <libgdl/core.hpp>
#include <libgdl/core/util/template_utils.hpp>
#include <libgdl/core/data_types/move_list.hpp>
#include <libgdl/core/data_types/move_vector.hpp>
#include <libgdl/propnet/propnet.hpp>

#include <boost/unordered_set.hpp>

namespace libgdl
{
namespace state_machine
{

/**
 * Forward Dead Reckoning Propnet State Machine
 */
class ForwardDeadReckoningPropnetStateMachine
{
 public:
  typedef propnet::PropNet ReasonerType;
  typedef ReasonerType::StateType StateType;
  typedef ReasonerType::MoveType MoveType;

 private:
  typedef boost::unordered_set<unsigned short> MoveSet;

  typedef std::function<bool(const StateType&,bool*)> IsTerminal_m_t;
  typedef std::function<MoveList<MoveType>(const StateType& s)> GetLegalMoves_l_t;
  typedef std::function<StateType(const StateType&, const MoveType&)> GetNextState_t;
  typedef std::function<const size_t*(const StateType&)> GetGoals_t;
  typedef std::function<std::list<StateType>(const StateType&) const> GetNextStates_t;

  typedef void (GetGoals_m_t)(const StateType& s, size_t* goals, bool* buff);

 public:
  explicit ForwardDeadReckoningPropnetStateMachine(int argc, char* argv[]);

  template<typename ConfigType>
  void Init(const gdlparser::KIF& kif, const ConfigType& config, size_t timeout);

  ~ForwardDeadReckoningPropnetStateMachine();

  bool Init();

  GetLegalMoves_l_t GetLegalMoves_l;

  template<class CollectionType>
  void GetLegalMoves(const StateType& state,
                     CollectionType& collection,
                     size_t& turn_role,
                     bool& is_terminal);

  StateType GetNextState(const StateType& s,
                         const MoveType& m);

  GetGoals_t GetGoals;

  IsTerminal_m_t IsTerminal_compiled;

  MoveType GetRandomLegalMove(const StateType& s);

  const size_t* Simulate(const StateType& s);
  const size_t* Simulate2(const StateType& s);
  inline const size_t* Simulate3(const StateType& s);
  inline const size_t* Simulate4(const StateType& s);
  const size_t* Simulate5(const StateType& s);
  const size_t* Simulate6(const StateType& s);

  AState InitState() const
  {
    return init.Clone();
  }

  inline size_t GetRoleNum() const
  {
    return role_size;
  }

  size_t GetRoleID(const std::string& role) const
  {
    return initial_pn.GetRoleID(role);
  }

  const size_t* GetGoals_goal_m(const StateType& s);

 private:
  void FinalizeInitialPropNet();

  void SeparateGoalNet(bool compile_goal_net);

  void MetaGame(size_t simulation_time);

  void CheckZeroSumGame();

  void MetaGame_multi_player(size_t simulation_time);

  void SeparateRolePropNets();

  size_t base_size;
  size_t role_size;

  StateType init;

  //! Initial propnet
  ReasonerType initial_pn;

  ReasonerType::PayLoadType* initial_pn_payload = NULL;
  ReasonerType::PayLoadType2* initial_pn_payload2 = NULL;

  ReasonerType* without_terminal_pn = NULL;
  ReasonerType::PayLoadType2* without_terminal_payload = NULL;

  //! Function with initial propnet
  bool IsTerminal_initial_dfp(const StateType& s);
  MoveList<MoveType> GetLegalMoves_l_initial_dfp(const StateType& s);
  StateType GetNextState_initial_dfp(const StateType& s, const MoveType& m);
  const size_t* GetGoal_initial_dfp(const StateType& s);
  bool CompiledIsTerminal(const StateType& state) const;

  //! Goal propnet
  ReasonerType goal_pn;
  ReasonerType::PayLoadType* goal_pn_payload = NULL;

  GetGoals_m_t* GetGoals_m = NULL;

  bool* GetGoals_buff = NULL;
  bool* IsTerminal_buff = NULL;

  //! Functions with goal net
  const size_t* GetGoal_goal_dfp(const StateType& s);


////////////////////////////////////////////////////////////////////////////////
/// Role propnets
////////////////////////////////////////////////////////////////////////////////
  //! BASIC
  ReasonerType* role_propnets = NULL;
  ReasonerType::PayLoadType** role_propnet_payloads = NULL;

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

  StateType* alt_role_masks = NULL;

  MoveSet::const_iterator* simulate2_it = NULL;
  StateType* simulate2_state_arr = new StateType[100];

   //! Logging stream
  mutable Log log;
};

}
}

#include "propnet_fcdrc_impl.hpp"

#endif // LIBGDL_STATE_MACHINE_HPP_INCLUDED
