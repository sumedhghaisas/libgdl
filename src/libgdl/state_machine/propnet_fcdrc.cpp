#include "propnet_fcdrc.hpp"

#include <boost/program_options.hpp>
#include <dlfcn.h>

#include <libgdl/gdlparser/kif.hpp>

using namespace std;
using namespace boost::program_options;
using namespace libgdl;
using namespace libgdl::state_machine;

PropnetFCDRC::PropnetFCDRC(int argc, char* argv[])
  : log(GLOBAL_LOG)
{
  bool isDot = false;
  string dot_file = "";

  bool separatePropNetForGoals = false;
  bool separate_pn_for_roles = false;

  size_t meta_simulation_time = 1000000;

  // Declare the supported options.
  options_description desc("Allowed options");
  desc.add_options()
  ("source-files,c", value<vector<string> >()->multitoken(),
                     "source files")
  ("dot-file,d", value<string>(), "DOT PropNet file")
  ("separate-goal-net", "Create separate propnet for goals.")
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
    log.Fatal << "ERROR: at least one source file has to be mentioned." << endl;
    log.Fatal << "USAGE : PropnetFCDRC -c SOURCE_FILES" << endl;
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
  MoveType::RawType::n_roles = role_size;

  //! Initialize Init State
  init = StateType("");
  initial_pn.InitState(init);

  if(separatePropNetForGoals)
  {
    SeparateGoalNet(true);
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

  without_terminal_pn = new ReasonerType(initial_pn);

  ReasonerType terminal_pn;
  without_terminal_pn->SplitTerminalNet(terminal_pn);

  without_terminal_pn->Finalize();
  without_terminal_payload = without_terminal_pn->GetPayLoadInstance2();

  std::string is_terminal_so = terminal_pn.CreateIsTerminalMachineCode();

  void* handle = dlopen(is_terminal_so.c_str(), RTLD_NOW);
  if (!handle)
  {
    log.Warn << LOGID << "The error is " << dlerror() << std::endl;
    return;
  }

  typedef bool (*IsTerminal_m_t)(const StateType&, bool*);

  IsTerminal_compiled = (IsTerminal_m_t)dlsym(handle, "IsTerminal");
  if(!IsTerminal_compiled)
  {
    log.Warn << LOGID << "The error is " << dlerror() << std::endl;
    return;
  }

  typedef size_t (*IsTerminal_m_memory_t)();

  IsTerminal_m_memory_t IsTerminal_memory = (IsTerminal_m_memory_t)dlsym(handle, "IsTerminalMemoryRequirement");
  if(!IsTerminal_memory)
  {
    log.Warn << LOGID << "The error is " << dlerror() << std::endl;
    return;
  }

  size_t mem = IsTerminal_memory();

  IsTerminal_buff = new bool[mem];
  simulate2_it = new MoveSet::const_iterator[role_size];
  for(size_t i = 0;i < 100;i++)
    simulate2_state_arr[i] = StateType("");
}

PropnetFCDRC::~PropnetFCDRC()
{
  delete[] alt_role_masks;
  delete initial_pn_payload;
  delete goal_pn_payload;
  delete[] role_propnet_payloads;
}

PropnetFCDRC::MoveType PropnetFCDRC::GetRandomLegalMove(const StateType& s)
{
  initial_pn.Update(s, *initial_pn_payload);
  AMove out("");
  initial_pn.GetRandomLegalMove(*initial_pn_payload, out);
  return out;
}

const size_t* PropnetFCDRC::Simulate5(const StateType& s)
{
  temp.Equate(s);

  while(!initial_pn.Update(temp, *initial_pn_payload))
  {
    initial_pn.GetRandomLegalMove(*initial_pn_payload, m);

    initial_pn.Update(m, *initial_pn_payload);

    temp.Equate(initial_pn_payload->GetState());
  }

  return GetGoals(temp);
}

const size_t* PropnetFCDRC::Simulate6(const StateType& s)
{
  temp.Equate(s);

  size_t role_id = 0;

  for(size_t i = 0;i < role_size;i++)
  {
    if(alt_role_masks[i] & temp)
    {
      role_id = i;
      break;
    }
  }

  while(!initial_pn.Update(temp, *role_propnet_payloads[role_id]))
  {
    initial_pn.GetRandomLegalMove(*role_propnet_payloads[role_id], m);

    initial_pn.Update(m, *role_propnet_payloads[role_id]);

    temp.Equate(role_propnet_payloads[role_id]->GetState());

    role_id = ++role_id % role_size;
  }

  return GetGoals(temp);
}

void PropnetFCDRC::SeparateRolePropNets()
{
  log.Info << "Creating separate propnets for each role." << std::endl;

  is_propnet_role_separated = true;

  role_propnet_payloads = new typename ReasonerType::PayLoadType*[role_size];
  for(size_t i = 0;i < role_size;i++)
  {
    role_propnet_payloads[i] = initial_pn.GetPayLoadInstance();
  }
}

void PropnetFCDRC::MetaGame(size_t simulation_time)
{
  if(role_size > 1)
  {
    MetaGame_multi_player(simulation_time);
  }
  else
  {
    log.Info << "Single player game." << std::endl;
  }
}

void PropnetFCDRC::MetaGame_multi_player(size_t simulation_time)
{
  isZeroSumGame = true;
  isAlternatingMoves = true;

  size_t num_simulations = 0;

  size_t stop = util::Timer::microtimer() + simulation_time;

  AState temp("");
  for(size_t i = 0;i < core::RawAState::arr_size;i++)
    temp->s[i] = 255;

  alt_role_masks = new AState[role_size];
  for(size_t i = 0;i < role_size;i++)
    alt_role_masks[i] = temp.Clone();

  while(util::Timer::microtimer() < stop)
  {
    StateType temp = InitState().Clone();

    MoveType m("");

    while(!initial_pn.Update(temp, *initial_pn_payload))
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

      initial_pn.Update(m, *initial_pn_payload);

      temp.Equate(initial_pn_payload->top);
    }

    if(isZeroSumGame)
      CheckZeroSumGame();

    num_simulations++;
  }

  log.Info << "Performed " << num_simulations << " simulations in meta game simulations." << std::endl;

  if(isZeroSumGame)
  {
    log.Info << "Game is Zero-Sum game." << std::endl;
  }

  if(isAlternatingMoves)
  {
    log.Info << "Game with alternating moves." << std::endl;
  }
  else
  {
    delete[] alt_role_masks;
    alt_role_masks = NULL;
  }
}

