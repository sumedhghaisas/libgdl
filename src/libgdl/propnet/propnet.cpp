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

size_t PropNet::debug_time = 0;

PropNet::PropNetPayLoad::~PropNetPayLoad()
{
  if(isCrystallized)
  {
    base->s = NULL;
    top->s = NULL;
    base_move->moves = NULL;
    delete[] crystal_buffer;
  }
  else
  {
    delete[] legal_size;
    delete[] goals;
    delete[] data;
    delete[] t_stack;
  }
}

void PropNet::PropNetPayLoad::Crystallize(size_t data_init_size, size_t stack_size)
{
  size_t mem_size_in_bytes = 0;

  size_t state_size = StateType::RawType::arr_size * sizeof(char);
  size_t move_size = MoveType::RawType::n_roles * sizeof(MoveType::RawType::BuffType);
  size_t legal_size_size = MoveType::RawType::n_roles * sizeof(size_t);
  size_t goal_size = MoveType::RawType::n_roles * sizeof(size_t);
  size_t data_size = data_init_size * sizeof(signed short);
  size_t t_stack_size = stack_size * sizeof(int);

  //! Add the memory required for base
  mem_size_in_bytes += state_size;
  //! Add the memory required for top
  mem_size_in_bytes += state_size;
  //! Add the memory required for base_move
  mem_size_in_bytes += move_size;
  //! Add the memory required for legal_size
  mem_size_in_bytes += legal_size_size;
  //! Add the memory required for goals
  mem_size_in_bytes += goal_size;
  //! Add the memory required for data
  mem_size_in_bytes += data_size;
  //! Add the memory required for n_stack
  mem_size_in_bytes += t_stack_size;

  crystal_buffer = new char[mem_size_in_bytes];

  char* current = crystal_buffer;

  //! Relocate base to crystal
  base.Relocate(current);
  current = current + state_size;

  //! Relocate top to crystal
  top.Relocate(current);
  current = current + state_size;

  //! Relocate base_move to crystal
  base_move.Relocate((size_t*)current);
  current = current + move_size;

  //! Relocate data to crystal
  signed short* data_t = (signed short*)current;
  for(size_t i = 0;i < data_init_size;i++)
    data_t[i] = data[i];
  delete[] data;
  data = data_t;
  current = current + data_size;

  //! Relocate t_stack to crystal
  int* t_stack_t = (int*)current;
  for(size_t i = 0;i < stack_size;i++)
    t_stack_t[i] = t_stack[i];
  delete[] t_stack;
  t_stack = t_stack_t;
  current = current + t_stack_size;

  //! Relocate legal_size to crystal
  size_t* legal_size_t = (size_t*)current;
  for(size_t i = 0;i < MoveType::RawType::n_roles;i++)
    legal_size_t[i] = legal_size[i];
  delete[] legal_size;
  legal_size = legal_size_t;
  current = current + legal_size_size;

  //! Relocate goals to crystal
  size_t* goals_t = (size_t*)current;
  for(size_t i = 0;i < MoveType::RawType::n_roles;i++)
    goals_t[i] = goals[i];
  delete[] goals;
  goals = goals_t;
  current = current + goal_size;

  isCrystallized = true;
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

  if(pn.terminal != NULL)
  {
    pn.terminal->CreateCopy(*this, NULL, copy_map);
  }

  for(auto it : pn.base_nodes)
  {
    it.second->CreateCopy(*this, NULL, copy_map);
  }

  init_props = pn.init_props;

  base_size = pn.base_size;
  role_size = pn.role_size;
  base_mask = pn.base_mask;
}

PropNet::~PropNet()
{
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

  delete[] arr_propnet;
  delete[] base_crystal_ids;
  if(input_crystal_ids != NULL)
    for(size_t i = 0;i < role_size;i++)
    {
      delete[] input_crystal_ids[i];
      delete[] legal_memory_ids[i];
    }
  delete[] input_crystal_ids;
  delete[] legal_memory_ids;
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

}

void PropNet::InitState(StateType& init)
{
  bool temp = true;

  init.Clear();

  init.Get(0, temp);

  for(auto it : init_props)
  {
    init.Set(it, true);
  }
}

