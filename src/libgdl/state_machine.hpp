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

#include <libgdl/propnet/propnet.hpp>
#include <libgdl/propnet/node_types.hpp>

namespace libgdl
{

class StateMachine
{
  typedef std::function<bool(const AState&)> IsTerminal_t;
  typedef std::function<MoveList<AMove>(const AState& s)> GetLegalMoves_l_t;
  typedef std::function<AState(const AState&, const AMove&)> GetNextState_t;
  typedef std::function<const size_t*(const AState&)> GetGoals_t;

 public:
  explicit StateMachine(int argc, char* argv[]);

  IsTerminal_t IsTerminal;

  GetLegalMoves_l_t GetLegalMoves_l;

  GetNextState_t GetNextState;

  GetGoals_t GetGoals;

  const size_t* Simulate(const AState& s);

  void PrintMoveList(std::ostream& stream, const MoveList<AMove>& ml)
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

 private:
  void SetInitialPropNet();
  void SeparateGoalNet();

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

   //! Logging stream
  mutable Log log;

};

}


#endif // LIBGDL_STATE_MACHINE_HPP_INCLUDED
