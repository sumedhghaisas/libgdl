#include "state_machine.hpp"

#include <stdlib.h>
#include <time.h>

#include <stdio.h>
#include <dlfcn.h>

#include <libgdl/core/thread_pool/thread_pool.hpp>
#include <libgdl/gdlparser/kif.hpp>
#include <libgdl/gdlreasoner/kif_flattener.hpp>

using namespace std;
using namespace libgdl;
using namespace libgdl::util;
using namespace libgdl::propnet;
using namespace boost::program_options;

StateMachine::StateMachine(int argc, char* argv[])
  : initial_pn(GLOBAL_LOG),
  goal_pn(GLOBAL_LOG),
  GetGoals_buff(NULL),
  is_goal_propnet_used(false),
  is_goal_propnet_compiled(false),
  isZeroSumGame(false),
  log(GLOBAL_LOG)
{
  bool isDot = false;
  string dot_file = "";

  bool separatePropNetForGoals = false;
  bool compile_goal_net = false;

  size_t meta_simulation_time = D_META_SIM_TIME;

  // Declare the supported options.
  options_description desc("Allowed options");
  desc.add_options()
  ("source-files,c", value<std::vector<std::string> >()->multitoken(),
                     "source files")
  ("dot-file,d", value<std::string>(), "DOT PropNet file")
  ("separate-goal-net", "Create separate propnet for goals.")
  ("compile-goal-net", "Compile the goal net.")
  ("meta-simulation-time", value<size_t>(), "Assign time for meta-game simulation(in micro seconds).")
  ;

  variables_map vm;
  store(command_line_parser(argc, argv).
        options(desc).run(), vm);
  notify(vm);

  vector<string> source_files;

  if(vm.count("source-files"))
  {
    source_files = vm["source-files"].as<std::vector<std::string>>();
  }
  else
  {
    log.Fatal << "ERROR: at least one source file has to be mentioned." << std::endl;
    log.Fatal << "USAGE : StateMachine -c SOURCE_FILES" << std::endl;
    exit(1);
  }

  if(vm.count("dot-file"))
  {
    isDot = true;
    dot_file = vm["dot-file"].as<string>();
  }

  if(vm.count("separate-goal-net"))
  {
    separatePropNetForGoals = true;
  }
  if(vm.count("compile-goal-net"))
  {
    compile_goal_net = true;
  }

  if(vm.count("meta-simulation-time"))
  {
    meta_simulation_time = vm["meta-simulation-time"].as<size_t>();
  }

  gdlparser::KIF kif(true, 1, log);
  for(auto it : source_files)
    kif.AddFile(it);
  kif.Parse();

  if(!isDot)
  {
    //! Initialize propnet with the file given
    initial_pn.Initialize(kif);
  }
  else
    initial_pn.InitializeWithDOT(kif, dot_file);

  //! Get base size and role size
  base_size = initial_pn.BaseSize();
  role_size = initial_pn.RoleSize();

  //! Initialize AState with base size
  if(base_size % 8 == 0)
    AState::RawType::arr_size = base_size / 8;
  else
    AState::RawType::arr_size = base_size / 8 + 1;

  //! Initialize AMove with role size
  AMove::RawType::n_roles = role_size;

  //! Initialize Init State
  init = AState("");
  initial_pn.InitState(init);

  //! Assign enough memory for array which holds goals
  goals = new size_t[role_size];

  SetInitialPropNet();

  MetaGame(meta_simulation_time);

  initial_pn.PrintPropnet("test.dot");

  if(separatePropNetForGoals)
  {
    SeparateGoalNet(compile_goal_net);
  }

  //thread_pool::ThreadPool::GetGlobalThreadPool().Stop();

//
//  AState test = cache.Clone();
//
//  AMove m_cache("");
//  for(size_t i = 0;i < full_pn.RoleSize();i++)
//    m_cache->moves[i] = 0;
//
//  full_pn.PrintState(cout, test);
//
//  m.UpdateNodes(m_cache, input_nodes, cache, m_set, goals);
//
//  full_pn.PrintState(cout, cache);
//
//  cache.UpdateNodes(init, test, base_nodes, m_set, goals);
//
//  ml = MoveList<AMove>(m_set, full_pn.RoleSize());
//
//  full_pn.PrintMoveCollection(cout, ml);
//
//  full_pn.PrintPropnet("test.dot");

  //AState term = init.Clone();
  //term.Set(21, true);

  //full_pn.PrintState(cout, term);

  //term.UpdateNodes(init, base_nodes, m_set);
}

void StateMachine::MetaGame(size_t simulation_time)
{
  if(role_size > 1)
  {
    MetaGame_multi_player(simulation_time);
  }
  else
  {
    log.Info << "Single player game." << endl;
  }
}

