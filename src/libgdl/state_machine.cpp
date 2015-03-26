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

  if(separatePropNetForGoals)
  {
    SeparateGoalNet(compile_goal_net);
  }

  if(isAlternatingMoves && role_size > 1 && separate_pn_for_roles)
  {
    SeparateRolePropNets();
  }

  if(crystallize)
  {
    //PrintState(cout, InitState());

    //PrintState(cout, initial_pn_top);

    for(size_t i = 0;i < role_size;i++)
      initial_pn_legals[i].clear();

    initial_pn_top.Clear();
    initial_pn_base = InitState().Clone();

    for(size_t i = 0;i < role_size;i++)
      initial_pn_base_move->moves[i] = 0;

    id_map = initial_pn.Crystallize(initial_pn_top, initial_pn_legals, goals);

    terminal_crystal_id = id_map.find(initial_pn.GetTerminalNode())->second;

    //MoveList<AMove> ml = MoveList<AMove>(initial_pn_legals, role_size);

    //PrintMoveList(cout, ml);

   // ofstream out("out.txt");

    //for(auto it : id_map)
    //{
      //out << it.first->UName() << " " << it.second << endl;
    //}
    //out.close();

    //MoveList<AMove> ml(initial_pn_legals, role_size);
    //AMove m = *++ml.begin();

    //PrintMoveList(cout, ml);

    //PrintState(cout, initial_pn_top);

    //PrintMove(cout, m);

    //UpdateCrystal_move(m, initial_pn_base_move, initial_pn_top, initial_pn_legals, goals);

    //PrintState(cout, initial_pn_top);

    //AState temp = initial_pn_top.Clone();

    //PrintState(cout, temp);

    //initial_pn.PrintPropnet("test.dot");

    //UpdateCrystal_base(temp, initial_pn_base_mask, initial_pn_top, initial_pn_base, initial_pn_legals, goals);

    //ml = MoveList<AMove>(initial_pn_legals, role_size);

    //PrintMoveList(cout, ml);

    //m = *ml.begin();

    //UpdateCrystal_move(m, initial_pn_base_move, initial_pn_top, initial_pn_legals, goals);

    //PrintState(cout, initial_pn_top);
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

void StateMachine::SeparateRolePropNets()
{
  log.Info << "Creating separate propnets for each role." << endl;

  is_propnet_role_separated = true;

  role_propnets = new PropNet*[role_size];
  role_pn_base_mask = new AState[role_size];
  role_pn_base = new AState[role_size];
  role_pn_top = new AState[role_size];
  role_pn_base_move = new AMove[role_size];
  role_pn_legals = new std::set<size_t>*[role_size];
  role_pn_base_nodes = new propnet::node_types::Node**[role_size];
  role_pn_input_nodes = new propnet::node_types::Node***[role_size];

  for(size_t i = 0;i < role_size;i++)
  {
    role_propnets[i] = new PropNet(initial_pn);

    role_pn_base_mask[i] = AState("");
    role_pn_base[i] = InitState().Clone();
    role_pn_top[i] = AState("");

    role_pn_base_move[i] = AMove("");
    for(size_t j = 0;j < role_size;j++)
      role_pn_base_move[i]->moves[j] = 0;

    role_pn_legals[i] = new std::set<size_t>[role_size];

    role_propnets[i]->InitializeRun(role_pn_top[i], role_pn_base_mask[i], role_pn_legals[i], goals);

    //MoveVector<AMove> ml = MoveVector<AMove>(role_pn_legals[i], role_size);

    //PrintMoveVector(cout, ml);

    role_pn_base_nodes[i] = new node_types::Node*[base_size];
    for(size_t j = 0;j < base_size;j++)
      role_pn_base_nodes[i][j] = NULL;
    for(auto it : role_propnets[i]->BaseNodes())
    {
      role_pn_base_nodes[i][it.first] = it.second;
    }

    role_pn_input_nodes[i] = new node_types::Node**[role_size];
    size_t index = 0;
    for(auto it : role_propnets[i]->InputNodes())
    {
      role_pn_input_nodes[i][index] = new node_types::Node*[it.size()];
      for(auto it2 : it)
      {
        role_pn_input_nodes[i][index][it2.first] = it2.second;
      }
      index++;
    }
  }

  AState state = InitState();

  AState temp = state.Clone();
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

  temp.UpdateNodes(role_pn_base[role_id], role_pn_top[role_id], role_pn_base_mask[role_id], role_pn_base_move[role_id], role_pn_base_nodes[role_id], role_pn_legals[role_id], goals);

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
    m.UpdateNodes(role_pn_base[role_id], role_pn_top[role_id], role_pn_base_move[role_id], role_pn_input_nodes[role_id], NULL, NULL);

    temp = role_pn_top[role_id].Clone();

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

    temp.UpdateNodes(role_pn_base[role_id], role_pn_top[role_id], role_pn_base_mask[role_id], role_pn_base_move[role_id], role_pn_base_nodes[role_id], role_pn_legals[role_id], goals);

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

    temp.UpdateNodes(initial_pn_base, initial_pn_top, initial_pn_base_mask, initial_pn_base_move, initial_pn_base_nodes, initial_pn_legals, goals);

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
            if(current_control_r_id > 0)
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

      m.UpdateNodes(initial_pn_base, initial_pn_top, initial_pn_base_move, initial_pn_input_nodes, NULL, NULL);

      temp = initial_pn_top.Clone();

      temp.UpdateNodes(initial_pn_base, initial_pn_top, initial_pn_base_mask, initial_pn_base_move, initial_pn_base_nodes, initial_pn_legals, goals);

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
    //MoveVector<AMove> ml = MoveVector<AMove>(initial_pn_legals, role_size);

    //PrintMoveList(cout, ml);

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

const size_t* StateMachine::Simulate3(const AState& s)
{
  AState temp = s.Clone();

  //PrintState(cout, temp);

  //MoveList<AMove> ml = MoveList<AMove>(initial_pn_legals, role_size);

  //PrintMoveList(cout, ml);

  //PrintState(cout, initial_pn_top);

  UpdateCrystal_base(temp, initial_pn_base_mask, initial_pn_top, initial_pn_base, initial_pn_legals, goals);

  //PrintState(cout, initial_pn_top);

  //temp.UpdateNodes(initial_pn_base, initial_pn_top, initial_pn_base_mask, initial_pn_base_move, initial_pn_base_nodes, initial_pn_legals, goals);

  bool is_terminal = initial_pn.data_init[terminal_crystal_id] & 0x4000;//initial_pn.GetTerminalNode()->holding_value;

  AMove m("");

  while(!is_terminal)
  {
    //MoveList<AMove> ml = MoveList<AMove>(initial_pn_legals, role_size);

    //PrintMoveList(cout, ml);

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

    //PrintMove(cout, m);

    //temp = GetNextState(temp, m);
    //m.UpdateNodes(initial_pn_base, initial_pn_top, initial_pn_base_move, initial_pn_input_nodes, NULL, NULL);
    UpdateCrystal_move(m, initial_pn_base_move, initial_pn_top, initial_pn_legals, goals);

    //temp = initial_pn_top.Clone();
    temp.Equate(initial_pn_top);

    //PrintState(cout, temp);

    //temp.UpdateNodes(initial_pn_base, initial_pn_top, initial_pn_base_mask, initial_pn_base_move, initial_pn_base_nodes, initial_pn_legals, goals);
    UpdateCrystal_base(temp, initial_pn_base_mask, initial_pn_top, initial_pn_base, initial_pn_legals, goals);

    is_terminal = initial_pn.data_init[terminal_crystal_id] & 0x4000;//initial_pn.GetTerminalNode()->holding_value;//IsTerminal(temp);
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

void StateMachine::UpdateCrystal_base(const AState& state, const AState& mask, AState& top, AState& base, set<size_t>* m_set, size_t* goals)
{
  stack<unsigned short> n_stack;
  stack<signed short> v_stack;

  for(size_t i = 0;i < state.get()->arr_size;i++)
  {
    char x_or = state.get()->s[i] ^ base.get()->s[i];
    x_or = x_or & mask.get()->s[i];
    if(x_or)
    {
      char s_val = state.get()->s[i];
      if(x_or & 1)
      {
        n_stack.push(id_map.find(initial_pn_base_nodes[8*i])->second);
        if((bool)(s_val & 1))
        {
          v_stack.push(0x0001);
        }
        else v_stack.push(0xffff);
      }

      //x_or = x_or >> 1;
      //s_val = s_val >> 1;
      if(x_or & 2)
      {
        n_stack.push(id_map.find(initial_pn_base_nodes[8*i + 1])->second);
        if((bool)(s_val & 2))
          v_stack.push(0x0001);
        else v_stack.push(0xffff);
      }

      //x_or = x_or >> 1;
      //s_val = s_val >> 1;
      if(x_or & 4)
      {
        n_stack.push(id_map.find(initial_pn_base_nodes[8*i + 2])->second);
        if((bool)(s_val & 4))
          v_stack.push(0x0001);
        else v_stack.push(0xffff);
      }

      //x_or = x_or >> 1;
      //s_val = s_val >> 1;
      if(x_or & 8)
      {
        n_stack.push(id_map.find(initial_pn_base_nodes[8*i + 3])->second);
        if((bool)(s_val & 8))
          v_stack.push(0x0001);
        else v_stack.push(0xffff);
      }

      //x_or = x_or >> 1;
      //s_val = s_val >> 1;
      if(x_or & 16)
      {
        n_stack.push(id_map.find(initial_pn_base_nodes[8*i + 4])->second);
        if((bool)(s_val & 16))
          v_stack.push(0x0001);
        else v_stack.push(0xffff);
      }

      //x_or = x_or >> 1;
      //s_val = s_val >> 1;
      if(x_or & 32)
      {
        n_stack.push(id_map.find(initial_pn_base_nodes[8*i + 5])->second);
        if((bool)(s_val & 32))
          v_stack.push(0x0001);
        else v_stack.push(0xffff);
      }

      //x_or = x_or >> 1;
      //s_val = s_val >> 1;
      if(x_or & 64)
      {
        n_stack.push(id_map.find(initial_pn_base_nodes[8*i + 6])->second);
        if((bool)(s_val & 64))
          v_stack.push(0x0001);
        else v_stack.push(0xffff);
      }

      //x_or = x_or >> 1;
      //s_val = s_val >> 1;
      if(x_or & 128)
      {
        n_stack.push(id_map.find(initial_pn_base_nodes[8*i + 7])->second);
        if((bool)(s_val & 128))
          v_stack.push(0x0001);
        else v_stack.push(0xffff);
      }
    }
  }

  while(!n_stack.empty())
  {
    size_t n_id = n_stack.top();
    signed short val = v_stack.top();

    //cout << n_id << " " << std::hex << val << std::dec << endl;

    //if(n_id == 164)
    //{
      //cout << "lol" << endl;
    //}

    propnet::CrystalNode& cn = initial_pn.cry[n_id];
    signed short& n_val = initial_pn.data_init[n_id];

    n_stack.pop();
    v_stack.pop();

    if(cn.type)
    {
      propnet::node_types::Node* n = (propnet::node_types::Node*)*(size_t*)(initial_pn.out_degree + cn.offset);

      //cout << cn.offset << endl;

      //cout << std::hex << *(initial_pn.out_degree + cn.offset) << std::dec << endl;

      n->CrystalUpdate(val, top, m_set, goals);
    }
    else
    {
      signed short t_val = val + n_val;

      //cout << std::hex << n_val << endl;
      //cout << std::hex << t_val << std::dec << endl;

      if((t_val ^ n_val) & 0x4000)
      {
        signed short p_val = 0;
        if(t_val & 0x4000)
          p_val = 0x0001;
        else p_val = 0xffff;

        unsigned short* o_start = initial_pn.out_degree + cn.offset;

        //cout << (size_t)cn.out_size << " " << cn.offset << endl;

        for(size_t i = 0;i < cn.out_size;i++)
        {
          //cout << o_start[i] << endl;
          n_stack.push(o_start[i]);
          v_stack.push(p_val);
        }
      }
      n_val = t_val;
    }
  }

  base.Equate(state);
}

void StateMachine::UpdateCrystal_move(const AMove& move, AMove& base, AState& top, std::set<size_t>* m_set, size_t* goals)
{
  stack<size_t> n_stack;
  stack<signed short> v_stack;

  for(size_t i = 0;i < move.get()->n_roles;i++)
  {
    if(move->moves[i] != base->moves[i])
    {
      n_stack.push(id_map.find(initial_pn_input_nodes[i][base->moves[i]])->second);
      v_stack.push(0xffff);
      base->moves[i] = move->moves[i];
      n_stack.push(id_map.find(initial_pn_input_nodes[i][move->moves[i]])->second);
      v_stack.push(0x0001);
    }
  }

  while(!n_stack.empty())
  {
    size_t n_id = n_stack.top();
    signed short val = v_stack.top();

    propnet::CrystalNode& cn = initial_pn.cry[n_id];
    signed short& n_val = initial_pn.data_init[n_id];

    n_stack.pop();
    v_stack.pop();

    if(cn.type)
    {
      propnet::node_types::Node* n = (propnet::node_types::Node*)*(size_t*)(initial_pn.out_degree + cn.offset);

      n->CrystalUpdate(val, top, m_set, goals);
    }
    else
    {
      signed short t_val = val + n_val;

      if((t_val ^ n_val) & 0x4000)
      {
        signed short p_val = 0;
        if(t_val & 0x4000)
          p_val = 0x0001;
        else p_val = 0xffff;

        unsigned short* o_start = initial_pn.out_degree + cn.offset;

        for(size_t i = 0;i < cn.out_size;i++)
        {
          n_stack.push(o_start[i]);
          v_stack.push(p_val);
        }
      }
      n_val = t_val;
    }
  }
}
