#include "propnet.hpp"

#include "entry_manager.hpp"

#include <sstream>
#include <fstream>
#include <tuple>

using namespace std;
using namespace libgdl;
using namespace libgdl::core;
using namespace libgdl::gdlreasoner;
using namespace libgdl::gdlparser;
using namespace libgdl::propnet;
using namespace libgdl::propnet::node_types;

PropNet::PropNet(const std::string& filename, Log log)
  : terminal(NULL), c_r_id(0), c_and_id(0), c_not_id(0), c_or_id(0), c_view_id(0), log(log)
{
  KIF kif(true, 1, log);
  kif.AddFile(filename);
  kif.Parse();

  KIFFlattener kf(log);
  kf.Flatten(kif);

  CreatePropNet(kf);
}

PropNet::PropNet(KIFFlattener& kf, Log log)
  : terminal(NULL), c_r_id(0), c_and_id(0), c_not_id(0), c_or_id(0), c_view_id(0), log(log)
{
  CreatePropNet(kf);
}

PropNet::~PropNet()
{
  for(auto it : view_nodes)
  {
    delete it.second;
  }

  for(auto it : next_nodes)
  {
    delete it.second;
  }

  for(auto it : input_nodes)
  {
    for(auto it2 : it)
    {
      delete it2.second;
    }
  }

  for(auto it : legal_nodes)
  {
    for(auto it2 : it)
    {
      delete it2.second;
    }
  }

  for(auto it : goal_nodes)
  {
    for(auto it2 : it)
    {
      delete it2.second;
    }
  }

  delete terminal;

  for(auto it : base_nodes)
  {
    delete it.second;
  }

  for(auto it : and_nodes)
  {
    delete it;
  }

  for(auto it : or_nodes)
  {
    delete it;
  }

  for(auto it : not_nodes)
  {
    delete it;
  }
}

void PropNet::CreatePropNet(KIFFlattener& kf)
{
  const list<Fact>& facts = kf.Facts();
  const list<Clause>& clauses = kf.Clauses();

  SymbolTable sym = kf.GetSymbolTable();

  for(auto f : facts)
  {
    CreateNode(sym, f.arg);
  }

  for(auto c : clauses)
  {
    Node* head = CreateNode(sym, c.head);

    Node* a = new AndNode("And", c_and_id++);
    and_nodes.push_back(a);

    for(auto premiss : c.premisses)
    {
      Node* temp = CreateNode(sym, premiss);
      a->AddIn(temp);
      temp->AddOut(a);
    }

    head->AddIn(a);
    a->AddOut(head);
  }
}