void PropnetFCDRC::CheckZeroSumGame()
{
  size_t sum = 0;
  for(size_t i = 0;i < role_size;i++)
    sum += initial_pn_payload->GetGoals()[i];

  if(sum != 100)
    isZeroSumGame = false;
}

void PropnetFCDRC::FinalizeInitialPropNet()
{
  initial_pn.Finalize();
  initial_pn_payload = initial_pn.GetPayLoadInstance();
  initial_pn_payload2 = initial_pn.GetPayLoadInstance2();

  GetLegalMoves_l = core::template_utils::BindToObject(&PropnetFCDRC::GetLegalMoves_l_initial_dfp, this);

  //GetNextState = core::template_utils::BindToObject(&PropnetFCDRC::GetNextState_initial_dfp, this);
}

void PropnetFCDRC::SeparateGoalNet(bool compile_goal_propnet)
{
  log.Info << "Creating separate propnet for goals." << std::endl;

  is_goal_propnet_used = true;

  //! Initialize goal_net to number of roles
  goal_pn.InitializeToRoles(role_size);

  //! Split the goal net from full net
  initial_pn.SplitGoalNet(goal_pn);

  //! Initialize goal pn and get base mas
  goal_pn.Finalize();
  goal_pn_payload = goal_pn.GetPayLoadInstance();

  GetGoals = core::template_utils::BindToObject(&PropnetFCDRC::GetGoal_goal_dfp, this);

  if(compile_goal_propnet)
  {
    std::string object = goal_pn.CreateGetGoalMachineCode();

    void* handle = dlopen(object.c_str(), RTLD_NOW);
    if (!handle)
    {
      log.Warn << LOGID << "The error is " << dlerror() << std::endl;
      return;
    }

    typedef size_t (GetGoalMemoryRequirement_t)();

    GetGoals_m = (GetGoals_m_t*)dlsym(handle, "GetGoals");
    if(!GetGoals_m)
    {
      log.Warn << LOGID << "Could not find function GetGoal in the shared object." << std::endl;
      return;
    }

    GetGoalMemoryRequirement_t* ggmr = (GetGoalMemoryRequirement_t*)dlsym(handle, "GetGoalsMemoryRequirement");
    if(!ggmr)
    {
      log.Warn << LOGID << "Could not find function GetGoalMemoryRequirement in the shared object." << std::endl;
      return;
    }

    size_t mem = ggmr();

    GetGoals_buff = new bool[mem];

    is_goal_propnet_compiled = true;
    GetGoals = core::template_utils::BindToObject(&PropnetFCDRC::GetGoals_goal_m, this);

    log.Info << "Goal propnet successfully compiled." << std::endl;
  }
}

