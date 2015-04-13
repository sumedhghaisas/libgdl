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

StateMachine::StateMachine(int argc, char* argv[])
  : log(GLOBAL_LOG)
{
  bool isDot = false;
  string dot_file = "";

  bool separatePropNetForGoals = false;
  bool compile_goal_net = false;
  bool separate_pn_for_roles = false;

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

  cout << init << endl;

  if(separatePropNetForGoals)
  {
    SeparateGoalNet(compile_goal_net);
  }

  FinalizeInitialPropNet();

  log.Info << "Initial propnet is configured for a run." << endl;

  MetaGame(meta_simulation_time);

  if(isAlternatingMoves && role_size > 1 && separate_pn_for_roles)
  {
    SeparateRolePropNets();

    if(!is_goal_propnet_used)
      SeparateGoalNet(true);
  }

  cout << "testing" << endl;

  cout << "Comps: " << initial_pn.GetNumComponents() << endl;
  cout << "Data size: " << initial_pn.GetCrystalDataSize() << endl;
  cout << "Base comps: " << initial_pn.BaseNodes().size() << endl;
  cout << "And components: " << initial_pn.GetNumAndComponents() << endl;
  cout << "Or components: " << initial_pn.GetNumOrComponents() << endl;
}

StateMachine::~StateMachine()
{
//  if(role_propnets != NULL)
//  {
//    for(size_t i = 0;i < role_size;i++)
//    {
//      if(role_data != NULL)
//        delete[] role_data[i];
//      delete role_propnets[i];
//      delete[] role_pn_legals[i];
//      delete[] role_pn_m_legal_size[i];
//    }
//    delete[] role_propnets;
//    delete[] role_pn_base;
//    delete[] role_pn_top;
//    delete[] role_pn_base_move;
//    delete[] alt_role_masks;
//    delete[] role_pn_m_legal_size;
//  }
}

void StateMachine::SeparateRolePropNets()
{
  log.Info << "Creating separate propnets for each role." << endl;

  is_propnet_role_separated = true;

  role_propnet_payloads = new PropNet::PayLoadType*[role_size];
  for(size_t i = 0;i < role_size;i++)
  {
    role_propnet_payloads[i] = initial_pn.GetPayLoadInstance();
  }
}

