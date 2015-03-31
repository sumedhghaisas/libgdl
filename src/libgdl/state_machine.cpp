#include "state_machine.hpp"

#include <stdlib.h>
#include <time.h>
#include <stack>

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

size_t StateMachine::stack_time = 0;

sitmo::prng_engine StateMachine::eng;

StateMachine::StateMachine(int argc, char* argv[])
  : log(GLOBAL_LOG)
{
  bool isDot = false;
  string dot_file = "";

  bool separatePropNetForGoals = false;
  bool compile_goal_net = false;
  bool separate_pn_for_roles = false;
  bool crystallize = false;

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
  ("separate-propnet-for-roles", "Use separate propnets for different roles.")
  ("crystallize", "Crystallize uncompiled propnets.")
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

  if(vm.count("separate-propnet-for-roles"))
  {
    separate_pn_for_roles = true;
  }

  if(vm.count("crystallize"))
  {
    crystallize = true;
  }

  gdlparser::KIF kif(true, 1, log);
  for(auto it : source_files)
    kif.AddFile(it);
  kif.Parse();

  log.Info << "Source KIF parsed." << endl;

  if(!isDot)
  {
    //! Initialize propnet with the file given
    initial_pn.Initialize(kif);
  }
  else
    initial_pn.InitializeWithDOT(kif, dot_file);

  log.Info << "Initial propnet initialized." << endl;

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

  log.Info << "Initial propnet is configured for a run." << endl;

  MetaGame(meta_simulation_time);

  if(separatePropNetForGoals)
  {
    SeparateGoalNet(compile_goal_net);
  }

  if(isAlternatingMoves && role_size > 1 && separate_pn_for_roles)
  {
    SeparateRolePropNets();

    if(!is_goal_propnet_used)
      SeparateGoalNet(true);
  }

  if(crystallize)
  {
    log.Info << "Crystallizing propnets." << endl;

    for(size_t i = 0;i < role_size;i++)
      initial_pn_legals[i].clear();

    initial_pn_top.Clear();
    initial_pn_base = InitState().Clone();

    for(size_t i = 0;i < role_size;i++)
      initial_pn_base_move->moves[i] = 0;

    initial_pn.Crystallize(data_init, initial_pn_top, initial_pn_legals, goals);

    if(is_propnet_role_separated)
    {
      role_data = new signed short*[role_size];

      for(size_t i = 0;i < role_size;i++)
      {
        role_data[i] = new signed short[initial_pn.data_init_size];

        for(size_t j = 0;j < initial_pn.data_init_size;j++)
          role_data[i][j] = data_init[j];
      }

      for(size_t i = 0;i < role_size;i++)
      {
        role_pn_top[i] = initial_pn_top.Clone();
        role_pn_base[i] = initial_pn_base.Clone();

        for(size_t j = 0;j < role_size;j++)
        {
          role_pn_legals[i][j].clear();
          for(auto it : initial_pn_legals[j])
            role_pn_legals[i][j].insert(it);
        }

        role_pn_base_move[i] = initial_pn_base_move.Clone();
      }
    }
  }
}

void StateMachine::SeparateRolePropNets()
{
  log.Info << "Creating separate propnets for each role." << endl;

  is_propnet_role_separated = true;

  role_propnets = new PropNet*[role_size];
  role_pn_base = new AState[role_size];
  role_pn_top = new AState[role_size];
  role_pn_base_move = new AMove[role_size];
  role_pn_legals = new std::set<size_t>*[role_size];

  for(size_t i = 0;i < role_size;i++)
  {
    role_propnets[i] = new PropNet(initial_pn);

    role_pn_base[i] = InitState().Clone();
    role_pn_top[i] = AState("");

    role_pn_base_move[i] = AMove("");
    for(size_t j = 0;j < role_size;j++)
      role_pn_base_move[i]->moves[j] = 0;

    role_pn_legals[i] = new std::set<size_t>[role_size];

    role_propnets[i]->PrimaryRun(role_pn_top[i], role_pn_legals[i], goals);

    //MoveVector<AMove> ml = MoveVector<AMove>(role_pn_legals[i], role_size);

    //PrintMoveVector(cout, ml);
  }
}