void StateMachine::MetaGame_multi_player(size_t simulation_time)
{
  isZeroSumGame = true;

  size_t num_simulations = 0;

  size_t stop = Timer::microtimer() + simulation_time;

  while(Timer::microtimer() < stop)
  {
    AState temp = InitState().Clone();

    temp.UpdateNodes(initial_pn_base, initial_pn_top, initial_pn_base_mask, initial_pn_base_move, initial_pn_base_nodes, initial_pn_legals, goals);

    bool is_terminal = initial_pn.GetTerminalNode()->holding_value;

    AMove m("");

    while(!is_terminal)
    {
      MoveList<AMove> ml = MoveList<AMove>(initial_pn_legals, role_size);

      size_t rnd = rand() % ml.size();

      MoveList<AMove>::iterator it = ml.begin();
      for(size_t i = 0;i < rnd;i++)
        it++;

      m = *it;

      m.UpdateNodes(initial_pn_base, initial_pn_top, initial_pn_base_move, initial_pn_input_nodes, NULL, NULL);

      temp = initial_pn_top.Clone();

      temp.UpdateNodes(initial_pn_base, initial_pn_top, initial_pn_base_mask, initial_pn_base_move, initial_pn_base_nodes, initial_pn_legals, goals);

      is_terminal = initial_pn.GetTerminalNode()->holding_value;
    }

    if(isZeroSumGame)
      CheckZeroSumGame();

    num_simulations++;
  }

  log.Info << "Performed " << num_simulations * 1e6 / simulation_time << "simulations/sec before optimizing." << endl;

  if(isZeroSumGame)
  {
    log.Info << "Game is Zero-Sum game." << endl;
  }

  if(isZeroSumGame)
  {

  }
}

void StateMachine::CheckZeroSumGame()
{
  size_t sum = 0;
  for(size_t i = 0;i < role_size;i++)
    sum += goals[i];

  if(sum != 100)
    isZeroSumGame = false;
}

void StateMachine::SetInitialPropNet()
{
  //! Set initial propnet base
  initial_pn_base = init.Clone();

  //! Set initial propnet top
  initial_pn_top = AState("");

  //! Set initial propnet move base
  initial_pn_base_move = AMove("");
  for(size_t i = 0;i < role_size;i++)
    initial_pn_base_move->moves[i] = 0;

  //! Assign memory for sets which holds legal moves
  initial_pn_legals = new set<size_t>[role_size];

  //! Initialize the nodes in the propnet
  //! Also get the base_mask for this propnet
  initial_pn_base_mask = AState("");
  initial_pn.InitializeRun(initial_pn_top, initial_pn_base_mask, initial_pn_legals, goals);

  //! Initialize base nodes for initial_pn
  initial_pn_base_nodes = new node_types::Node*[base_size];
  for(size_t i = 0;i < base_size;i++)
    initial_pn_base_nodes[i] = NULL;
  for(auto it : initial_pn.BaseNodes())
  {
    initial_pn_base_nodes[it.first] = it.second;
  }

  //! Initialize input nodes for initial_pn
  initial_pn_input_nodes = new node_types::Node**[role_size];
  size_t index = 0;
  for(auto it : initial_pn.InputNodes())
  {
    initial_pn_input_nodes[index] = new node_types::Node*[it.size()];
    for(auto it2 : it)
    {
      initial_pn_input_nodes[index][it2.first] = it2.second;
    }
    index++;
  }

  IsTerminal = core::template_utils::BindToObject(&StateMachine::IsTerminal_initial_dfp, this);

  GetLegalMoves_l = core::template_utils::BindToObject(&StateMachine::GetLegalMoves_l_initial_dfp, this);

  GetNextState = core::template_utils::BindToObject(&StateMachine::GetNextState_initial_dfp, this);

  GetGoals = core::template_utils::BindToObject(&StateMachine::GetGoal_initial_dfp, this);
}

void StateMachine::SeparateGoalNet(bool compile_goal_propnet)
{
  is_goal_propnet_used = true;

  //! Initialize goal_net to number of roles
  goal_pn.InitializeToRoles(role_size);

  //! Split the goal net from full net
  initial_pn.SplitGoalNet(goal_pn);

  //! Set base for goal pn
  goal_pn_base = init.Clone();

  //! Set top for goal pn
  goal_pn_top = AState("");

  //! Initialize goal pn and get base mask
  goal_pn_base_mask = AState("");
  goal_pn.InitializeRun(goal_pn_top, goal_pn_base_mask, NULL, goals);

  //PrintState(cout, goal_pn_base_mask);

  //! Initialize base nodes for goal_pn
  goal_pn_base_nodes = new node_types::Node*[base_size];
  for(size_t i = 0;i < base_size;i++)
    goal_pn_base_nodes[i] = NULL;
  for(auto it : goal_pn.BaseNodes())
  {
    goal_pn_base_nodes[it.first] = it.second;
  }

  GetGoals = core::template_utils::BindToObject(&StateMachine::GetGoal_goal_dfp, this);

  if(compile_goal_propnet)
  {
    std::string object = goal_pn.CreateGetGoalMachineCode();

    void* handle = dlopen(object.c_str(), RTLD_NOW);
    if (!handle)
    {
      log.Warn << LOGID << "The error is " << dlerror() << endl;
      return;
    }

    typedef size_t (GetGoalMemoryRequirement_t)();

    GetGoals_m = (GetGoals_m_t*)dlsym(handle, "GetGoals");
    if(!GetGoals_m)
    {
      log.Warn << LOGID << "Could not find function GetGoal in the shared object." << endl;
      return;
    }

    GetGoalMemoryRequirement_t* ggmr = (GetGoalMemoryRequirement_t*)dlsym(handle, "GetGoalsMemoryRequirement");
    if(!ggmr)
    {
      log.Warn << LOGID << "Could not find function GetGoalMemoryRequirement in the shared object." << endl;
      return;
    }

    size_t mem = ggmr();

    GetGoals_buff = new bool[mem];

    is_goal_propnet_compiled = true;
    GetGoals = core::template_utils::BindToObject(&StateMachine::GetGoals_goal_m, this);

    log.Info << "Goal propnet successfully compiled." << endl;
  }
}