Node* PropNet::CreateNode(SymbolTable sym, const Argument* arg)
{
  std::string s_arg = arg->DecodeToString(sym);
  Node* out = NULL;

  if(arg->value == SymbolTable::RoleID)
  {
    roles_ids[sym.GetCommandName(arg->args[0]->value)] = c_r_id++;
    imove.AddRole();
    input_nodes.emplace_back();
    goal_nodes.emplace_back();
    legal_nodes.emplace_back();
  }
  else if(arg->value == SymbolTable::BaseID || arg->value == SymbolTable::TrueID)
  {
    s_arg = arg->args[0]->DecodeToString(sym);
    size_t p_id = istate.AddProp(s_arg);

    auto it = base_nodes.find(p_id);
    if(it == base_nodes.end())
    {
      out = new BaseNode(s_arg, p_id);
      base_nodes[p_id] = out;
    }
    else out = it->second;
  }
  else if(arg->value == SymbolTable::InputID || arg->value == SymbolTable::DoesID )
  {
    size_t r_id = roles_ids[sym.GetCommandName(arg->args[0]->value)];
    s_arg = arg->args[1]->DecodeToString(sym);
    size_t in_id = imove.AddMove(s_arg, r_id);

    auto it = input_nodes[r_id].find(in_id);
    if(it == input_nodes[r_id].end())
    {
      out = new InputNode(s_arg, r_id, in_id);
      input_nodes[r_id][in_id] = out;
    }
    else out = it->second;
  }
  else if(arg->value == SymbolTable::NextID)
  {
    s_arg = arg->args[0]->DecodeToString(sym);
    size_t p_id = istate.AddProp(s_arg);

    auto it = next_nodes.find(p_id);
    if(it == next_nodes.end())
    {
      out = new NextNode(s_arg, p_id);
      next_nodes[p_id] = out;
    }
    else out = it->second;
  }
  else if(arg->value == SymbolTable::LegalID)
  {
    size_t r_id = roles_ids[sym.GetCommandName(arg->args[0]->value)];
    s_arg = arg->args[1]->DecodeToString(sym);
    size_t in_id = imove.AddMove(s_arg, r_id);

    auto it = legal_nodes[r_id].find(in_id);
    if(it == legal_nodes[r_id].end())
    {
      out = new LegalNode(s_arg, r_id, in_id);
      legal_nodes[r_id][in_id] = out;
    }
    else out = it->second;
  }
  else if(arg->value == SymbolTable::GoalID)
  {
    size_t r_id = roles_ids[sym.GetCommandName(arg->args[0]->value)];
    s_arg = arg->args[1]->DecodeToString(sym);
    stringstream stream;
    stream << s_arg;
    size_t g_id;
    stream >> g_id;

    auto it = goal_nodes[r_id].find(g_id);
    if(it == goal_nodes[r_id].end())
    {
      out = new GoalNode(s_arg, r_id, g_id);
      goal_nodes[r_id][g_id] = out;
    }
    else out = it->second;
  }
  else if(arg->value == SymbolTable::TerminalID)
  {
    if(terminal == NULL)
    {
      terminal = new TerminalNode(s_arg);
    }

    out = terminal;
  }
  else if(arg->value == SymbolTable::OrID)
  {
    out = new OrNode(s_arg, c_or_id++);
    or_nodes.push_back(out);
    for(auto a : arg->args)
    {
      Node* temp = CreateNode(sym, a);
      temp->AddOut(out);
      out->AddIn(temp);
    }
  }
  else if(arg->value == SymbolTable::NotID)
  {
    out = new NotNode(s_arg, c_not_id++);
    not_nodes.push_back(out);
    Node* temp = CreateNode(sym, arg->args[0]);
    temp->AddOut(out);
    out->AddIn(temp);
  }
  else if(arg->value == SymbolTable::InitID)
  {
    s_arg = arg->args[0]->DecodeToString(sym);
    size_t p_id = istate.AddProp(s_arg);

    init_props.push_back(p_id);
  }
  else
  {
    auto it = view_nodes.find(s_arg);
    if(it == view_nodes.end())
    {
      out = new ViewNode(s_arg, c_view_id++);
      view_nodes[s_arg] = out;
    }
    else out = it->second;
  }

  return out;
}