const size_t* StateMachine::Simulate2(const AState& s)
{
  AState temp = s.Clone();

  size_t role_id = 0;

  for(size_t i = 0;i < role_size;i++)
  {
    if(alt_role_masks[i] & temp)
    {
      role_id = i;
      break;
    }
  }

  role_propnets[role_id]->UpdateNormal_base(temp, role_pn_base[role_id], role_pn_top[role_id], role_pn_legals[role_id], goals);

  bool is_terminal = role_propnets[role_id]->GetTerminalNode()->holding_value;

  AMove m("");

  while(!is_terminal)
  {
    //MoveVector<AMove> ml = MoveVector<AMove>(role_pn_legals[role_id], role_size);

    //PrintMoveVector(cout, ml);

    //size_t rnd = rand() % ml.size();

    //MoveList<AMove>::iterator it = ml.begin();
    //for(size_t i = 0;i < rnd;i++)
      //it++;

    //for(size_t i = 0;i < role_size;i++)
    //{
      //m->moves[i] = *initial_pn_legals[i].begin();
    //}

    //m = ml[rnd];

    for(size_t i = 0;i < role_size;i++)
    {
      size_t rnd = rand() % role_pn_legals[role_id][i].size();
      auto it = role_pn_legals[role_id][i].begin();
      for(size_t j = 0;j < rnd;j++)
        it++;
      //cout << i << endl;
      m->moves[i] = *it;
    }

    //PrintMove(cout, m);

    //temp = GetNextState(temp, m);
    role_propnets[role_id]->UpdateNormal_input(m, role_pn_base[role_id], role_pn_top[role_id], role_pn_base_move[role_id], NULL, goals);

    temp.Equate(role_pn_top[role_id]);

    //PrintState(cout, temp);

    //for(size_t i = 0;i < role_size;i++)
    //{
      //PrintState(cout, alt_role_masks[i]);
      //PrintState(cout, alt_role_masks[i] & temp);

      //if(alt_role_masks[i] & temp)
      //{
        //role_id = i;
        //break;
      //}
    //}
    role_id = ++role_id % role_size;

    //PrintState(cout, role_pn_base[role_id]);

    role_propnets[role_id]->UpdateNormal_base(temp, role_pn_base[role_id], role_pn_top[role_id], role_pn_legals[role_id], goals);

    is_terminal = role_propnets[role_id]->GetTerminalNode()->holding_value;//IsTerminal(temp);
  }

  //PrintState(cout, temp);

  return GetGoals(temp);
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
  isAlternatingMoves = true;

  size_t num_simulations = 0;

  size_t stop = Timer::microtimer() + simulation_time;

  AState temp("");
  for(size_t i = 0;i < core::RawAState::arr_size;i++)
    temp->s[i] = 255;

  alt_role_masks = new AState[role_size];
  for(size_t i = 0;i < role_size;i++)
    alt_role_masks[i] = temp.Clone();

  while(Timer::microtimer() < stop)
  {
    AState temp = InitState().Clone();

    initial_pn.UpdateNormal_base(temp, initial_pn_base, initial_pn_top, initial_pn_legals, goals);

    bool is_terminal = initial_pn.GetTerminalNode()->holding_value;

    AMove m("");

    while(!is_terminal)
    {
      if(isAlternatingMoves)
      {
        int current_control_r_id = -1;

        for(size_t i = 0;i < role_size;i++)
        {
          if(initial_pn_legals[i].size() > 1)
          {
            if(current_control_r_id > -1)
            {
              isAlternatingMoves = false;
              break;
            }
            else current_control_r_id = i;
          }
        }

        if(current_control_r_id != -1)
          alt_role_masks[current_control_r_id] = temp & std::move(alt_role_masks[current_control_r_id]);
      }

      MoveVector<AMove> ml = MoveVector<AMove>(initial_pn_legals, role_size);

      size_t rnd = rand() % ml.size();

      m = ml[rnd];

      initial_pn.UpdateNormal_input(m, initial_pn_base, initial_pn_top, initial_pn_base_move, NULL, goals);

      temp = initial_pn_top.Clone();

      initial_pn.UpdateNormal_base(temp, initial_pn_base, initial_pn_top, initial_pn_legals, goals);

      is_terminal = initial_pn.GetTerminalNode()->holding_value;
    }

    if(isZeroSumGame)
      CheckZeroSumGame();

    num_simulations++;
  }

  log.Info << "Performed " << num_simulations << " simulations in meta game simulations." << endl;

  if(isZeroSumGame)
  {
    log.Info << "Game is Zero-Sum game." << endl;
  }

  if(isAlternatingMoves)
  {
    log.Info << "Game with alternating moves." << endl;
  }
  else
  {
    delete[] alt_role_masks;
    alt_role_masks = NULL;
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
  initial_pn.PrimaryRun(initial_pn_top, initial_pn_legals, goals);

  IsTerminal = core::template_utils::BindToObject(&StateMachine::IsTerminal_initial_dfp, this);

  GetLegalMoves_l = core::template_utils::BindToObject(&StateMachine::GetLegalMoves_l_initial_dfp, this);

  GetNextState = core::template_utils::BindToObject(&StateMachine::GetNextState_initial_dfp, this);

  GetGoals = core::template_utils::BindToObject(&StateMachine::GetGoal_initial_dfp, this);
}

void StateMachine::SeparateGoalNet(bool compile_goal_propnet)
{
  log.Info << "Creating separate propnet for goals." << endl;

  is_goal_propnet_used = true;

  //! Initialize goal_net to number of roles
  goal_pn.InitializeToRoles(role_size);

  //! Split the goal net from full net
  initial_pn.SplitGoalNet(goal_pn);

  //! Set base for goal pn
  goal_pn_base = init.Clone();

  //! Set top for goal pn
  goal_pn_top = AState("");

  //! Initialize goal pn and get base mas
  goal_pn.PrimaryRun(goal_pn_top, NULL, goals);

  goal_pn.Crystallize(goal_net_data, goal_pn_top, NULL, goals);

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

  initial_pn.UpdateNormal_base(temp, initial_pn_base, initial_pn_top, initial_pn_legals, goals);

  bool is_terminal = initial_pn.GetTerminalNode()->holding_value;

  AMove m("");

  //size_t t;

  while(!is_terminal)
  {
    //MoveVector<AMove> ml = MoveVector<AMove>(initial_pn_legals, role_size);

    //PrintMoveVector(std::cout, ml);

    //size_t rnd = rand() % ml.size();

    //MoveList<AMove>::iterator it = ml.begin();
    //for(size_t i = 0;i < rnd;i++)
      //it++;

    //for(size_t i = 0;i < role_size;i++)
    //{
      //m->moves[i] = *initial_pn_legals[i].begin();
    //}

    //m = ml[rnd];
    //for(size_t i = 0;i < role_size;i++)
      //m->moves[i] = *initial_pn_legals[i].begin();
    for(size_t i = 0;i < role_size;i++)
    {
      size_t rnd = rand() % initial_pn_legals[i].size();
      auto it = initial_pn_legals[i].begin();
      for(size_t j = 0;j < rnd;j++)
        it++;
      //cout << i << endl;
      m->moves[i] = *it;
    }

    //PrintMove(std::cout, m);

    //temp = GetNextState(temp, m);
    initial_pn.UpdateNormal_input(m, initial_pn_base, initial_pn_top, initial_pn_base_move, NULL, goals);

    temp.Equate(initial_pn_top);

    //PrintState(std::cout, temp);

    //std::cin >> t;

    initial_pn.UpdateNormal_base(temp, initial_pn_base, initial_pn_top, initial_pn_legals, goals);

    is_terminal = initial_pn.GetTerminalNode()->holding_value;//IsTerminal(temp);
  }

  //PrintState(cout, temp);

  return goals;//GetGoals(temp);

   //for(size_t i = 0;i < role_size;i++)
    //cout << goals[i] << endl;

   //return goals;
}

////////////////////////////////////////////////////////////////////////////////
/// Functions for initial propnet
////////////////////////////////////////////////////////////////////////////////

bool StateMachine::IsTerminal_initial_dfp(const AState& s)
{
  initial_pn.UpdateNormal_base(s, initial_pn_base, initial_pn_top, initial_pn_legals, goals);
  //full_pn_base = s.Clone();
  return initial_pn.GetTerminalNode()->holding_value;
}

MoveList<AMove> StateMachine::GetLegalMoves_l_initial_dfp(const AState& s)
{
  initial_pn.UpdateNormal_base(s, initial_pn_base, initial_pn_top, initial_pn_legals, goals);
  //full_pn_base = s.Clone();

  //size_t start = util::Timer::microtimer();
  return MoveList<AMove>(initial_pn_legals, role_size);
  //size_t end = util::Timer::microtimer();

  //cout << end - start << endl;

  //exit(1);
}

AState StateMachine::GetNextState_initial_dfp(const AState& s, const AMove& m)
{
  initial_pn.UpdateNormal_base(s, initial_pn_base, initial_pn_top, initial_pn_legals, goals);
  initial_pn.UpdateNormal_input(m, initial_pn_base, initial_pn_top, initial_pn_base_move, NULL, goals);
  //full_pn_base_move = m.Clone();
  //full_pn_base = s.Clone();
  return initial_pn_top.Clone();
}

const size_t* StateMachine::GetGoal_initial_dfp(const AState& s)
{
  initial_pn.UpdateNormal_base(s, initial_pn_base, initial_pn_top, initial_pn_legals, goals);
  return goals;
}

////////////////////////////////////////////////////////////////////////////////
/// Functions for Goal Net
////////////////////////////////////////////////////////////////////////////////

const size_t* StateMachine::GetGoal_goal_dfp(const AState& s)
{
  goal_pn.CrystalUpdate_base(s, goal_pn_base, goal_pn_top, NULL, goals, goal_net_data, n_stack, v_stack);
  return goals;
}

const size_t* StateMachine::GetGoals_goal_m(const AState& s)
{
  GetGoals_m(s, goals, GetGoals_buff);
  return goals;
}