const size_t* StateMachine::Simulate(const AState& s)
{
  AState temp = s.Clone();

  temp.UpdateNodes(initial_pn_base, initial_pn_top, initial_pn_base_mask, initial_pn_base_move, initial_pn_base_nodes, initial_pn_legals, goals);

  bool is_terminal = initial_pn.GetTerminalNode()->holding_value;

  AMove m("");

  while(!is_terminal)
  {
    MoveList<AMove> ml = MoveList<AMove>(initial_pn_legals, role_size);

    //PrintMoveList(cout, ml);

    size_t rnd = rand() % ml.size();

    MoveList<AMove>::iterator it = ml.begin();
    for(size_t i = 0;i < rnd;i++)
      it++;

    //for(size_t i = 0;i < role_size;i++)
    //{
      //m->moves[i] = *initial_pn_legals[i].begin();
    //}

    m = *it;

    //PrintMove(cout, m);

    //temp = GetNextState(temp, m);
    m.UpdateNodes(initial_pn_base, initial_pn_top, initial_pn_base_move, initial_pn_input_nodes, NULL, NULL);

    temp = initial_pn_top.Clone();

    //PrintState(cout, temp);

    temp.UpdateNodes(initial_pn_base, initial_pn_top, initial_pn_base_mask, initial_pn_base_move, initial_pn_base_nodes, initial_pn_legals, goals);

    is_terminal = initial_pn.GetTerminalNode()->holding_value;//IsTerminal(temp);
  }

  //PrintState(cout, temp);

  return GetGoals(temp);

   //for(size_t i = 0;i < role_size;i++)
    //cout << goals[i] << endl;

   //return goals;
}

////////////////////////////////////////////////////////////////////////////////
/// Functions for initial propnet
////////////////////////////////////////////////////////////////////////////////

bool StateMachine::IsTerminal_initial_dfp(const AState& s)
{
  s.UpdateNodes(initial_pn_base, initial_pn_top, initial_pn_base_mask, initial_pn_base_move, initial_pn_base_nodes, initial_pn_legals, goals);
  //full_pn_base = s.Clone();
  return initial_pn.GetTerminalNode()->holding_value;
}

MoveList<AMove> StateMachine::GetLegalMoves_l_initial_dfp(const AState& s)
{
  s.UpdateNodes(initial_pn_base, initial_pn_top, initial_pn_base_mask, initial_pn_base_move, initial_pn_base_nodes, initial_pn_legals, goals);
  //full_pn_base = s.Clone();

  //size_t start = util::Timer::microtimer();
  return MoveList<AMove>(initial_pn_legals, role_size);
  //size_t end = util::Timer::microtimer();

  //cout << end - start << endl;

  //exit(1);
}

AState StateMachine::GetNextState_initial_dfp(const AState& s, const AMove& m)
{
  s.UpdateNodes(initial_pn_base, initial_pn_top, initial_pn_base_mask, initial_pn_base_move, initial_pn_base_nodes, initial_pn_legals, goals);
  m.UpdateNodes(initial_pn_base, initial_pn_top, initial_pn_base_move, initial_pn_input_nodes, NULL, NULL);
  //full_pn_base_move = m.Clone();
  //full_pn_base = s.Clone();
  return initial_pn_top.Clone();
}

const size_t* StateMachine::GetGoal_initial_dfp(const AState& s)
{
  s.UpdateNodes(initial_pn_base, initial_pn_top, initial_pn_base_mask, initial_pn_base_move, initial_pn_base_nodes, initial_pn_legals, goals);
  return goals;
}

////////////////////////////////////////////////////////////////////////////////
/// Functions for Goal Net
////////////////////////////////////////////////////////////////////////////////

const size_t* StateMachine::GetGoal_goal_dfp(const AState& s)
{
  s.UpdateNodes(goal_pn_base, goal_pn_top, goal_pn_base_mask, initial_pn_base_move, goal_pn_base_nodes, NULL, goals);
  return goals;
}

const size_t* StateMachine::GetGoals_goal_m(const AState& s)
{
  GetGoals_m(s, goals, GetGoals_buff);
  return goals;
}