bool PropNet::PrintPropnet(const std::string& filename) const
{
  ofstream stream(filename);

  if(!stream.is_open())
  {
    log.Fatal << LOGID << "Unable to open file " << filename << std::endl;
    return false;
  }

  stream << "digraph dependency_graph {" << endl;


  for(auto it : base_nodes)
  {
    stream << it.second->UName() << " [label = \""
           << it.second->Name() << "\"];" << endl;
  }

  for(auto it : next_nodes)
  {
    stream << it.second->UName() << " [label = \""
           << it.second->Name() << "\"];" << endl;
  }

  for(auto it : view_nodes)
  {
    stream << it.second->UName() << " [label = \""
           << it.second->Name() << "\"];" << endl;
  }

  for(auto m : input_nodes)
  {
    for(auto it : m)
    {
      stream << it.second->UName() << " [label = \""
             << it.second->Name() << "\"];" << endl;
    }
  }

  for(auto m : legal_nodes)
  {
    for(auto it : m)
    {
      stream << it.second->UName() << " [label = \""
             << it.second->Name() << "\"];" << endl;
    }
  }

  for(auto m : goal_nodes)
  {
    for(auto it : m)
    {
      stream << it.second->UName() << " [label = \""
             << it.second->Name() << "\"];" << endl;
    }
  }

  for(auto a : and_nodes)
  {
    stream << a->UName() << " [label = \""
           << a->Name() << "\"];" << endl;
  }

  for(auto a : or_nodes)
  {
    stream << a->UName() << " [label = \""
           << a->Name() << "\"];" << endl;
  }

  for(auto a : not_nodes)
  {
    stream << a->UName() << " [label = \""
           << a->Name() << "\"];" << endl;
  }

  stream << terminal->UName() << " [label = \""
         << terminal->Name() << "\"];" << endl;

  for(auto it : view_nodes)
  {
    for(auto n : it.second->in_degree)
    {
      stream << n->UName() << " -> " << it.second->UName() << ";" << endl;
    }
  }

  for(auto it : next_nodes)
  {
    for(auto n : it.second->in_degree)
    {
      stream << n->UName() << " -> " << it.second->UName() << ";" << endl;
    }
  }

  for(auto a : and_nodes)
  {
    for(auto n : a->in_degree)
    {
      stream << n->UName() << " -> " << a->UName() << ";" << endl;
    }
  }

  for(auto a : or_nodes)
  {
    for(auto n : a->in_degree)
    {
      stream << n->UName() << " -> " << a->UName() << ";" << endl;
    }
  }

  for(auto a : not_nodes)
  {
    for(auto n : a->in_degree)
    {
      stream << n->UName() << " -> " << a->UName() << ";" << endl;
    }
  }

  for(auto m : legal_nodes)
  {
    for(auto it : m)
    {
      for(auto n : it.second->in_degree)
      {
        stream << n->UName() << " -> " << it.second->UName() << ";" << endl;
      }
    }
  }

  for(auto m : goal_nodes)
  {
    for(auto it : m)
    {
      for(auto n : it.second->in_degree)
      {
        stream << n->UName() << " -> " << it.second->UName() << ";" << endl;
      }
    }
  }

  for(auto n : terminal->in_degree)
  {
    stream << n->UName() << " -> " << terminal->UName() << ";" << endl;
  }

  stream << "}";
  stream.close();
  return true;
}

