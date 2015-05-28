#include "propnet.hpp"

#include <sstream>
#include <fstream>
#include <tuple>
#include <regex>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/unordered_map.hpp>

#include <libgdl/core/data_types/move_list.hpp>

#include "compilation/entry_manager.hpp"
#include "compilation/handlers/code_handler.hpp"
#include "compilation/handlers/file_handler.hpp"

#include "crystallization/crystal_data.hpp"

using namespace std;
using namespace libgdl;
using namespace libgdl::core;
using namespace libgdl::gdlreasoner;
using namespace libgdl::gdlparser;
using namespace libgdl::propnet;
using namespace libgdl::propnet::node_types;
using namespace libgdl::propnet::crystallization;
using namespace libgdl::propnet::compilation;
using namespace libgdl::propnet::compilation::handlers;

size_t PropNet::debug_time = 0;

PropNet::PropNet(const PropNet& pn)
{
  map<const Node*, Node*> copy_map;

  InitializeToRoles(pn.RoleSize());

  roles_ids = pn.roles_ids;

  for(auto it : pn.view_nodes)
  {
    if(pn.del.find(it.second) == pn.del.end())
      it.second->CreateCopy(*this, NULL, copy_map);
  }

  for(auto it : pn.next_nodes)
  {
    if(pn.del.find(it.second) == pn.del.end())
      it.second->CreateCopy(*this, NULL, copy_map);
  }

  for(auto it : pn.input_nodes)
  {
    for(auto it2 : it)
    {
      if(pn.del.find(it2.second) == pn.del.end())
        it2.second->CreateCopy(*this, NULL, copy_map);
    }
  }

  for(auto it : pn.legal_nodes)
  {
    for(auto it2 : it)
    {
      if(pn.del.find(it2.second) == pn.del.end())
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
    if(pn.del.find(terminal) == pn.del.end())
      pn.terminal->CreateCopy(*this, NULL, copy_map);
  }

  for(auto it : pn.base_nodes)
  {
    if(pn.del.find(it.second) == pn.del.end())
      it.second->CreateCopy(*this, NULL, copy_map);
  }

  init_props = pn.init_props;

  base_size = pn.base_size;
  role_size = pn.role_size;
  base_mask = pn.base_mask;

  c_r_id = pn.c_r_id;
  c_and_id = pn.c_and_id;
  c_not_id = pn.c_not_id;
  c_not_id = pn.c_not_id;
  c_or_id = pn.c_or_id;
  c_view_id = pn.c_view_id;
  c_base_id = pn.c_base_id;
  c_input_id = pn.c_input_id;
  sym = pn.sym;
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

void PropNet::Initialize(const PropNet& pn)
{
  map<const Node*, Node*> copy_map;

  InitializeToRoles(pn.RoleSize());

  roles_ids = pn.roles_ids;

  for(auto it : pn.view_nodes)
  {
    if(pn.del.find(it.second) == pn.del.end())
      it.second->CreateCopy(*this, NULL, copy_map);
  }

  for(auto it : pn.next_nodes)
  {
    if(pn.del.find(it.second) == pn.del.end())
      it.second->CreateCopy(*this, NULL, copy_map);
  }

  for(auto it : pn.input_nodes)
  {
    for(auto it2 : it)
    {
      if(pn.del.find(it2.second) == pn.del.end())
        it2.second->CreateCopy(*this, NULL, copy_map);
    }
  }

  for(auto it : pn.legal_nodes)
  {
    for(auto it2 : it)
    {
      if(pn.del.find(it2.second) == pn.del.end())
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
    if(pn.del.find(terminal) == pn.del.end())
      pn.terminal->CreateCopy(*this, NULL, copy_map);
  }

  for(auto it : pn.base_nodes)
  {
    if(pn.del.find(it.second) == pn.del.end())
      it.second->CreateCopy(*this, NULL, copy_map);
  }

  init_props = pn.init_props;

  base_size = pn.base_size;
  role_size = pn.role_size;
  base_mask = pn.base_mask;

  c_r_id = pn.c_r_id;
  c_and_id = pn.c_and_id;
  c_not_id = pn.c_not_id;
  c_not_id = pn.c_not_id;
  c_or_id = pn.c_or_id;
  c_view_id = pn.c_view_id;
  c_base_id = pn.c_base_id;
  c_input_id = pn.c_input_id;
  sym = pn.sym;
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

void PropNet::Crystallize(map<const Node*, CrystalData>& data_map,
                          list<const Node*>& crystal_node_order)
{
  size_t current_index = 0;

  for(auto it : input_nodes)
  {
    for(auto it2 : it)
    {
      if(del.find(it2.second) == del.end())
        it2.second->Crystallize(data_map, crystal_node_order, current_index);
    }
  }

  for(auto it : base_nodes)
  {
    if(del.find(it.second) == del.end())
      it.second->Crystallize(data_map, crystal_node_order, current_index);
  }

  for(auto it : legal_nodes)
  {
    for(auto it2 : it)
    {
      if(del.find(it2.second) == del.end())
        it2.second->Crystallize(data_map, crystal_node_order, current_index);
    }
  }

  for(auto it : goal_nodes)
  {
    for(auto it2 : it)
    {
      if(del.find(it2.second) == del.end())
        it2.second->Crystallize(data_map, crystal_node_order, current_index);
    }
  }

  for(auto it : next_nodes)
  {
    if(del.find(it.second) == del.end())
      it.second->Crystallize(data_map, crystal_node_order, current_index);
  }

  if(terminal != NULL)
    terminal->Crystallize(data_map, crystal_node_order, current_index);
}

void PropNet::FillCrystal(map<const Node*, CrystalData>& crystal_data_map,
                          list<const Node*>& crystal_node_order,
                          map<unsigned short, unsigned short>& id_to_mem)
{
  list<unsigned short> crystal_as_list;

  for(auto n_it : crystal_node_order)
  {
    const CrystalData& cd = crystal_data_map.find(n_it)->second;

    CrystalNode t_cn;
    t_cn.data_id = cd.id;

    if(cd.type == CrystalConfig::Type::AND ||
       cd.type == CrystalConfig::Type::OR ||
       cd.type == CrystalConfig::Type::NOT ||
       cd.type == CrystalConfig::Type::NOR ||
       cd.type == CrystalConfig::Type::NAND)
    {
      default_payload.data[t_cn.data_id] = CrystalConfig::GetTypeInit(cd.type);
      t_cn.type = false;
    }
    else if(cd.type == CrystalConfig::Type::OR_UPDATE || cd.type == CrystalConfig::Type::AND_UPDATE)
    {
      default_payload.data[t_cn.data_id] = CrystalConfig::GetTypeInit(cd.type);
      t_cn.type = true;
      t_cn.out_size = 4;

      id_to_mem[cd.id] = crystal_as_list.size();
      unsigned short* temp = (unsigned short*)&t_cn;
      crystal_as_list.push_back(temp[0]);
      crystal_as_list.push_back(temp[1]);

      size_t t = (size_t)cd.node;
      temp = (unsigned short*)&t;
      crystal_as_list.push_back(temp[0]);
      crystal_as_list.push_back(temp[1]);
      crystal_as_list.push_back(temp[2]);
      crystal_as_list.push_back(temp[3]);
      continue;
    }

    id_to_mem[cd.id] = crystal_as_list.size();
    t_cn.out_size = cd.out_degree.size();

    unsigned short* temp = (unsigned short*)&t_cn;
    crystal_as_list.push_back(temp[0]);
    crystal_as_list.push_back(temp[1]);

    for(auto it : cd.out_degree)
    {
      if(it > 65536)
      {
        log.Fatal << LOGID << "Out of bound while crystallizing." << endl;
        exit(1);
      }
      crystal_as_list.push_back((unsigned short)it);
    }
  }

  arr_propnet = new unsigned short[crystal_as_list.size()];

  size_t c_index = 0;
  auto it = crystal_as_list.begin();
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
        arr_propnet[c_index] = id_to_mem.find(*it)->second;
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

    if(it == crystal_as_list.end())
      break;
  }
}

void PropNet::Finalize()
{
  map<const Node*, CrystalData> crystal_data_map;
  list<const Node*> crystal_node_order;

  Crystallize(crystal_data_map, crystal_node_order);

  crystal_data_init_size = crystal_data_map.size();

  default_payload.Initialize(crystal_data_init_size, role_size, PayloadStackSize);

  map<unsigned short, unsigned short> id_to_mem;

  FillCrystal(crystal_data_map, crystal_node_order, id_to_mem);

  set<const Node*> initialized;

  InitState(default_payload.base);

  for(auto it : legal_nodes)
    for(auto it2 : it)
      if(del.find(it2.second) == del.end())
        it2.second->CrystalInitialize(*this, crystal_data_map, default_payload, initialized);

  for(auto it : next_nodes)
    if(del.find(it.second) == del.end())
      it.second->CrystalInitialize(*this, crystal_data_map, default_payload, initialized);

  for(auto it : goal_nodes)
    for(auto it2 : it)
      if(del.find(it2.second) == del.end())
        it2.second->CrystalInitialize(*this, crystal_data_map, default_payload, initialized);

  if(terminal != NULL)
    terminal->CrystalInitialize(*this, crystal_data_map, default_payload, initialized);

  if(terminal != NULL)
    terminal_crystal_id = crystal_data_map.find(GetTerminalNode())->second.id;


  base_crystal_ids = new unsigned short[BaseSize()];
  for(size_t i = 0;i < BaseSize();i++)
    base_crystal_ids[i] = 0;
  for(auto it : base_nodes)
  {
    base_crystal_ids[it.first] = id_to_mem.find(crystal_data_map.find(it.second)->second.id)->second;
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
      input_crystal_ids[index][i] = id_to_mem.find(crystal_data_map.find(input_nodes[index].find(i)->second)->second.id)->second;
    }
    index++;
  }

  legal_memory_ids = new unsigned short*[role_size];
  for(size_t i = 0;i < role_size;i++)
    legal_memory_ids[i] = NULL;

  for(size_t i = 0;i < role_size;i++)
  {
    legal_memory_ids[i] = new unsigned short[LegalNodes()[i].size()];
    for(size_t j = 0;j < LegalNodes()[i].size();j++)
    {
      legal_memory_ids[i][j] = crystal_data_map.find(LegalNodes()[i].find(j)->second)->second.id;
      //if(default_payload.data[legal_memory_ids[i][j]] & CrystalConfig::CrystalSignMask)
        //default_payload.legal_size[i]++;
    }
  }

  base_mask = AState();

  for(auto it : BaseNodes())
  {
    if(del.find(it.second) == del.end())
    {
      base_mask.Set(it.first, true);
    }
  }

  default_payload.Crystallize(crystal_data_init_size, PayloadStackSize);
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
  out->data = new signed short[crystal_data_init_size];
  for(size_t i = 0;i < crystal_data_init_size;i++)
    out->data[i] = default_payload.data[i];
  out->terminal = default_payload.terminal;

  out->Crystallize(crystal_data_init_size, PayloadStackSize);

  return out;
}

PropNet::PayLoadType2* PropNet::GetPayLoadInstance2() const
{
  PayLoadType2* out = new PayLoadType2();
//  out->top = default_payload2.top.Clone();
//  out->base = default_payload2.base.Clone();
//  out->base_move = default_payload2.base_move.Clone();
//  out->m_set = new MoveSet[role_size];
//  out->goals = new size_t[role_size];
//  for(size_t i = 0;i < role_size;i++)
//  {
//    out->m_set[i] = default_payload2.m_set[i];
//    out->goals[i] = default_payload2.goals[i];
//  }
//  out->t_stack = new int[PayloadStackSize];
//  out->data = new signed short[crystal_data_init_size];
//  for(size_t i = 0;i < crystal_data_init_size;i++)
//    out->data[i] = default_payload2.data[i];
//  out->terminal = default_payload2.terminal;

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
      out++;
    }

  for(auto it : next_nodes)
    if(del.find(it.second) == del.end())
      out++;

  out++;

  return out;
}

size_t PropNet::GetNumNotComponenets() const
{
  size_t out = 0;

  for(auto it : not_nodes)
    if(del.find(it) == del.end())
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
//  for(size_t i = 0;i < StateType::RawType::arr_size;i++)
//  {
//    if(mask.get()->s[i])
//    {
//      for(size_t j = 0;j < 8;j++)
//      {
//        char t_mask = 1 << j;
//        if(mask.get()->s[i] & t_mask)
//        {
//          Node* n = base_nodes.find(8*i + j)->second;
//
//        }
//      }
//    }
//  }
}

void PropNet::OptimizeWithNodeMerge()
{
  cout << GetNumComponents() << endl;

  for(auto it : legal_nodes)
  {
    for(auto it2 : it)
    {
      it2.second->MergeWithChild(*this);
    }
  }

  for(auto it : next_nodes)
  {
    it.second->MergeWithChild(*this);
  }

  for(auto it : goal_nodes)
    for(auto it2 : it)
      if(del.find(it2.second) == del.end())
        it2.second->MergeWithChild(*this);

  terminal->MergeWithChild(*this);

  stack<Node*> n_stack;

  for(auto it : legal_nodes)
  {
    for(auto it2 : it)
    {
      n_stack.push(it2.second);
    }
  }

  for(auto it : next_nodes)
  {
    n_stack.push(it.second);
  }

  while(!n_stack.empty())
  {
    Node* n = n_stack.top();
    n_stack.pop();

    Node* temp = n->MergeWithChild(*this);
    if(temp != NULL)
      n = temp;

    for(auto it : n->in_degree)
      n_stack.push(it);
  }

  cout << GetNumComponents() << endl;
}