const size_t* StateMachine::Simulate2(const AState& s)
{
//  AState temp = s.Clone();
//
//  size_t role_id = 0;
//
//  for(size_t i = 0;i < role_size;i++)
//  {
//    if(alt_role_masks[i] & temp)
//    {
//      role_id = i;
//      break;
//    }
//  }
//
//  role_propnets[role_id]->UpdateNormal_base(temp, role_pn_base[role_id], role_pn_top[role_id], role_pn_legals[role_id], goals);
//
//  bool is_terminal = role_propnets[role_id]->GetTerminalNode()->holding_value;
//
//  AMove m("");
//
//  while(!is_terminal)
//  {
//    //MoveVector<AMove> ml = MoveVector<AMove>(role_pn_legals[role_id], role_size);
//
//    //PrintMoveVector(cout, ml);
//
//    //size_t rnd = rand() % ml.size();
//
//    //MoveList<AMove>::iterator it = ml.begin();
//    //for(size_t i = 0;i < rnd;i++)
//      //it++;
//
//    //for(size_t i = 0;i < role_size;i++)
//    //{
//      //m->moves[i] = *initial_pn_legals[i].begin();
//    //}
//
//    //m = ml[rnd];
//
//    for(size_t i = 0;i < role_size;i++)
//    {
//      size_t rnd = rand() % role_pn_legals[role_id][i].size();
//      auto it = role_pn_legals[role_id][i].begin();
//      for(size_t j = 0;j < rnd;j++)
//        it++;
//      //cout << i << endl;
//      m->moves[i] = *it;
//    }
//
//    //PrintMove(cout, m);
//
//    //temp = GetNextState(temp, m);
//    role_propnets[role_id]->UpdateNormal_input(m, role_pn_base[role_id], role_pn_top[role_id], role_pn_base_move[role_id], NULL, goals);
//
//    temp.Equate(role_pn_top[role_id]);
//
//    //PrintState(cout, temp);
//
//    //for(size_t i = 0;i < role_size;i++)
//    //{
//      //PrintState(cout, alt_role_masks[i]);
//      //PrintState(cout, alt_role_masks[i] & temp);
//
//      //if(alt_role_masks[i] & temp)
//      //{
//        //role_id = i;
//        //break;
//      //}
//    //}
//    role_id = ++role_id % role_size;
//
//    //PrintState(cout, role_pn_base[role_id]);
//
//    role_propnets[role_id]->UpdateNormal_base(temp, role_pn_base[role_id], role_pn_top[role_id], role_pn_legals[role_id], goals);
//
//    is_terminal = role_propnets[role_id]->GetTerminalNode()->holding_value;//IsTerminal(temp);
//  }
//
//  //PrintState(cout, temp);
//
//  return GetGoals(temp);
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

    AMove m("");

    while(!initial_pn.CrystalUpdate_base(temp, *initial_pn_payload))
    {
      if(isAlternatingMoves)
      {
        int current_control_r_id = -1;

        for(size_t i = 0;i < role_size;i++)
        {
          if(initial_pn_payload->legal_size[i] > 1)
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

      initial_pn.GetRandomLegalMove(*initial_pn_payload, m);

      initial_pn.CrystalUpdate_input(m, *initial_pn_payload);

      temp.Equate(initial_pn_payload->top);
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
    sum += initial_pn_payload->goals[i];

  if(sum != 100)
    isZeroSumGame = false;
}

void StateMachine::FinalizeInitialPropNet()
{
  initial_pn.Finalize();
  initial_pn_payload = initial_pn.GetPayLoadInstance();

  //IsTerminal = core::template_utils::BindToObject(&StateMachine::IsTerminal_initial_dfp, this);

  GetLegalMoves_l = core::template_utils::BindToObject(&StateMachine::GetLegalMoves_l_initial_dfp, this);

  GetNextState = core::template_utils::BindToObject(&StateMachine::GetNextState_initial_dfp, this);
}

void StateMachine::SeparateGoalNet(bool compile_goal_propnet)
{
  log.Info << "Creating separate propnet for goals." << endl;

  is_goal_propnet_used = true;

  //! Initialize goal_net to number of roles
  goal_pn.InitializeToRoles(role_size);

  //! Split the goal net from full net
  initial_pn.SplitGoalNet(goal_pn);

  //! Initialize goal pn and get base mas
  goal_pn.Finalize();
  goal_pn_payload = goal_pn.GetPayLoadInstance();

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


////////////////////////////////////////////////////////////////////////////////
/// Functions for initial propnet
////////////////////////////////////////////////////////////////////////////////

MoveList<AMove> StateMachine::GetLegalMoves_l_initial_dfp(const AState& s)
{
  //initial_pn.CrystalUpdate_base(s, *initial_pn_payload);
  //full_pn_base = s.Clone();

  //size_t start = util::Timer::microtimer();
  //return MoveList<AMove>(initial_pn_legals, role_size);
  //size_t end = util::Timer::microtimer();

  //cout << end - start << endl;

  //exit(1);
}

AState StateMachine::GetNextState_initial_dfp(const AState& s, const AMove& m)
{
  initial_pn.CrystalUpdate_base(s, *initial_pn_payload);
  initial_pn.CrystalUpdate_input(m, *initial_pn_payload);
  //full_pn_base_move = m.Clone();
  //full_pn_base = s.Clone();
  return initial_pn_payload->top.Clone();
}

////////////////////////////////////////////////////////////////////////////////
/// Functions for Goal Net
////////////////////////////////////////////////////////////////////////////////

const size_t* StateMachine::GetGoal_goal_dfp(const AState& s)
{
  goal_pn.CrystalUpdate_base(s, *goal_pn_payload);
  cout << "testing" << endl;
  return initial_pn_payload->goals;
}

const size_t* StateMachine::GetGoals_goal_m(const AState& s)
{
  GetGoals_m(s, initial_pn_payload->goals, GetGoals_buff);
  return initial_pn_payload->goals;
}