void PropNet::GetPropNetBaseMask(StateType& s)
{
  s.Clear();
  for(auto it : base_nodes)
    if(del.find(it.second) == del.end())
      s.Set(it.first, true);
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
      log.Fatal << LOGID << "Unrecognized role " << sym.GetCommandName(arg->args[0]->value) << " in token .";
      SymbolDecodeStream sds(sym, cout);
      sds << *arg << endl;
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
    auto r_it = roles_ids.find(sym.GetCommandName(arg->args[0]->value));
    if(r_it == roles_ids.end())
    {
      log.Fatal << LOGID << "Unrecognized role " << sym.GetCommandName(arg->args[0]->value) << " in token .";
      SymbolDecodeStream sds(sym, cout);
      sds << *arg << endl;
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
    and_nodes.push_back(out);
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

    auto it = base_nodes.find(p_id);
    if(it == base_nodes.end())
    {
      out = new BaseNode(s_arg, p_id);
      base_nodes[p_id] = out;
    }
    else out = it->second;

    out = NULL;

    init_props.push_back(p_id);
  }
  else
  {
    if(s_arg == "init")
      return NULL;
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
           << it.second->UName() << "\"];" << endl;
  }

  for(auto it : next_nodes)
  {
    if(del.find(it.second) == del.end())
      stream << it.second->UName() << " [label = \""
           << it.second->UName() << "\"];" << endl;
  }

  for(auto it : view_nodes)
  {
    if(del.find(it.second) == del.end())
      stream << it.second->UName() << " [label = \""
           << it.second->UName() << "\"];" << endl;
  }

  for(auto m : input_nodes)
  {
    for(auto it : m)
    {
      if(del.find(it.second) == del.end())
        stream << it.second->UName() << " [label = \""
             << it.second->UName() << "\"];" << endl;
    }
  }

  for(auto m : legal_nodes)
  {
    for(auto it : m)
    {
      if(del.find(it.second) == del.end())
        stream << it.second->UName() << " [label = \""
             << it.second->UName() << "\"];" << endl;
    }
  }

  for(auto m : goal_nodes)
  {
    for(auto it : m)
    {
      if(del.find(it.second) == del.end())
        stream << it.second->UName() << " [label = \""
             << it.second->UName() << "\"];" << endl;
    }
  }

  for(auto a : and_nodes)
  {
    if(del.find(a) == del.end())
    stream << a->UName() << " [label = \""
           << a->UName() << "\"];" << endl;
  }

  for(auto a : or_nodes)
  {
    if(del.find(a) == del.end())
      stream << a->UName() << " [label = \""
           << a->UName() << "\"];" << endl;
  }

  for(auto a : not_nodes)
  {
    if(del.find(a) == del.end())
      stream << a->UName() << " [label = \""
           << a->UName() << "\"];" << endl;
  }

  if(terminal != NULL)
    stream << terminal->UName() << " [label = \""
         << terminal->UName() << "\"];" << endl;

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

void PropNet::SplitTerminalNet(PropNet& terminal_net)
{
  map<const Node*, Node*> node_map;

  terminal->CreateCopy(terminal_net, NULL, node_map);

  for(auto it : legal_nodes)
    for(auto it2 : it)
      it2.second->DFSMark(2);

  for(auto it : next_nodes)
    it.second->DFSMark(2);

  terminal->DeleteIfNotMarked(NULL, del, 2);

  terminal = NULL;

  terminal_net.base_size = base_size;
  terminal_net.role_size = role_size;
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

string PropNet::CreateIsTerminalMachineCode()
{
  FileHandler fh;

  EntryManager t_em;

  tuple<bool, size_t> out;

  //! Generate entries for goal nodes
  t_em.StartNewList();
  list<size_t> terminal_to_get_l;

  list<tuple<size_t, size_t>> temp;
  terminal_to_get_l.push_back(std::get<1>(terminal->CodeGen(t_em, 1)));

  //! Initialize the entry manager to start
  t_em.InitializeIterator();

  CodeHandler IsTerminal("bool", "IsTerminal", "(const AState& s, bool* buff)", "(const AState& s, bool* buff)", "(s, buff)");

  //! Add required includes
  IsTerminal.init_ss << "#include <libgdl/core/data_types/a_state.hpp>" << endl;

  IsTerminal.init_ss << "using namespace std;" << endl;
  IsTerminal.init_ss << "using namespace libgdl;" << endl << endl;

  //! Generate code for GetGoals
  list<size_t> terminal_ret = t_em.CodeGen(IsTerminal, terminal_to_get_l);

  IsTerminal.fun_deinit_ss << "return buff[" << *terminal_ret.begin() << "];" << endl;

  IsTerminal.GenerateCode(fh);

  CodeHandler IsTerminalMemoryRequirement("size_t", "IsTerminalMemoryRequirement", "()", "()", "()");

  IsTerminalMemoryRequirement.init_ss << "#include <iostream>" << endl
                                   << "using namespace std;" << endl;

  IsTerminalMemoryRequirement.fun_deinit_ss << "return " << t_em.GetRequiredMemory() << ";" << endl;

  IsTerminalMemoryRequirement.GenerateCode(fh);

  fh.GenerateSharedObject("IsTerminal.so");

  return "IsTerminal.so";
}

map<const Node*, size_t> PropNet::Crystallize(signed short*& data_init, AState& top, MoveSet* m_set, size_t* goals)
{
  map<const Node*, size_t> id_map;
  map<size_t, CrystalData> data_map;
  map<size_t, size_t> init_map;

  size_t current_index = 0;
  size_t current_m_index = 0;

  for(auto it : input_nodes)
  {
    for(auto it2 : it)
    {
      if(del.find(it2.second) == del.end())
        it2.second->Crystallize(id_map, data_map, init_map, current_index, current_m_index);
    }
  }

  for(auto it : base_nodes)
  {
    if(del.find(it.second) == del.end())
      it.second->Crystallize(id_map, data_map, init_map, current_index, current_m_index);
  }

  for(auto it : legal_nodes)
  {
    for(auto it2 : it)
    {
      if(del.find(it2.second) == del.end())
        it2.second->Crystallize(id_map, data_map, init_map, current_index, current_m_index);
    }
  }

  for(auto it : goal_nodes)
  {
    for(auto it2 : it)
    {
      if(del.find(it2.second) == del.end())
        it2.second->Crystallize(id_map, data_map, init_map, current_index, current_m_index);
    }
  }

  for(auto it : next_nodes)
  {
    if(del.find(it.second) == del.end())
      it.second->Crystallize(id_map, data_map, init_map, current_index, current_m_index);
  }

  if(terminal != NULL)
    terminal->Crystallize(id_map, data_map, init_map, current_index, current_m_index);

  list<unsigned short> out_list;
  data_init = new signed short[init_map.size()];

  map<unsigned short, unsigned short> temp_to_o;

  for(size_t i = 0;i < data_map.size();i++)
  {
    CrystalData& cd = data_map.find(i)->second;

    CrystalNode t_cn;
    auto m_it = init_map.find(i);
    if(m_it != init_map.end())
      t_cn.data_id = m_it->second;

    if(cd.type == 0)
    {
      data_init[t_cn.data_id] = 0x4000;
      t_cn.type = false;
    }
    else if(cd.type == 1)
    {
      data_init[t_cn.data_id] = 0xbfff;
      t_cn.type = false;
    }
    else if(cd.type == 2)
    {
      data_init[t_cn.data_id] = 0x8000;
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

  map<const Node*, size_t> memory_map;
  for(auto it : id_map)
  {
    auto it2 = init_map.find(it.second);
    if(it2 != init_map.end())
      memory_map[it.first] = it2->second;
  }

  for(auto it : legal_nodes)
    for(auto it2 : it)
      if(del.find(it2.second) == del.end())
        it2.second->CrystalInitialize(*this, memory_map, data_init, top, m_set, goals, initialized);

  //MoveList<AMove> ml = MoveList<AMove>(m_set, roles_ids.size());

  //PrintMoveList(cout, ml);

  for(auto it : next_nodes)
    if(del.find(it.second) == del.end())
      it.second->CrystalInitialize(*this, memory_map, data_init, top, m_set, goals, initialized);

  for(auto it : goal_nodes)
    for(auto it2 : it)
      if(del.find(it2.second) == del.end())
        it2.second->CrystalInitialize(*this, memory_map, data_init, top, m_set, goals, initialized);

  if(terminal != NULL)
    terminal->CrystalInitialize(*this, memory_map, data_init, top, m_set, goals, initialized);

  //PrintState(cout, top);

  data_init_size = init_map.size();

  terminal_crystal_id = init_map.find(id_map.find(GetTerminalNode())->second)->second;

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

  base_mask = AState();

  for(auto it : base_nodes)
    if(del.find(it.second) == del.end())
      base_mask.Set(it.first, true);

  return memory_map;
}

void PropNet::Finalize()
{
  map<const Node*, size_t> id_map;
  map<size_t, CrystalData> data_map;
  map<size_t, size_t> init_map;

  size_t current_index = 0;
  size_t current_m_index = 0;

  for(auto it : input_nodes)
  {
    for(auto it2 : it)
    {
      if(del.find(it2.second) == del.end())
        it2.second->Crystallize(id_map, data_map, init_map, current_index, current_m_index);
    }
  }

  for(auto it : base_nodes)
  {
    if(del.find(it.second) == del.end())
      it.second->Crystallize(id_map, data_map, init_map, current_index, current_m_index);
  }

  for(auto it : legal_nodes)
  {
    for(auto it2 : it)
    {
      if(del.find(it2.second) == del.end())
        it2.second->Crystallize(id_map, data_map, init_map, current_index, current_m_index);
    }
  }

  for(auto it : goal_nodes)
  {
    for(auto it2 : it)
    {
      if(del.find(it2.second) == del.end())
        it2.second->Crystallize(id_map, data_map, init_map, current_index, current_m_index);
    }
  }

  for(auto it : next_nodes)
  {
    if(del.find(it.second) == del.end())
      it.second->Crystallize(id_map, data_map, init_map, current_index, current_m_index);
  }

  if(terminal != NULL)
    terminal->Crystallize(id_map, data_map, init_map, current_index, current_m_index);

  list<unsigned short> out_list;
  default_payload.data = new signed short[init_map.size()];
  data_init_size = init_map.size();

  map<unsigned short, unsigned short> temp_to_o;

  for(size_t i = 0;i < data_map.size();i++)
  {
    CrystalData& cd = data_map.find(i)->second;

    CrystalNode t_cn;
    auto m_it = init_map.find(i);
    if(m_it != init_map.end())
      t_cn.data_id = m_it->second;

    if(cd.type == 0)
    {
      default_payload.data[t_cn.data_id] = 0x8000;
      t_cn.type = false;
    }
    else if(cd.type == 1)
    {
      default_payload.data[t_cn.data_id] = 0x7fff;
      t_cn.type = false;
    }
    else if(cd.type == 2)
    {
      default_payload.data[t_cn.data_id] = 0x0000;
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

  map<const Node*, size_t> memory_map;
  for(auto it : id_map)
  {
    auto it2 = init_map.find(it.second);
    if(it2 != init_map.end())
    {
      memory_map[it.first] = it2->second;
    }
  }

  default_payload2.m_set = new MoveSet[role_size];

  default_payload.top = StateType();
  default_payload.goals = new size_t[role_size];
  default_payload.base = StateType();
  InitState(default_payload.base);

  default_payload2.top = StateType();
  default_payload2.goals = new size_t[role_size];
  default_payload2.base = StateType();
  InitState(default_payload2.base);
  default_payload2.data = new signed short[data_init_size];

  for(auto it : legal_nodes)
    for(auto it2 : it)
      if(del.find(it2.second) == del.end())
        it2.second->CrystalInitialize(*this, memory_map, default_payload.data, default_payload.top, default_payload2.m_set, default_payload.goals, initialized);

  for(auto it : next_nodes)
    if(del.find(it.second) == del.end())
      it.second->CrystalInitialize(*this, memory_map, default_payload.data, default_payload.top, default_payload2.m_set, default_payload.goals, initialized);

  for(auto it : goal_nodes)
    for(auto it2 : it)
      if(del.find(it2.second) == del.end())
        it2.second->CrystalInitialize(*this, memory_map, default_payload.data, default_payload.top, default_payload2.m_set, default_payload.goals, initialized);

  if(terminal != NULL)
    terminal->CrystalInitialize(*this, memory_map, default_payload.data, default_payload.top, default_payload2.m_set, default_payload.goals, initialized);

  default_payload2.top = default_payload.top.Clone();
  for(size_t i = 0;i < data_init_size;i++)
    default_payload2.data[i] = default_payload.data[i];

  if(terminal != NULL)
    terminal_crystal_id = init_map.find(id_map.find(GetTerminalNode())->second)->second;

  base_crystal_ids = new unsigned short[BaseSize()];
  for(size_t i = 0;i < BaseSize();i++)
    base_crystal_ids[i] = 0;
  for(auto it : base_nodes)
  {
    base_crystal_ids[it.first] = temp_to_o.find(id_map.find(it.second)->second)->second;
  }

  input_crystal_ids = new unsigned short*[role_size];
  for(size_t i = 0;i < role_size;i++)
    input_crystal_ids[i] = NULL;
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

  legal_memory_ids = new unsigned short*[role_size];
  for(size_t i = 0;i < role_size;i++)
    legal_memory_ids[i] = NULL;
  default_payload.legal_size = new size_t[role_size];

  for(size_t i = 0;i < role_size;i++)
  {
    legal_memory_ids[i] = new unsigned short[LegalNodes()[i].size()];
    default_payload.legal_size[i] = 0;
    for(size_t j = 0;j < LegalNodes()[i].size();j++)
    {
      legal_memory_ids[i][j] = memory_map.find(LegalNodes()[i].find(j)->second)->second;
      if(default_payload.data[legal_memory_ids[i][j]] & 0x8000)
        default_payload.legal_size[i]++;
    }
  }

  default_payload.t_stack = new int[PayloadStackSize];
  default_payload.base_move = MoveType();

  default_payload2.t_stack = new int[PayloadStackSize];
  default_payload2.base_move = MoveType();

  base_mask = AState();

  for(auto it : BaseNodes())
  {
    if(del.find(it.second) == del.end())
    {
      base_mask.Set(it.first, true);
    }
  }

  default_payload.terminal = false;
  default_payload2.terminal = false;

  default_payload.Crystallize(data_init_size, PayloadStackSize);
}

PropNet::PayLoadType* PropNet::GetPayLoadInstance() const
{
  PayLoadType* out = new PayLoadType();
  out->top = default_payload.top.Clone();
  out->base = default_payload.base.Clone();
  out->base_move = default_payload.base_move.Clone();
  out->legal_size = new size_t[role_size];
  out->goals = new size_t[role_size];
  for(size_t i = 0;i < role_size;i++)
  {
    out->legal_size[i] = default_payload.legal_size[i];
    out->goals[i] = default_payload.goals[i];
  }
  out->t_stack = new int[PayloadStackSize];
  out->data = new signed short[data_init_size];
  for(size_t i = 0;i < data_init_size;i++)
    out->data[i] = default_payload.data[i];
  out->terminal = default_payload.terminal;

  out->Crystallize(data_init_size, PayloadStackSize);

  return out;
}

PropNet::PayLoadType2* PropNet::GetPayLoadInstance2() const
{
  PayLoadType2* out = new PayLoadType2();
  out->top = default_payload2.top.Clone();
  out->base = default_payload2.base.Clone();
  out->base_move = default_payload2.base_move.Clone();
  out->m_set = new MoveSet[role_size];
  out->goals = new size_t[role_size];
  for(size_t i = 0;i < role_size;i++)
  {
    out->m_set[i] = default_payload2.m_set[i];
    out->goals[i] = default_payload2.goals[i];
  }
  out->t_stack = new int[PayloadStackSize];
  out->data = new signed short[data_init_size];
  for(size_t i = 0;i < data_init_size;i++)
    out->data[i] = default_payload2.data[i];
  out->terminal = default_payload2.terminal;

  return out;
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

size_t PropNet::GetNumComponents() const
{
  size_t out = 0;

  for(auto it : base_nodes)
    if(del.find(it.second) == del.end())
      out++;

  for(auto it : input_nodes)
    for(auto it2 : it)
      if(del.find(it2.second) == del.end())
        out++;

  for(auto it : goal_nodes)
    for(auto it2 : it)
      if(del.find(it2.second) == del.end())
        out++;

  for(auto it : legal_nodes)
    for(auto it2 : it)
      if(del.find(it2.second) == del.end())
        out++;

  for(auto it : and_nodes)
    if(del.find(it) == del.end())
      out++;

  for(auto it : or_nodes)
    if(del.find(it) == del.end())
      out++;

  for(auto it : not_nodes)
    if(del.find(it) == del.end())
      out++;

  for(auto it : view_nodes)
    if(del.find(it.second) == del.end())
    {
      cout << it.second->UName() << endl;
      out++;
    }

  cout << out << endl;

  for(auto it : next_nodes)
    if(del.find(it.second) == del.end())
      out++;

  out++;

  return out;
}

size_t PropNet::GetNumOrComponents() const
{
  size_t out = 0;

  for(auto it : or_nodes)
    if(del.find(it) == del.end())
      out++;

  return out;
}

size_t PropNet::GetNumAndComponents() const
{
  size_t out = 0;

  for(auto it : and_nodes)
    if(del.find(it) == del.end())
      out++;

  return out;
}

PropNet* PropNet::OptimizeWithRoleMask(const StateType& mask)
{
  for(size_t i = 0;i < StateType::RawType::arr_size;i++)
  {
    if(mask.get()->s[i])
    {
      for(size_t j = 0;j < 8;j++)
      {
        char t_mask = 1 << j;
        if(mask.get()->s[i] & t_mask)
        {
          Node* n = base_nodes.find(8*i + j)->second;

        }
      }
    }
  }
}