void PropNet::GenerateStateMachineCode(std::ostream& m_file)
{
  //! Add required includes
  m_file << "#include <iostream>" << endl;
  m_file << "#include <vector>" << endl;
  m_file << "#include <list>" << std::endl;
  m_file << "#include <stdlib.h>" << endl;
  m_file << "#include <time.h>" << endl << endl;

  m_file << "#include \"state.hpp\"" << endl;
  m_file << "#include \"move.hpp\"" << endl;
  m_file << "#include <libgdl/core/data_types/move_list.hpp>" << endl << endl;

  m_file << "using namespace std;" << endl;
  m_file << "using namespace libgdl;" << endl << endl;

////////////////////////////////////////////////////////////////////////////////
/// InitState Function Generation
////////////////////////////////////////////////////////////////////////////////
  {
    m_file << "extern \"C\" State InitState()" << std::endl;
    m_file << "{" << std::endl;
    m_file << "State init(new RawState());" << std::endl;

    for(auto i_prop : init_props)
    {
      m_file << "init->Set_" << i_prop << "(true);" << std::endl;
    }
    m_file << "return init;" << std::endl;
    m_file << "}" << std::endl << std::endl;
  }

////////////////////////////////////////////////////////////////////////////////
/// Series Functions generation
////////////////////////////////////////////////////////////////////////////////

  GenerateSeriesFunctions(m_file, 0);

  m_file << endl << endl;

////////////////////////////////////////////////////////////////////////////////
/// GetGoals function
////////////////////////////////////////////////////////////////////////////////

  EntryManager gg_em;

  tuple<bool, size_t> out;

  //! Generate entries for goal nodes
  gg_em.StartNewList();
  list<size_t> goal_to_get_l;
  list<list<tuple<size_t, size_t>>> goal_fun_m;

  for(auto role_goals : goal_nodes)
  {
    list<tuple<size_t, size_t>> temp;
    for(auto goal : role_goals)
    {
      out = goal.second->CodeGen(gg_em, 1);
      temp.emplace_back(goal.first, get<1>(out));
      goal_to_get_l.push_back(get<1>(out));
    }
    goal_fun_m.push_back(temp);
  }

  //! Initialize the entry manager to start
  gg_em.InitializeIterator();

  //! Generate code for GetGoals
  stringstream goals_ss1;
  stringstream goals_ss2;
  list<size_t> goals_ret = gg_em.CodeGen(goals_ss1, goals_ss2, goal_to_get_l);

  m_file << "extern \"C\" std::list<size_t> GetGoals(const State& s, bool* buff)" << endl;
  m_file << "{" << endl;

  m_file << goals_ss1.str() << endl;

  auto ret_it = goals_ret.begin();

  m_file << "std::list<size_t> out;" << endl;

  for(auto role : goal_fun_m)
  {
    auto it = role.begin();
    m_file << "if(buff[" << *ret_it << "]) out.push_back(" << get<0>(*it) << ");" << endl;
    ret_it++;
    it++;
    for(it = it;it != role.end();it++)
    {
      m_file << "else if(buff[" << *ret_it << "]) out.push_back(" << get<0>(*it) << ");" << endl;
      ret_it++;
    }
  }

  m_file << "return out;" << endl;

  m_file << "}" << endl;

  m_file << "extern \"C\" size_t GetGoalMemoryRequirement()" << endl;
  m_file << "{" << endl;
  m_file << "return " << gg_em.GetRequiredMemory() << ";" << endl;
  m_file << "}" << endl;

//////////////////////////////////////////////////////////////////////////////////
///// IsTerminal Function Generation
//////////////////////////////////////////////////////////////////////////////////
//  {
//    EntryManager is_terminal_em;
//    is_terminal_em.StartNewList();
//
//    //! Generate entries for IsTerminal function
//    tuple<bool, size_t> is_terminal_out = terminal->CodeGen(is_terminal_em, 2);
//
//    std::stringstream is_terminal_ss1;
//    std::stringstream is_terminal_ss2;
//
//    //! This list is used to receive memory locations of the entries required
//    list<size_t> is_terminal_to_get_l;
//    is_terminal_to_get_l.push_back(get<1>(is_terminal_out));
//
//    //! Initialize the entry manager to start
//    is_terminal_em.InitializeIterator();
//
//    //! Generate code from generated entries
//    list<size_t> is_terminal_ret = is_terminal_em.CodeGen(is_terminal_ss1, is_terminal_ss2, is_terminal_to_get_l);
//
////    m_file << "template<typename BType, typename SType>" << std::endl;
////    m_file << "static void IsTerminal(const SType& s, BType* buff, BType* ret)" << endl;
////    m_file << "{" << endl;
////
////    m_file << ss2.str() << endl;
////
////    m_file << "*ret = buff[" << *ret.begin() << "];" << endl;
////    m_file << "}" << endl;
//
//    //! Generate code for the function
//    m_file << "static void IsTerminal(const State& s, bool* buff, bool* ret)" << endl;
//    m_file << "{" << endl;
//
//    m_file << is_terminal_ss1.str() << endl;
//
//    is_terminal_to_get_l.push_back(get<1>(is_terminal_out));
//
//    m_file << "*ret = buff[" << *is_terminal_ret.begin() << "];" << endl;
//
//    m_file << "}" << endl;
//  }

//  //! Generate GetGoals function
//  EntryManager em2;
//  em2.StartNewList();
//  std::list<size_t> to_get_l;
//  to_get_l.clear();
//
//  list<list<tuple<size_t, size_t>>> goal_fun_m;
//
//  std::tuple<bool, size_t> out;
//
//  for(auto role_goals : goal_nodes)
//  {
//    list<tuple<size_t, size_t>> temp;
//    for(auto goal : role_goals)
//    {
//      out = goal.second->CodeGen(em2, 2);
//      temp.emplace_back(goal.first, get<1>(out));
//      to_get_l.push_back(get<1>(out));
//    }
//    goal_fun_m.push_back(temp);
//  }
//
//  std::stringstream getgoal_ss1;
//  std::stringstream getgoal_ss2;
//
//  em2.InitializeIterator();
//  std::list<size_t> ret = em2.CodeGen(getgoal_ss1, getgoal_ss2, to_get_l);
//
//  m_file << "static void GetGoals(const State& s, bool* buff, std::list<size_t>& out)" << endl;
//  m_file << "{" << endl;
//
//  m_file << getgoal_ss1.str() << std::endl;
//
//  m_file << "out.clear();" << endl;
//
//  auto ret_it = ret.begin();
//
//  for(auto role : goal_fun_m)
//  {
//    auto it = role.begin();
//    m_file << "if(buff[" << *ret_it << "]) out.push_back(" << get<0>(*it) << ");" << endl;
//    ret_it++;
//    it++;
//    for(it = it;it != role.end();it++)
//    {
//      m_file << "else if(buff[" << *ret_it << "]) out.push_back(" << get<0>(*it) << ");" << endl;
//      ret_it++;
//    }
//  }
//
//  m_file << "}" << endl << endl;
//
//  //! generate the simulation function
//  EntryManager em3;
//
//  to_get_l.clear();
//
//  list<list<tuple<size_t, size_t>>> sim_legal_fun_m;
//
//  //! Generate entries for legal nodes
//  em3.StartNewList();
//  for(auto role_legals : legal_nodes)
//  {
//    list<tuple<size_t, size_t>> temp;
//    for(auto legal : role_legals)
//    {
//      out = legal.second->CodeGen(em3, 2);
//      temp.emplace_back(legal.first, get<1>(out));
//      to_get_l.push_back(get<1>(out));
//    }
//    sim_legal_fun_m.push_back(temp);
//  }
//
//  //! Generate entries for next nodes and terminal
//  em3.StartNewList();
//  for(auto nn : next_nodes)
//  {
//    out = nn.second->CodeGen(em3, 2);
//  }
//  tuple<bool, size_t> terminal_entry_id = terminal->CodeGen(em3, 2);
//
//  //! Generate entries for goal nodes
//  em3.StartNewList();
//  std::list<size_t> sim_goal_to_get_l;
//  list<list<tuple<size_t, size_t>>> sim_goal_fun_m;
//
//  for(auto role_goals : goal_nodes)
//  {
//    list<tuple<size_t, size_t>> temp;
//    for(auto goal : role_goals)
//    {
//      out = goal.second->CodeGen(em3, 2);
//      temp.emplace_back(goal.first, get<1>(out));
//      sim_goal_to_get_l.push_back(get<1>(out));
//    }
//    sim_goal_fun_m.push_back(temp);
//  }
//
//  stringstream sim_legal_ss1;
//  stringstream sim_legal_ss2;
//
//  em3.InitializeIterator();
//
//  //! generate code to compute legal moves
//  std::list<size_t> legal_ret = em3.CodeGen(sim_legal_ss1, sim_legal_ss2, to_get_l);
//
//  std::stringstream sim_next_t_ss1;
//  std::stringstream sim_next_t_ss2;
//
//  to_get_l.clear();
//  to_get_l.push_back(std::get<1>(terminal_entry_id));
//
//  //! Generate code for next nodes and terminal
//  std::list<size_t> terminal_ret = em3.CodeGen(sim_next_t_ss1, sim_next_t_ss2, to_get_l);
//
//  stringstream sim_goals_ss1;
//  stringstream sim_goals_ss2;
//
//  //! Generate code for goal nodes
//  std::list<size_t> sim_goals_ret = em3.CodeGen(sim_goals_ss1, sim_goals_ss2, sim_goal_to_get_l);
//
//  //! Random Move taker
//  m_file << "static size_t RandomActionTaker(size_t role_id, size_t* actions, size_t count)" << endl;
//  m_file << "{" << endl;
//  m_file << "size_t ran = rand() % count;" << endl;
//  m_file << "return actions[ran];" << endl;
//  m_file << "}" << endl;
//
//  //! simulation function
//  m_file << "static void RunSimulation(const State& in_s, bool* buff, size_t* l_move_buff, const std::function<size_t(size_t,size_t*, size_t)>& action_taker, std::list<size_t>& out)" << endl;
//  m_file << "{" << endl;
//
//  m_file << "State s = in_s;" << std::endl;
//  m_file << "bool isTerminal = false;" << std::endl;
//  m_file << "while(!isTerminal) {" << endl << endl;
//
//  //! print the legal move generator code
//  m_file << sim_legal_ss1.str() << endl;
//
//  //! Create a move based on ActionTaker function
//  m_file << "Move move(new RawMove());" << endl;
//  m_file << "size_t index = 0;" << endl;
//
//  m_file << "size_t m_buffer_index = 0;" << endl;
//
//  ret_it = legal_ret.begin();
//
//  for(auto role : sim_legal_fun_m)
//  {
//    for(auto it : role)
//    {
//      m_file << "if(buff[" << *ret_it << "]) l_move_buff[m_buffer_index++] = " << get<0>(it) << ";" << endl;
//      ret_it++;
//    }
//    m_file << "move.SetMove(index++, action_taker(index, l_move_buff, m_buffer_index));" << endl;
//    m_file << "m_buffer_index = 0;" << endl;
//  }
//
//  //m_file << "std::cout << move << std::endl;" << endl;
//
//  m_file << "State s_out(new RawState());" << endl;
//
//  m_file << sim_next_t_ss1.str() << endl;
//
//  //m_file << "std::cout << s_out << std::endl;" << endl;
//  m_file << "s = s_out;" << std::endl;
//
//  m_file << "isTerminal = buff[" << *terminal_ret.begin() << "];" << endl;
//  //m_file << "std::cout << isTerminal << std::endl;" << endl;
//
//  m_file << "}//while loop" << endl;
//
//  m_file << sim_goals_ss1.str() << endl;
//
//  m_file << "out.clear();" << endl;
//
//  auto sim_ret_it = sim_goals_ret.begin();
//
//  for(auto role : sim_goal_fun_m)
//  {
//    auto it = role.begin();
//    m_file << "if(buff[" << *sim_ret_it << "]) out.push_back(" << get<0>(*it) << ");" << endl;
//    sim_ret_it++;
//    it++;
//    for(it = it;it != role.end();it++)
//    {
//      m_file << "else if(buff[" << *sim_ret_it << "]) out.push_back(" << get<0>(*it) << ");" << endl;
//      sim_ret_it++;
//    }
//  }
//
//  m_file << "}" << endl;
//
}

