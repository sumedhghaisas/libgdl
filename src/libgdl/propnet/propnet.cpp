#include "propnet.hpp"

#include "entry_manager.hpp"

#include <sstream>
#include <fstream>
#include <tuple>
#include <regex>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/unordered_map.hpp>

#include "handlers/code_handler.hpp"
#include "handlers/file_handler.hpp"

#include <libgdl/core/data_types/move_list.hpp>

using namespace std;
using namespace libgdl;
using namespace libgdl::core;
using namespace libgdl::gdlreasoner;
using namespace libgdl::gdlparser;
using namespace libgdl::propnet;
using namespace libgdl::propnet::node_types;

size_t n_debug = 0;

size_t and_debug = 0;

size_t PropNet::n_count = 0;

PropNet::PropNet(Log log)
  : terminal(NULL), c_r_id(0), c_and_id(0), c_not_id(0),
  c_or_id(0), c_view_id(0), base_mask(""), log(log)
{
  for(size_t i = 0;i < 10000;i++)
    t_data[i] = (unsigned short)-1;
}

PropNet::PropNet(const PropNet& pn)
{
  map<const Node*, Node*> copy_map;

  InitializeToRoles(pn.RoleSize());

  roles_ids = pn.roles_ids;

  for(auto it : pn.view_nodes)
  {
    it.second->CreateCopy(*this, NULL, copy_map);
  }

  for(auto it : pn.next_nodes)
  {
    it.second->CreateCopy(*this, NULL, copy_map);
  }

  for(auto it : pn.input_nodes)
  {
    for(auto it2 : it)
    {
      it2.second->CreateCopy(*this, NULL, copy_map);
    }
  }

  for(auto it : pn.legal_nodes)
  {
    for(auto it2 : it)
    {
      it2.second->CreateCopy(*this, NULL, copy_map);
    }
  }

  for(auto it : pn.goal_nodes)
  {
    for(auto it2 : it)
    {
      if(pn.del.find(it2.second) == pn.del.end())
        it2.second->CreateCopy(*this, NULL, copy_map);
    }
  }

  pn.terminal->CreateCopy(*this, NULL, copy_map);

  for(auto it : pn.base_nodes)
  {
    it.second->CreateCopy(*this, NULL, copy_map);
  }

  init_props = pn.init_props;

  base_size = pn.base_size;
  role_size = pn.role_size;

  base_mask = pn.base_mask;
}

void PropNet::Initialize(const std::string& filename)
{
  KIF kif(true, 1, log);
  kif.AddFile(filename);
  kif.Parse();

  //kif.PrintDependencyGraph("test.dot");
  //exit(1);

  sym = kif.GetSymbolTable();

  KIFFlattener kf(log);

  size_t start = util::Timer::microtimer();
  kf.Flatten(kif, *this);
  size_t end = util::Timer::microtimer();

  cout << end - start << endl;
}

void PropNet::Initialize(KIF& kif)
{
  sym = kif.GetSymbolTable();

  KIFFlattener kf(log);
  kf.Flatten(kif, *this);
}

PropNet::~PropNet()
{
  PrintPropnet("test.dot");

  for(auto it : view_nodes)
  {
    if(del.find(it.second) == del.end())
      delete it.second;
  }

  for(auto it : next_nodes)
  {
    if(del.find(it.second) == del.end())
      delete it.second;
  }

  for(auto it : input_nodes)
  {
    for(auto it2 : it)
    {
      if(del.find(it2.second) == del.end())
        delete it2.second;
    }
  }

  for(auto it : legal_nodes)
  {
    for(auto it2 : it)
    {
      if(del.find(it2.second) == del.end())
        delete it2.second;
    }
  }

  for(auto it : goal_nodes)
  {
    for(auto it2 : it)
    {
      if(del.find(it2.second) == del.end())
        delete it2.second;
    }
  }

  delete terminal;

  for(auto it : base_nodes)
  {
    if(del.find(it.second) == del.end())
      delete it.second;
  }

  for(auto it : and_nodes)
  {
    if(del.find(it) == del.end())
      delete it;
  }

  for(auto it : or_nodes)
  {
    if(del.find(it) == del.end())
      delete it;
  }

  for(auto it : not_nodes)
  {
    if(del.find(it) == del.end())
      delete it;
  }
}

void PropNet::AddFact(const Fact& f)
{
  CreateNode(sym, f.arg);
}

void PropNet::AddClause(const Clause& c)
{
  Node* head = CreateNode(sym, c.head);

  Node* a = new AndNode("And", c_and_id++);
  and_nodes.push_back(a);

  //and_debug++;
  //if(and_debug % 1000 == 0)
    //cout << "Created " << and_debug << " and nodes..." << endl;

  for(auto premiss : c.premisses)
  {
    Node* temp = CreateNode(sym, premiss);
    a->AddIn(temp);
    temp->AddOut(a);
  }

  head->AddIn(a);
  a->AddOut(head);
}