const size_t* PropnetFCDRC::Simulate2(const StateType& s)
{
  temp.Equate(s);

  size_t state_index = 0;
  bool isLoop = true;
  bool isLoop2 = true;

  size_t t;

  //cout << "testing-2" << endl;

  StateType top = without_terminal_payload->top;
  MoveSet* m_set = without_terminal_payload->m_set;

  while(!without_terminal_payload->terminal)
  {
    //cout << "testing-1" << endl;

    without_terminal_pn->Update2(temp, *without_terminal_payload);

    //cout << "testing-4" << endl;

    for(size_t i = 0;i < role_size;i++)
    {
      //cout << *m_set[i].begin() << endl;
      simulate2_it[i] = m_set[i].begin();
//      if(m_set[i].begin() == m_set[i].end())
//      {
//        std::cout << s << std::endl;
//        std::cout << LOGID << ": lol" << endl;
//        size_t t;
//        std::cin >> t;
//      }
    }

    state_index = 0;
    isLoop2 = true;
    while(isLoop2)
    {
      //cout << "testing" << endl;

      without_terminal_pn->Update3(simulate2_it, *without_terminal_payload);

      //cout << "testing1" << endl;

      //std::cout << without_terminal_payload->top << std::endl;

      //std::cin >> t;

      if(IsTerminal_compiled(top, GetGoals_buff))
      {
        return GetGoals(top);
      }
      else simulate2_state_arr[state_index++].Equate(top);

      //cout << "testing2" << endl;

      size_t index = 0;
      while(true)
      {
        simulate2_it[index]++;
        if(simulate2_it[index] == m_set[index].end())
        {
          if(index == role_size - 1)
          {
            isLoop2 = false;
            break;
          }
          simulate2_it[index] = m_set[index].begin();
          index++;
        }
        else break;
      }
    }

    //cout << "testing3" << endl;

    size_t rnd = rand() % state_index;
    temp.Equate(simulate2_state_arr[rnd]);

    //std::cout << temp << std::endl;

    //std::cin >> t;
  }

  return GetGoals(temp);
}

PropnetFCDRC::StateType PropnetFCDRC::GetNextState(const StateType& s,
                                                   const MoveType& m)
{
  initial_pn.Update2(s, *initial_pn_payload2);
  initial_pn.Update2(m, *initial_pn_payload2);

  return initial_pn_payload2->top.Clone();
}

////////////////////////////////////////////////////////////////////////////////
/// Functions for initial propnet
////////////////////////////////////////////////////////////////////////////////

MoveList<PropnetFCDRC::MoveType>
  PropnetFCDRC::GetLegalMoves_l_initial_dfp(const StateType& s)
{
//  initial_pn.Update(s, *initial_pn_payload);
//
//  size_t* it = new size_t[role_size];
//  for(size_t i = 0;i < role_size;i++)
//  {
//    it[i] = 0;
//    while(initial_pn_payload->data[initial_pn.legal_memory_ids[i][it[i]])
//      it[0]++;
//  }
//
//  while(true)
//  {
//
//    std::list<size_t> moves;
//    for(size_t i = 0;i < n_roles;i++)
//    {
//      moves.push_back(*it[i]);
//    }
//    (*this)->push_back(AMove(moves));
//
//    it[0]++;
//    size_t index = 1;
//    if(it[0] == result[0].end())
//    {
//      it[0] = result[0].begin();
//
//      while(true)
//      {
//        if(index == n_roles ||
//          (it[index] == (--result[index].end()) && index == n_roles - 1))
//        {
//          delete[] it;
//          return;
//        }
//        else if(it[index] == (--result[index].end()))
//        {
//          it[index] = result[index].begin();
//          index++;
//        }
//        else
//        {
//          it[index]++;
//          break;
//        }
//      }
//    }
//  }
}

PropnetFCDRC::StateType
  PropnetFCDRC::GetNextState_initial_dfp(const StateType& s,
                                         const MoveType& m)
{
  initial_pn.Update(s, *initial_pn_payload);
  initial_pn.Update(m, *initial_pn_payload);
  //full_pn_base_move = m.Clone();
  //full_pn_base = s.Clone();
  return initial_pn_payload->top.Clone();
}

bool PropnetFCDRC::CompiledIsTerminal(const StateType& state) const
{
  return IsTerminal_compiled(state, IsTerminal_buff);
}

////////////////////////////////////////////////////////////////////////////////
/// Functions for Goal Net
////////////////////////////////////////////////////////////////////////////////

const size_t* PropnetFCDRC::GetGoal_goal_dfp(const StateType& s)
{
  goal_pn.Update(s, *goal_pn_payload);
  return initial_pn_payload->GetGoals();
}

const size_t* PropnetFCDRC::GetGoals_goal_m(const StateType& s)
{
  GetGoals_m(s, initial_pn_payload->goals, GetGoals_buff);
  return initial_pn_payload->GetGoals();
}