void PropNet::GenerateSeriesFunctions(std::ostream& m_file, size_t mark_index)
{
////////////////////////////////////////////////////////////////////////////////
/// Scenario 1
/// IsTerminal -> GetLegalMoves -> NextState
////////////////////////////////////////////////////////////////////////////////

  {
    tuple<bool, size_t> out;

    EntryManager global_em;

    //! Generate entries for terminal node
    global_em.StartNewList();
    tuple<bool, size_t> terminal_entry_id = terminal->CodeGen(global_em, mark_index);

    //! Generate entries for legal nodes
    list<list<tuple<size_t, size_t>>> legal_fun_m;
    list<size_t> legal_to_get_l;

    global_em.StartNewList();
    for(auto role_legals : legal_nodes)
    {
      list<tuple<size_t, size_t>> temp;
      for(auto legal : role_legals)
      {
        out = legal.second->CodeGen(global_em, mark_index);
        temp.emplace_back(legal.first, get<1>(out));
        legal_to_get_l.push_back(get<1>(out));
      }
      legal_fun_m.push_back(temp);
    }

    //! Generate entries for next nodes
    global_em.StartNewList();
    for(auto nn : next_nodes)
    {
      out = nn.second->CodeGen(global_em, mark_index);
    }

    //! Initialize entry manager
    global_em.InitializeIterator();

    //! Generate code for is terminal
    stringstream terminal_ss1;
    stringstream terminal_ss2;
    list<size_t> terminal_ret;
    {
      list<size_t> temp;
      temp.push_back(get<1>(terminal_entry_id));
      terminal_ret = global_em.CodeGen(terminal_ss1, terminal_ss2, temp);
    }

    //! Generate code for legal moves
    stringstream legal_ss1;
    stringstream legal_ss2;
    list<size_t> legal_ret = global_em.CodeGen(legal_ss1, legal_ss2, legal_to_get_l);

    //! Generate code for next state
    stringstream next_ss1;
    stringstream next_ss2;
    global_em.CodeGen(next_ss1, next_ss2, legal_to_get_l);

    ////////////////////////////////////////////////////////////////////////////
    /// Create series IsTerminal function
    ////////////////////////////////////////////////////////////////////////////

    m_file << "extern \"C\" bool IsTerminal(const State& s, bool* buff)" << endl;
    m_file << "{" << endl;

    m_file << terminal_ss1.str() << endl;

    m_file << "return buff[" << *terminal_ret.begin() << "];" << endl;

    m_file << "}" << endl << endl;

    ////////////////////////////////////////////////////////////////////////////////
  /// Create a series GetLegalMoves function
  ////////////////////////////////////////////////////////////////////////////////

    m_file << "extern \"C\" MoveList GetLegalMoves_sc1(const State& s, bool* buff)" << endl;
    m_file << "{" << endl;

    //! print the legal move generator code
    m_file << legal_ss1.str() << endl;

    m_file << "std::list<size_t> legal_moves[" << roles_ids.size() << "];" << endl;

    auto legal_ret_it = legal_ret.begin();

    size_t r_index = 0;
    for(auto role : legal_fun_m)
    {
      for(auto it : role)
      {
        m_file << "if(buff[" << *legal_ret_it << "]) legal_moves[" << r_index << "].push_back(" << get<0>(it) << ");" << endl;
        legal_ret_it++;
      }
      r_index++;
    }

    m_file << "return MoveList(legal_moves, " << roles_ids.size() << ");" << endl;

    m_file << "}" << endl << endl;

  //////////////////////////////////////////////////////////////////////////////
  /// Create series GetNextState function
  //////////////////////////////////////////////////////////////////////////////

    m_file << "extern \"C\" State GetNextState_sc1(const State& s, const Move& move, bool* buff)" << endl;
    m_file << "{" << endl;
    m_file << "State s_out(new RawState());" << endl;

    m_file << next_ss1.str() << endl;

    m_file << "return s_out;" << endl;

    m_file << "}" << endl << endl;

    ////////////////////////////////////////////////////////////////////////////
    /// Save buffer space requirements
    ////////////////////////////////////////////////////////////////////////////

    m_file << "extern \"C\" size_t GetSeriesMemoryRequirement()" << endl;
    m_file << "{" << endl;
    m_file << "return " << global_em.GetRequiredMemory() << ";" << endl;
    m_file << "}" << endl;
  }
}

void PropNet::GenerateMoveCode(std::ostream& stream)
{
  imove.CodeGen(stream);
}

void PropNet::GenerateStateCode(std::ostream& stream)
{
  istate.CodeGen(stream);
}