Node* PropNet::CreateNode(SymbolTable sym, const Argument* arg)
{
  std::string s_arg = arg->DecodeToString(sym);
  Node* out = NULL;

  if(arg->value == SymbolTable::RoleID)
  {
    roles_ids[sym.GetCommandName(arg->args[0]->value)] = c_r_id++;
    str_input_nodes.emplace_back();
    c_input_id.emplace_back();
    input_nodes.emplace_back();
    goal_nodes.emplace_back();
    legal_nodes.emplace_back();
  }
  else if(arg->value == SymbolTable::BaseID || arg->value == SymbolTable::TrueID)
  {
    s_arg = arg->args[0]->DecodeToString(sym);

    size_t p_id = 0;
    auto str_b_it = str_base_nodes.find(s_arg);
    if(str_b_it != str_base_nodes.end())
      p_id = str_b_it->second;
    else
    {
      p_id = c_base_id++;
      str_base_nodes[s_arg] = p_id;
    }

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
    auto r_it = roles_ids.find(sym.GetCommandName(arg->args[0]->value));
    if(r_it == roles_ids.end())
    {
      cout << LOGID << "Unrecognized role " << sym.GetCommandName(arg->args[0]->value) << "." << endl;
      exit(1);
    }

    size_t r_id = r_it->second;
    s_arg = arg->args[1]->DecodeToString(sym);

    size_t in_id = 0;

    auto str_in_it = str_input_nodes[r_id].find(s_arg);

    if(str_in_it != str_input_nodes[r_id].end())
      in_id = str_in_it->second;
    else
    {
      in_id = c_input_id[r_id]++;
      str_input_nodes[r_id][s_arg] = in_id;
    }

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

    size_t p_id = 0;
    auto str_b_it = str_base_nodes.find(s_arg);
    if(str_b_it != str_base_nodes.end())
      p_id = str_b_it->second;
    else
    {
      p_id = c_base_id++;
      str_base_nodes[s_arg] = p_id;
    }

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

    size_t in_id = 0;

    auto str_in_it = str_input_nodes[r_id].find(s_arg);

    if(str_in_it != str_input_nodes[r_id].end())
      in_id = str_in_it->second;
    else
    {
      in_id = c_input_id[r_id]++;
      str_input_nodes[r_id][s_arg] = in_id;
    }

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
  else if(arg->value == SymbolTable::AndID)
  {
    out = new AndNode(s_arg, c_and_id++);
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
    if(!arg->args.empty())
    {
      Node* temp = CreateNode(sym, arg->args[0]);
      temp->AddOut(out);
      out->AddIn(temp);
    }
  }
  else if(arg->value == SymbolTable::InitID)
  {
    s_arg = arg->args[0]->DecodeToString(sym);

    size_t p_id = 0;
    auto str_b_it = str_base_nodes.find(s_arg);
    if(str_b_it != str_base_nodes.end())
      p_id = str_b_it->second;
    else
    {
      p_id = c_base_id++;
      str_base_nodes[s_arg] = p_id;
    }

    init_props.push_back(p_id);
  }
  else
  {
    auto it = view_nodes.find(s_arg);
    if(it == view_nodes.end())
    {
      //n_debug++;
      //if(n_debug % 1000 == 0)
        //cout << "Created " << n_debug << " nodes..." << endl;

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
    if(del.find(it.second) == del.end())
      stream << it.second->UName() << " [label = \""
           << it.second->Name() << "\"];" << endl;
  }

  for(auto it : next_nodes)
  {
    if(del.find(it.second) == del.end())
      stream << it.second->UName() << " [label = \""
           << it.second->Name() << "\"];" << endl;
  }

  for(auto it : view_nodes)
  {
    if(del.find(it.second) == del.end())
      stream << it.second->UName() << " [label = \""
           << it.second->Name() << "\"];" << endl;
  }

  for(auto m : input_nodes)
  {
    for(auto it : m)
    {
      if(del.find(it.second) == del.end())
        stream << it.second->UName() << " [label = \""
             << it.second->Name() << "\"];" << endl;
    }
  }

  for(auto m : legal_nodes)
  {
    for(auto it : m)
    {
      if(del.find(it.second) == del.end())
        stream << it.second->UName() << " [label = \""
             << it.second->Name() << "\"];" << endl;
    }
  }

  for(auto m : goal_nodes)
  {
    for(auto it : m)
    {
      if(del.find(it.second) == del.end())
        stream << it.second->UName() << " [label = \""
             << it.second->Name() << "\"];" << endl;
    }
  }

  for(auto a : and_nodes)
  {
    if(del.find(a) == del.end())
    stream << a->UName() << " [label = \""
           << a->Name() << "\"];" << endl;
  }

  for(auto a : or_nodes)
  {
    if(del.find(a) == del.end())
      stream << a->UName() << " [label = \""
           << a->Name() << "\"];" << endl;
  }

  for(auto a : not_nodes)
  {
    if(del.find(a) == del.end())
      stream << a->UName() << " [label = \""
           << a->Name() << "\"];" << endl;
  }

  if(terminal != NULL)
    stream << terminal->UName() << " [label = \""
         << terminal->Name() << "\"];" << endl;

  for(auto it : view_nodes)
  {
    if(del.find(it.second) == del.end())
      for(auto n : it.second->in_degree)
      {
        stream << n->UName() << " -> " << it.second->UName() << ";" << endl;
      }
  }

  for(auto it : next_nodes)
  {
    if(del.find(it.second) == del.end())
      for(auto n : it.second->in_degree)
      {
        stream << n->UName() << " -> " << it.second->UName() << ";" << endl;
      }
  }

  for(auto a : and_nodes)
  {
    if(del.find(a) == del.end())
      for(auto n : a->in_degree)
      {
        stream << n->UName() << " -> " << a->UName() << ";" << endl;
      }
  }

  for(auto a : or_nodes)
  {
    if(del.find(a) == del.end())
      for(auto n : a->in_degree)
      {
        stream << n->UName() << " -> " << a->UName() << ";" << endl;
      }
  }

  for(auto a : not_nodes)
  {
    if(del.find(a) == del.end())
      for(auto n : a->in_degree)
      {
        stream << n->UName() << " -> " << a->UName() << ";" << endl;
      }
  }

  for(auto m : legal_nodes)
  {
    for(auto it : m)
    {
      if(del.find(it.second) == del.end())
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
      if(del.find(it.second) == del.end())
        for(auto n : it.second->in_degree)
        {
          stream << n->UName() << " -> " << it.second->UName() << ";" << endl;
        }
    }
  }

  if(terminal != NULL)
    for(auto n : terminal->in_degree)
    {
      stream << n->UName() << " -> " << terminal->UName() << ";" << endl;
    }

  stream << "}";
  stream.close();
  return true;
}

void PropNet::GenerateStateMachineCode()
{
//////////////////////////////////////////////////////////////////////////////////
///// START FILE print_functions.cpp
//////////////////////////////////////////////////////////////////////////////////
//
//  m_file.open("state_machine/print_functions.cpp");
//
//  //! Add required includes
//  m_file << "#include <libgdl/core/data_types/a_state.hpp>" << endl;
//  m_file << "#include <libgdl/core/data_types/a_move.hpp>" << endl;
//  m_file << "#include <libgdl/core/data_types/move_vector.hpp>" << endl;
//  m_file << "#include <libgdl/core/data_types/move_list.hpp>" << endl << endl;
//
//  m_file << "using namespace std;" << endl;
//  m_file << "using namespace libgdl;" << endl << endl;
//
//////////////////////////////////////////////////////////////////////////////////
///// Generate Print Functions
//////////////////////////////////////////////////////////////////////////////////
//
//  m_file << "extern \"C\" void PrintState(std::ostream& stream, const AState& s)" << endl;
//  m_file << "{" << endl;
//  m_file << "stream << s;" << endl;
//  m_file << "}" << endl << endl;
//
//  m_file << "extern \"C\" void PrintMove(std::ostream& stream, const AMove& m)" << endl;
//  m_file << "{" << endl;
//  m_file << "stream << m;" << endl;
//  m_file << "}" << endl << endl;
//
//  m_file << "extern \"C\" void PrintMoveList(std::ostream& stream, const MoveList<AMove>& ml)" << endl;
//  m_file << "{" << endl;
//  m_file << "stream << ml;" << endl;
//  m_file << "}" << endl << endl;

////////////////////////////////////////////////////////////////////////////////
/// END FILE print_functions.cpp
////////////////////////////////////////////////////////////////////////////////

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

void PropNet::GenerateSeriesFunctions(size_t mark_index)
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

////////////////////////////////////////////////////////////////////////////////
/// START FILE IsTerminal.cpp
////////////////////////////////////////////////////////////////////////////////

    CodeHandler is_terminal_ch("bool", "IsTerminal", "(const AState& s, bool* buff)", "(const AState& s, bool* buff)", "(s, buff)");

    is_terminal_ch.init_ss << "#include <libgdl/core/data_types/a_state.hpp>" << endl << endl;

    is_terminal_ch.init_ss << "using namespace std;" << endl;
    is_terminal_ch.init_ss << "using namespace libgdl;" << endl << endl;

    //! Generate code for is terminal
    list<size_t> terminal_ret;
    {
      list<size_t> temp;
      temp.push_back(get<1>(terminal_entry_id));
      terminal_ret = global_em.CodeGen(is_terminal_ch, temp);
    }

    is_terminal_ch.fun_deinit_ss << "return buff[" << *terminal_ret.begin() << "];" << endl;

    is_terminal_ch.GenerateCode();

////////////////////////////////////////////////////////////////////////////////
/// END OF FILE IsTerminal.cpp
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// START FILE GetLegalMoves_l_sci.cpp
////////////////////////////////////////////////////////////////////////////////

    CodeHandler get_legal_moves_l_sc1("MoveList<AMove>", "GetLegalMoves_l_sc1", "(const AState& s, bool* buff)", "(const AState& s, bool* buff)", "(s, buff)");

    get_legal_moves_l_sc1.init_ss << "#include <libgdl/core/data_types/a_move.hpp>" << endl;
    get_legal_moves_l_sc1.init_ss << "#include <libgdl/core/data_types/move_list.hpp>" << endl;
    get_legal_moves_l_sc1.init_ss << "#include <libgdl/core/data_types/move_vector.hpp>" << endl;
    get_legal_moves_l_sc1.init_ss << "#include <libgdl/core/data_types/a_state.hpp>" << endl << endl;

    get_legal_moves_l_sc1.init_ss << "using namespace std;" << endl;
    get_legal_moves_l_sc1.init_ss << "using namespace libgdl;" << endl << endl;

  //////////////////////////////////////////////////////////////////////////////
  /// Create a series GetLegalMoves function
  //////////////////////////////////////////////////////////////////////////////

    //! print the legal move generator code
    //! Generate code for legal moves
    list<size_t> legal_ret = global_em.CodeGen(get_legal_moves_l_sc1, legal_to_get_l);

    get_legal_moves_l_sc1.fun_deinit_ss << "std::list<size_t> legal_moves[" << roles_ids.size() << "];" << endl;

    auto legal_ret_it = legal_ret.begin();

    size_t r_index = 0;
    for(auto role : legal_fun_m)
    {
      for(auto it : role)
      {
        get_legal_moves_l_sc1.fun_deinit_ss << "if(buff[" << *legal_ret_it << "]) legal_moves[" << r_index << "].push_back(" << get<0>(it) << ");" << endl;
        legal_ret_it++;
      }
      r_index++;
    }

    get_legal_moves_l_sc1.fun_deinit_ss << "return MoveList<AMove>(legal_moves, " << roles_ids.size() << ");" << endl;

    //get_legal_moves_l_sc1.GenerateCode();

////////////////////////////////////////////////////////////////////////////////
/// END FILE GetLegalMoves_l_sci.cpp
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// START FILE GetLegalMoves_v_sci.cpp
////////////////////////////////////////////////////////////////////////////////

    CodeHandler get_legal_moves_v_sc1("MoveVector<AMove>", "GetLegalMoves_v_sc1", "(const AState& s, bool* buff)", "(const AState& s, bool* buff)", "(s, buff)");

    get_legal_moves_v_sc1.s_entries = get_legal_moves_l_sc1.s_entries;
    get_legal_moves_v_sc1.init_ss << get_legal_moves_l_sc1.init_ss.str();

    get_legal_moves_v_sc1.fun_deinit_ss << "std::list<size_t> legal_moves[" << roles_ids.size() << "];" << endl;

    legal_ret_it = legal_ret.begin();

    r_index = 0;
    for(auto role : legal_fun_m)
    {
      for(auto it : role)
      {
        get_legal_moves_v_sc1.fun_deinit_ss << "if(buff[" << *legal_ret_it << "]) legal_moves[" << r_index << "].push_back(" << get<0>(it) << ");" << endl;
        legal_ret_it++;
      }
      r_index++;
    }

    get_legal_moves_v_sc1.fun_deinit_ss << "return MoveVector<AMove>(legal_moves, " << roles_ids.size() << ");" << endl;

    //get_legal_moves_v_sc1.GenerateCode();

////////////////////////////////////////////////////////////////////////////////
/// END OF FILE GetLegalMoves_v_sc1.cpp
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// START FILE GetNextState_sc1.cpp
////////////////////////////////////////////////////////////////////////////////

    CodeHandler get_next_state_sc1("AState", "GetNextState_sc1", "(const AState& s, const AMove& move, bool* buff)", "(const AState& s, const AMove& move, bool* buff, AState& s_out)", "(s, move, buff, s_out)");

    get_next_state_sc1.init_ss << "#include <libgdl/core/data_types/a_move.hpp>" << endl;
    get_next_state_sc1.init_ss << "#include <libgdl/core/data_types/a_state.hpp>" << endl << endl;

    get_next_state_sc1.init_ss << "using namespace std;" << endl;
    get_next_state_sc1.init_ss << "using namespace libgdl;" << endl << endl;

  //////////////////////////////////////////////////////////////////////////////
  /// Create series GetNextState function
  //////////////////////////////////////////////////////////////////////////////

    get_next_state_sc1.fun_init_ss << "AState s_out(new core::RawAState());" << endl;

    //! Generate code for next state
    global_em.CodeGen(get_next_state_sc1, legal_to_get_l);

    get_next_state_sc1.fun_deinit_ss << "return s_out;" << endl;

    //get_next_state_sc1.GenerateCode();

////////////////////////////////////////////////////////////////////////////////
/// END OF FILE get_next_state_sci.cpp
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// Save buffer space requirements
////////////////////////////////////////////////////////////////////////////////

    CodeHandler GetSeriesMemoryRequirement("size_t", "GetSeriesMemoryRequirement", "()", "()", "()");

    GetSeriesMemoryRequirement.init_ss << "#include <iostream>" << endl
                                       << "using namespace std;" << endl;

    GetSeriesMemoryRequirement.fun_deinit_ss << "return " << global_em.GetRequiredMemory() << ";" << endl;

    //GetSeriesMemoryRequirement.GenerateCode();
  }
}

void PropNet::GenerateStateMachine()
{
////////////////////////////////////////////////////////////////////////////////
/// Generate init_state.cpp
////////////////////////////////////////////////////////////////////////////////

  CodeHandler init_state("AState", "InitState", "()", "()", "()");

  init_state.init_ss << "#include <libgdl/core/data_types/a_state.hpp>" << endl;

  init_state.init_ss << "using namespace std;" << endl;
  init_state.init_ss << "using namespace libgdl;" << endl << endl;

  init_state.fun_init_ss << "AState init(new core::RawAState());" << endl;

  for(auto i_prop : init_props)
  {
    stringstream ss;
    ss << "init->Set(" << i_prop << ", true);";
    init_state.AddEntry(ss.str());
  }

  init_state.fun_deinit_ss << "return init;" << std::endl;
  init_state.GenerateCode();

////////////////////////////////////////////////////////////////////////////////
/// Series Functions generation
////////////////////////////////////////////////////////////////////////////////

  GenerateSeriesFunctions(0);

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

////////////////////////////////////////////////////////////////////////////////
/// START FILE GetGoals.cpp
////////////////////////////////////////////////////////////////////////////////

  CodeHandler GetGoals("std::list<size_t>", "GetGoals", "(const AState& s, bool* buff)", "(const AState& s, bool* buff)", "(s, buff)");

  //! Add required includes
  GetGoals.init_ss << "#include <libgdl/core/data_types/a_state.hpp>" << endl;
  GetGoals.init_ss << "#include <list>" << endl;

  GetGoals.init_ss << "using namespace std;" << endl;
  GetGoals.init_ss << "using namespace libgdl;" << endl << endl;

  //! Generate code for GetGoals
  list<size_t> goals_ret = gg_em.CodeGen(GetGoals, goal_to_get_l);

  auto ret_it = goals_ret.begin();

  GetGoals.fun_deinit_ss << "std::list<size_t> out;" << endl;

  for(auto role : goal_fun_m)
  {
    auto it = role.begin();
    GetGoals.fun_deinit_ss << "if(buff[" << *ret_it << "]) out.push_back(" << get<0>(*it) << ");" << endl;
    ret_it++;
    it++;
    for(it = it;it != role.end();it++)
    {
      GetGoals.fun_deinit_ss << "else if(buff[" << *ret_it << "]) out.push_back(" << get<0>(*it) << ");" << endl;
      ret_it++;
    }
  }

  GetGoals.fun_deinit_ss << "return out;" << endl;

  GetGoals.GenerateCode();

////////////////////////////////////////////////////////////////////////////////
/// END FILE GetGoals.cpp
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// START FILE GetGoalMemoryRequirement.cpp
////////////////////////////////////////////////////////////////////////////////

  CodeHandler GetGoalMemoryRequirement("size_t", "GetGoalMemoryRequirement", "()", "()", "()");

  GetGoalMemoryRequirement.init_ss << "#include <iostream>" << endl
                                   << "using namespace std;" << endl;

  GetGoalMemoryRequirement.fun_deinit_ss << "return " << gg_em.GetRequiredMemory() << ";" << endl;

  GetGoalMemoryRequirement.GenerateCode();

////////////////////////////////////////////////////////////////////////////////
/// END FILE GetGoalMemoryRequirement.cpp
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// Generate GetNextState function
////////////////////////////////////////////////////////////////////////////////

  EntryManager gns_em;

  //! Generate entries for next nodes
  gns_em.StartNewList();
  for(auto nn : next_nodes)
  {
    out = nn.second->CodeGen(gns_em, 2);
  }

  //! Initialize entry manager
  gns_em.InitializeIterator();

////////////////////////////////////////////////////////////////////////////////
/// START FILE GetNextState.cpp
////////////////////////////////////////////////////////////////////////////////

  CodeHandler GetNextState("AState", "GetNextState", "(const AState& s, const AMove& move, bool* buff)", "(const AState& s, const AMove& move, bool* buff, AState& s_out)", "(s, move, buff, s_out)");

  //! Add required includes
  GetNextState.init_ss << "#include <libgdl/core/data_types/a_state.hpp>" << endl;
  GetNextState.init_ss << "#include <libgdl/core/data_types/a_move.hpp>" << endl;

  GetNextState.init_ss << "using namespace std;" << endl;
  GetNextState.init_ss << "using namespace libgdl;" << endl << endl;

  GetNextState.fun_init_ss << "AState s_out(new core::RawAState());" << endl;

  std::list<size_t> next_temp;

  //! Generate code for next state
  gns_em.CodeGen(GetNextState, next_temp);

  GetNextState.fun_deinit_ss << "return s_out;" << endl;

  //GetNextState.GenerateCode();

////////////////////////////////////////////////////////////////////////////////
/// END FILE GetNextState.cpp
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// Generate GetLegalMoves functions
////////////////////////////////////////////////////////////////////////////////

  EntryManager glm_em;

  //! Generate entries for legal nodes
  list<list<tuple<size_t, size_t>>> legal_fun_m;
  list<size_t> legal_to_get_l;

  glm_em.StartNewList();
  for(auto role_legals : legal_nodes)
  {
    list<tuple<size_t, size_t>> temp;
    for(auto legal : role_legals)
    {
      out = legal.second->CodeGen(glm_em, 3);
      temp.emplace_back(legal.first, get<1>(out));
      legal_to_get_l.push_back(get<1>(out));
    }
    legal_fun_m.push_back(temp);
  }

  //! Initialize entry manager
  glm_em.InitializeIterator();

////////////////////////////////////////////////////////////////////////////////
/// START FILE GetLegalMoves_l
////////////////////////////////////////////////////////////////////////////////

  CodeHandler GetLegalMoves_l("MoveList<AMove>", "GetLegalMoves_l", "(const AState& s, bool* buff)", "(const AState& s, bool* buff)", "(s, buff)");

  //! Add required includes
  GetLegalMoves_l.init_ss << "#include <libgdl/core/data_types/a_state.hpp>" << endl;
  GetLegalMoves_l.init_ss << "#include <libgdl/core/data_types/a_move.hpp>" << endl;
  GetLegalMoves_l.init_ss << "#include <libgdl/core/data_types/move_vector.hpp>" << endl;
  GetLegalMoves_l.init_ss << "#include <libgdl/core/data_types/move_list.hpp>" << endl << endl;

  GetLegalMoves_l.init_ss << "using namespace std;" << endl;
  GetLegalMoves_l.init_ss << "using namespace libgdl;" << endl << endl;

  //! print the legal move generator code
  //! Generate code for legal moves
  list<size_t> legal_ret = glm_em.CodeGen(GetLegalMoves_l, legal_to_get_l);

  GetLegalMoves_l.fun_deinit_ss << "std::list<size_t> legal_moves[" << roles_ids.size() << "];" << endl;

  auto legal_ret_it = legal_ret.begin();

  size_t r_index = 0;
  for(auto role : legal_fun_m)
  {
    for(auto it : role)
    {
      GetLegalMoves_l.fun_deinit_ss << "if(buff[" << *legal_ret_it << "]) legal_moves[" << r_index << "].push_back(" << get<0>(it) << ");" << endl;
      legal_ret_it++;
    }
    r_index++;
  }

  GetLegalMoves_l.fun_deinit_ss << "return MoveList<AMove>(legal_moves, " << roles_ids.size() << ");" << endl;

  GetLegalMoves_l.GenerateCode();

////////////////////////////////////////////////////////////////////////////////
/// END FILE GetLegalMoves_l
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// START FILE GetLegalMoves_l
////////////////////////////////////////////////////////////////////////////////

  CodeHandler GetLegalMoves_v("MoveVector<AMove>", "GetLegalMoves_v", "(const AState& s, bool* buff)", "(const AState& s, bool* buff)", "(s, buff)");
  GetLegalMoves_v.s_entries = GetLegalMoves_l.s_entries;
  GetLegalMoves_v.init_ss << GetLegalMoves_l.init_ss.str();

  GetLegalMoves_v.fun_deinit_ss << "std::list<size_t> legal_moves[" << roles_ids.size() << "];" << endl;

  legal_ret_it = legal_ret.begin();

  r_index = 0;
  for(auto role : legal_fun_m)
  {
    for(auto it : role)
    {
      GetLegalMoves_v.fun_deinit_ss << "if(buff[" << *legal_ret_it << "]) legal_moves[" << r_index << "].push_back(" << get<0>(it) << ");" << endl;
      legal_ret_it++;
    }
    r_index++;
  }

  GetLegalMoves_v.fun_deinit_ss << "return MoveVector<AMove>(legal_moves, " << roles_ids.size() << ");" << endl;

  GetLegalMoves_v.GenerateCode();

////////////////////////////////////////////////////////////////////////////////
/// END FILE GetLegalMoves_l
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// Generate Move Creator
////////////////////////////////////////////////////////////////////////////////

  CodeHandler CreateMove("AMove", "CreateMove", "(const std::list<std::string>& s_moves)", "(const std::list<std::string>& s_moves)", "(s_moves)");
  CreateMove.init_ss << GetLegalMoves_l.init_ss.str();

  CreateMove.fun_deinit_ss << "return AMove(s_moves);" << endl;

  CreateMove.GenerateCode();

  FileHandler::GetMasterFileHandler().AddFile("state_machine/a_move");

  FileHandler::GetMasterFileHandler().GenerateSharedObject();
}

void PropNet::PrimaryRun(AState& s, Set<size_t>* m_set, size_t* goals)
{
  if(terminal != NULL)
    terminal->InitializeValue(*this, s, m_set, goals);

  for(auto it : goal_nodes)
    for(auto it2 : it)
      if(del.find(it2.second) == del.end())
        it2.second->InitializeValue(*this, s, m_set, goals);

  for(auto it : legal_nodes)
    for(auto it2 : it)
      if(del.find(it2.second) == del.end())
        it2.second->InitializeValue(*this, s, m_set, goals);

  for(auto it : next_nodes)
    if(del.find(it.second) == del.end())
      it.second->InitializeValue(*this, s, m_set, goals);

  base_mask = AState("");

  //! Initialize base nodes for initial_pn
  arr_base_nodes = new Node*[base_size];
  for(size_t i = 0;i < base_size;i++)
    arr_base_nodes[i] = NULL;
  for(auto it : BaseNodes())
  {
    if(del.find(it.second) == del.end())
    {
      arr_base_nodes[it.first] = it.second;
      base_mask.Set(it.first, true);
    }
  }

  //! Initialize input nodes for initial_pn
  arr_input_nodes = new Node**[role_size];
  size_t index = 0;
  for(auto it : InputNodes())
  {
    arr_input_nodes[index] = new Node*[it.size()];
    for(auto it2 : it)
    {
      arr_input_nodes[index][it2.first] = it2.second;
    }
    index++;
  }
}

void PropNet::SplitGoalNet(PropNet& goal_net)
{
  map<const Node*, Node*> node_map;

  for(auto it : goal_nodes)
    for(auto it2 : it)
      it2.second->CreateCopy(goal_net, NULL, node_map);

  for(auto it : legal_nodes)
    for(auto it2 : it)
      it2.second->DFSMark(1);

  for(auto it : next_nodes)
    it.second->DFSMark(1);

  terminal->DFSMark(1);

  for(auto it : goal_nodes)
    for(auto it2 : it)
      it2.second->DeleteIfNotMarked(NULL, del, 1);

  goal_net.base_size = base_size;
  goal_net.role_size = role_size;
}

void PropNet::ProcessDOTtoken(const string& token,
                              list<pair<string, string>>& edges,
                              map<string, string>& t_edges,
                              map<string, Node*>& nodes,
                              map<string, Node*>& n_nodes)
{
  regex word_regex(".*->.*");
  if(regex_search(token, word_regex))
  {
    regex reg1("@([0-9abcdef])+");
    auto words_begin = sregex_iterator(token.begin(), token.end(), reg1);
    auto words_end = sregex_iterator();
    string from = words_begin->str();
    string to = (++words_begin)->str();
    t_edges[from] = to;
    edges.emplace_back(from, to);
  }
  else
  {
    regex id_reg("@([0-9abcdef])+");
    string id = sregex_iterator(token.begin(), token.end(), id_reg)->str();

    regex label_reg("label=\".*\"");
    string label = sregex_iterator(token.begin(), token.end(), label_reg)->str();
    label = label.substr(7, label.length() - 8);
    boost::algorithm::to_lower(label);

    Node* to_add = NULL;

    if(label != "transition" && label != "true")
    {
      Argument arg(label, sym, true);

      to_add = CreateNode(sym, &arg);

      if(arg.value == SymbolTable::TrueID)
      {
        arg.value = SymbolTable::NextID;
        Node* t = CreateNode(sym, &arg);
        n_nodes[id] = t;
      }
      else if(arg.value == SymbolTable::LegalID)
      {
        arg.value = SymbolTable::InputID;
        CreateNode(sym, &arg);
      }
    }
    nodes[id] = to_add;
  }
}

void ProcessEdges(const list<pair<string, string>>& edges,
                  const map<string, string>& t_edges,
                  const map<string, Node*>& nodes,
                  const map<string, Node*>& n_nodes)
{
  for(auto it : edges)
  {
    Node* from = nodes.find(it.first)->second;
    if(from == NULL)
      continue;

    Node* to = nodes.find(it.second)->second;
    if(to == NULL)
    {
      auto it2 = t_edges.find(it.second);
      if(it2 == t_edges.end())
      {
        cout << LOGID << "Unexpected error!" << endl;
        exit(1);
      }

      if(n_nodes.find(it2->second) == n_nodes.end())
      {
        cout << LOGID << "Unexpected error" << endl;
        exit(1);
      }

      to = n_nodes.find(it2->second)->second;
    }
    from->out_degree.push_back(to);
    to->in_degree.push_back(from);
  }
}

bool PropNet::InitializeWithDOT(const KIF& kif,
                                const std::string& dot_filename)
{
  sym = kif.GetSymbolTable();

  for(auto it : kif.Facts())
  {
    if(it.arg->value == SymbolTable::RoleID)
      CreateNode(sym, it.arg);
  }

  ifstream file(dot_filename);
  if(!file.is_open())
  {
    log.Fatal << "Could not open file " << dot_filename << endl;
    exit(1);
  }

  list<pair<string, string>> edges;
  map<string, string> t_edges;

  map<string, Node*> nodes;
  map<string, Node*> n_nodes;

  if(!file.is_open())
  {
    return false;
  }

  string line;

  getline(file, line);
  getline(file, line);

  while(getline(file, line))
  {
    boost::char_separator<char> sep(";");
    boost::tokenizer<boost::char_separator<char>> tokens(line, sep);
    for(auto it : tokens)
    {
      boost::trim(it);
      if(it != "" && it != "}")
      {
        ProcessDOTtoken(it, edges, t_edges, nodes, n_nodes);
      }
    }
  }

  ProcessEdges(edges, t_edges, nodes, n_nodes);

  base_size = base_nodes.size();

  role_size = roles_ids.size();

  InitializePrintFunctions();

  return true;
}

string PropNet::CreateGetGoalMachineCode()
{
  FileHandler fh;

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

  CodeHandler GetGoals("void", "GetGoals", "(const AState& s, size_t* goals, bool* buff)", "(const AState& s, bool* buff)", "(s, buff)");

  //! Add required includes
  GetGoals.init_ss << "#include <libgdl/core/data_types/a_state.hpp>" << endl;
  GetGoals.init_ss << "#include <list>" << endl;

  GetGoals.init_ss << "using namespace std;" << endl;
  GetGoals.init_ss << "using namespace libgdl;" << endl << endl;

  //! Generate code for GetGoals
  list<size_t> goals_ret = gg_em.CodeGen(GetGoals, goal_to_get_l);

  auto ret_it = goals_ret.begin();

  size_t r_id = 0;
  for(auto role : goal_fun_m)
  {
    auto it = role.begin();
    GetGoals.fun_deinit_ss << "if(buff[" << *ret_it << "]) goals[" << r_id << "] = " << get<0>(*it) << ";" << endl;
    ret_it++;
    it++;
    for(it = it;it != role.end();it++)
    {
      GetGoals.fun_deinit_ss << "else if(buff[" << *ret_it << "]) goals[" << r_id << "] = " << get<0>(*it) << ";" << endl;
      ret_it++;
    }
    r_id++;
  }

  GetGoals.GenerateCode(fh);

  CodeHandler GetGoalMemoryRequirement("size_t", "GetGoalsMemoryRequirement", "()", "()", "()");

  GetGoalMemoryRequirement.init_ss << "#include <iostream>" << endl
                                   << "using namespace std;" << endl;

  GetGoalMemoryRequirement.fun_deinit_ss << "return " << gg_em.GetRequiredMemory() << ";" << endl;

  GetGoalMemoryRequirement.GenerateCode(fh);

  fh.GenerateSharedObject("GetGoals.so");

  return "GetGoals.so";
}

map<const Node*, size_t> PropNet::Crystallize(signed short*& data_init, AState& top, Set<size_t>* m_set, size_t* goals)
{
  map<const Node*, size_t> id_map;
  map<size_t, CrystalData> data_map;

  size_t current_index = 0;

  for(auto it : input_nodes)
  {
    for(auto it2 : it)
    {
      if(del.find(it2.second) == del.end())
        it2.second->Crystallize(id_map, data_map, current_index);
    }
  }

  for(auto it : base_nodes)
  {
    if(del.find(it.second) == del.end())
      it.second->Crystallize(id_map, data_map, current_index);
  }

  for(auto it : legal_nodes)
  {
    for(auto it2 : it)
    {
      if(del.find(it2.second) == del.end())
        it2.second->Crystallize(id_map, data_map, current_index);
    }
  }

  for(auto it : goal_nodes)
  {
    for(auto it2 : it)
    {
      if(del.find(it2.second) == del.end())
        it2.second->Crystallize(id_map, data_map, current_index);
    }
  }

  for(auto it : next_nodes)
  {
    if(del.find(it.second) == del.end())
      it.second->Crystallize(id_map, data_map, current_index);
  }

  if(terminal != NULL)
    terminal->Crystallize(id_map, data_map, current_index);

  list<unsigned short> out_list;
  data_init = new signed short[data_map.size()];

  map<unsigned short, unsigned short> temp_to_o;

  for(size_t i = 0;i < data_map.size();i++)
  {
    CrystalData& cd = data_map.find(i)->second;

    CrystalNode t_cn;
    t_cn.data_id = i;

    if(cd.type == 0)
    {
      data_init[i] = 0x4000;
      t_cn.type = false;
    }
    else if(cd.type == 1)
    {
      data_init[i] = 0xbfff;
      t_cn.type = false;
    }
    else if(cd.type == 2)
    {
      data_init[i] = 0x8000;
      t_cn.type = false;
    }
    else if(cd.type == 3)
    {
      t_cn.type = true;
      t_cn.out_size = 4;

      temp_to_o[i] = out_list.size();
      unsigned short* temp = (unsigned short*)&t_cn;
      out_list.push_back(temp[0]);
      out_list.push_back(temp[1]);

      //cout << cry[i].offset << endl;
      //cout << out_list.size() << " " << std::hex << (size_t)cd.node << std::dec << endl;
      size_t t = (size_t)cd.node;
      temp = (unsigned short*)&t;
      out_list.push_back(temp[0]);
      out_list.push_back(temp[1]);
      out_list.push_back(temp[2]);
      out_list.push_back(temp[3]);
      continue;
    }

    temp_to_o[i] = out_list.size();
    t_cn.out_size = cd.out_degree.size();

    unsigned short* temp = (unsigned short*)&t_cn;
    out_list.push_back(temp[0]);
    out_list.push_back(temp[1]);

    //cout << cry[i].offset << endl;

    for(auto it : cd.out_degree)
    {
      if(it > 65536)
      {
        cout << "Out of bound while crystallizing." << endl;
        exit(1);
      }
      out_list.push_back((unsigned short)it);
    }
  }

  arr_propnet = new unsigned short[out_list.size()];

  size_t c_index = 0;
  auto it = out_list.begin();
  while(true)
  {
    arr_propnet[c_index] = *it;
    CrystalNode* t_cn = (CrystalNode*)(arr_propnet + c_index);

    c_index++;
    it++;

    arr_propnet[c_index] = *it;
    c_index++;
    it++;

    if(!t_cn[0].type)
    {
      for(size_t i = 0 ;i < t_cn[0].out_size;i++)
      {
        arr_propnet[c_index] = temp_to_o.find(*it)->second;
        c_index++;
        it++;
      }
    }
    else
    {
      for(size_t i = 0 ;i < t_cn[0].out_size;i++)
      {
        arr_propnet[c_index] = *it;
        c_index++;
        it++;
      }
    }

    if(it == out_list.end())
      break;
  }

  set<const Node*> initialized;

  for(auto it : legal_nodes)
    for(auto it2 : it)
      if(del.find(it2.second) == del.end())
        it2.second->CrystalInitialize(*this, id_map, data_init, top, m_set, goals, initialized);

  //MoveList<AMove> ml = MoveList<AMove>(m_set, roles_ids.size());

  //PrintMoveList(cout, ml);

  for(auto it : next_nodes)
    if(del.find(it.second) == del.end())
      it.second->CrystalInitialize(*this, id_map, data_init, top, m_set, goals, initialized);

  for(auto it : goal_nodes)
    for(auto it2 : it)
      if(del.find(it2.second) == del.end())
        it2.second->CrystalInitialize(*this, id_map, data_init, top, m_set, goals, initialized);

  if(terminal != NULL)
    terminal->CrystalInitialize(*this, id_map, data_init, top, m_set, goals, initialized);

  //PrintState(cout, top);

  data_init_size = data_map.size();

  terminal_crystal_id = id_map.find(GetTerminalNode())->second;

  base_crystal_ids = new unsigned short[BaseSize()];
  for(size_t i = 0;i < BaseSize();i++)
    base_crystal_ids[i] = 0;
  for(auto it : base_nodes)
  {
    base_crystal_ids[it.first] = temp_to_o.find(id_map.find(it.second)->second)->second;
  }

  input_crystal_ids = new unsigned short*[roles_ids.size()];

  size_t index = 0;
  for(auto it : input_nodes)
  {
    input_crystal_ids[index] = new unsigned short[it.size()];
    for(size_t i = 0;i < it.size();i++)
    {
      input_crystal_ids[index][i] = temp_to_o.find(id_map.find(input_nodes[index].find(i)->second)->second)->second;
    }
    index++;
  }

  base_mask = AState("");

  for(auto it : base_nodes)
    if(del.find(it.second) == del.end())
      base_mask.Set(it.first, true);

  return id_map;
}

void PropNet::InitializePrintFunctions() const
{
  vector<string> str_base_props;
  for(size_t i = 0;i < base_size;i++)
  {
    str_base_props.push_back(base_nodes.find(i)->second->Name());
  }
  AState::InitializePrint(str_base_props);

  vector<vector<string>> str_input_props;
  for(auto it : input_nodes)
  {
    vector<string> temp;
    for(size_t i = 0;i < it.size();i++)
      temp.push_back(it.find(i)->second->name);

    str_input_props.push_back(temp);
  }
  AMove::InitializePrint(str_input_props);
}
