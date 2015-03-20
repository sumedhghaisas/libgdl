/**
 * @file Argument.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation of Argument data type.
 */
#include "argument.hpp"

#include <stack>
#include <sstream>
#include <tuple>
#include <boost/algorithm/string.hpp>

#include <libgdl/core/data_types/error_type.hpp>

using namespace std;
using namespace boost;
using namespace libgdl;
using namespace libgdl::core;

Argument::Argument(const Argument& arg) noexcept
  : val("")
{
  if(arg.IsVariable())
  {
    t = arg.t;
    val = arg.val;
    return;
  }

  // map to hold variable name versus assigned location mapping
  // this is important as all the occurrences of a variable in a clause
  // are assigned same object
  StrVarMap v_map;

  t = arg.t;
  value = arg.value;

  // call recursively on arguments
  for(size_t i = 0;i < arg.args.size();i++)
    args.push_back(ConstructArgument(*arg.args[i], v_map));
}

Argument::Argument(const std::string& str,
                   SymbolTable symbol_table,
                   bool isRel,
                   Log log)
{
  StrVarMap v_map;

  if(str[0] != '(')
  {
    if(str[0] == '?')
    {
      t = Argument::Var;
      val = str;
      return;
    }

    Symbol* sym;
    size_t id = symbol_table.CheckEntry(str, 0, isRel, sym);

    if(sym == NULL) id = symbol_table.AddEntry(str, Location(), 0, isRel);

    if(isRel)
      t = Argument::Relation;
    else t = Argument::Function;
    value = id;
    return;
  }

  std::string cmd;
  StringVec args;
  if(!SeparateCommand(str, cmd, args, log))
  {
    log.Fatal << "Unable to construct argument from " << str << std::endl;
    return;
  }

  Symbol* sym;
  size_t id = symbol_table.CheckEntry(cmd, args.size(), isRel, sym);

  if(sym == NULL)
    id = symbol_table.AddEntry(cmd, Location(), args.size(), isRel);

  if(isRel)
    t = Argument::Relation;
  else t = Argument::Function;
  value = id;

  for(size_t i = 0;i < args.size();i++)
  {
    Argument *arg;
    if(id == SymbolTable::NotID || id == SymbolTable::OrID)
      arg = ConstructArgument(args[i], v_map, symbol_table, true, log);
    else
      arg = ConstructArgument(args[i], v_map, symbol_table, false, log);

    if(arg != NULL) this->args.push_back(arg);
  }
}

Argument::~Argument()
{
  std::set<Argument*> v_set;
  // destroy arguments and clear arguments vector before deleting the object
  Destroy(v_set);
}

bool Argument::HasVariables() const
{
  stack<const Argument*> S;
  S.push(this);

  while(!S.empty())
  {
    const Argument* t = S.top();
    S.pop();

    if(t->IsVariable())
      return true;
    else for(size_t i = 0;i < t->args.size();i++)
      S.push(t->args[i]);
  }
  return false;
}

bool Argument::SeparateCommand(const std::string& input,
                               std::string& cmd,
                               std::vector<std::string>& args,
                               Log log)
{
  if (input.size() < 3 || input[0] != '(' || input[input.length()-1] != ')')
  {
    log.Fatal << "Input "
              << input
              << " is not surrounded by braces or not big enough" << std::endl;
    return false;
  }
  std::string withoutBraces = input.substr (1, input.length() - 2);
  boost::algorithm::trim(withoutBraces);
  size_t sep = withoutBraces.find(' ');
  if (sep == withoutBraces.npos)
  {
    // no arguments in string
    cmd = withoutBraces;
    return true;
  }
  cmd = withoutBraces.substr (0, sep);

  sep++;
  int depth = 0;
  // parsing parameters
  std::ostringstream* o = new std::ostringstream();
  while (sep < withoutBraces.length())
  {
    char c = withoutBraces[sep];
    if (c == '(')
    {
      depth++;
      *o << c;
    }
    else if (c == ')')
    {
      depth--;
      *o << c;
    }
    else if ((c == ' ') && (depth == 0))
    {
      args.push_back (o->str());
      delete o;
      o = new std::ostringstream();
    }
    else *o << c;
    sep++;
  }
  if (depth != 0)
  {
    log.Fatal << "Braces count mismatch in " << withoutBraces << std::endl;
    delete o;
    return false;
  }
  if (o->str() != "") args.push_back (o->str());
  delete o;
  return true;
}

void Argument::Destroy(std::set<Argument*>& v_set)
{
  // if variable add it to deleted variable set
  if(t == Argument::Var && v_set.find(this) == v_set.end())
  {
    v_set.insert(this);
    return;
  }
  else if(t != Argument::Var)
  {
    for(size_t i = 0;i < args.size();i++)
    {
      // only delete argument which have not been deleted yet
      if(v_set.find(args[i]) == v_set.end())
      {
        args[i]->Destroy(v_set);
        delete args[i];
      }
    }
    args.clear();
  }
}

Argument* Argument::ConstructArgument(const Argument& arg,
                                      StrVarMap& v_map)
{
  StrVarMap::iterator it;

  if(arg.IsVariable() && (it = v_map.find(arg.val)) != v_map.end())
    return it->second;
  else if(arg.IsVariable())
  {
    Argument* out = new Argument();
    out->t = Argument::Var;
    out->val = arg.val;
    v_map[out->val] = out;
    return out;
  }

  Argument* out = new Argument();
  out->t = arg.t;
  out->value = arg.value;

  for(size_t i = 0;i < arg.args.size();i++)
    out->args.push_back(ConstructArgument(*arg.args[i], v_map));
  return out;
}

Argument* Argument::ConstructArgument(const std::string& str,
                                      StrVarMap& v_map,
                                      SymbolTable& symbol_table,
                                      bool isRel,
                                      Log log)
{
  if(str[0] != '(')
  {
    if(str[0] == '?')
    {
      StrVarMap::iterator it;
      if((it = v_map.find(str)) != v_map.end())
        return it->second;
      else
      {
        Argument* out = new Argument();
        out->t = Argument::Var;
        out->val = str;
        v_map[str] = out;
        return out;
      }
    }

    Symbol* sym;
    size_t id = symbol_table.CheckEntry(str, 0, isRel, sym);

    if(sym == NULL) id = symbol_table.AddEntry(str, Location(), 0, isRel);

    Argument* out = new Argument();
    if(isRel)
      out->t = Argument::Relation;
    else out->t = Argument::Function;
    out->value = id;
    return out;
  }

  std::string cmd;
  StringVec args;
  if(!SeparateCommand(str, cmd, args, log))
  {
    log.Fatal << "Unable to construct argument from " << str << std::endl;
    return NULL;
  }

  Symbol* sym;
  size_t id = symbol_table.CheckEntry(cmd, args.size(), isRel, sym);

  if(sym == NULL)
    id = symbol_table.AddEntry(cmd, Location(), args.size(), isRel);

  Argument* out = new Argument();

  if(isRel)
    out->t = Argument::Relation;
  else out->t = Argument::Function;
  out->value = id;

  for(size_t i = 0;i < args.size();i++)
  {
    Argument* arg;
    if(id == SymbolTable::NotID || id == SymbolTable::OrID)
      arg = ConstructArgument(args[i], v_map, symbol_table, true, log);
    else
      arg = ConstructArgument(args[i], v_map, symbol_table, false, log);
    if(arg != NULL) out->args.push_back(arg);
  }
  return out;
}

bool Argument::operator==(const Argument& arg) const
{
  if(t != arg.t) return false;
  if(t == Var)
  {
    if(val != arg.val) return false;
    else return true;
  }

  if(value != arg.value) return false;
  if(args.size() != arg.args.size()) return false;

  for(size_t i = 0;i < args.size();i++)
    if(*args[i] != *arg.args[i]) return false;

  return true;
}

Argument& Argument::operator=(const Argument& arg)
{
  std::set<Argument*> v_set;
  // destroy arguments and clear arguments vector before deleting the object
  Destroy(v_set);

  if(arg.IsVariable())
  {
    t = arg.t;
    val = arg.val;
    return *this;
  }

  // map to hold variable name versus assigned location mapping
  // this is important as all the occurrences of a variable in a clause
  // are assigned same object
  std::map<std::string, Argument*> v_map;

  t = arg.t;
  value = arg.value;

  // call recursively on arguments
  for(size_t i = 0;i < arg.args.size();i++)
    args.push_back(ConstructArgument(*arg.args[i], v_map));

  return *this;
}

bool Argument::OrEquate(const Argument& arg) const
{
  // for 'or', if arg matches any argument return true
  if(val == "or")
  {
    for(size_t i = 0;i < args.size();i++)
      if(args[i]->OrEquate(arg)) return true;
    return false;
  }

  return (*this == arg);
}

bool Argument::HasAsArgument(const Argument& arg) const
{
  if(val == "not") return false;
  else if(val == "or")
  {
    bool has = true;
    for(size_t i = 0;i < args.size();i++)
      if(!args[i]->HasAsArgument(arg))
      {
        has = false;
        break;
      }
    return has;
  }

  for(size_t i = 0;i < args.size();i++)
    if(*(args[i]) == arg) return true;

  return false;
}

bool Argument::IsGround() const
{
  std::stack<const Argument*> S;
  S.push(this);

  while(!S.empty())
  {
    const Argument* temp = S.top();
    S.pop();

    if(temp->IsVariable()) return false;
    else for(size_t i = 0;i < temp->args.size();i++)
      S.push(temp->args[i]);
  }
  return true;
}

set<const Argument*> Argument::GetVariables() const
{
  set<const Argument*> variables;

  stack<const Argument*> S;
  S.push(this);

  while(!S.empty())
  {
    const Argument* arg = S.top();
    S.pop();

    if(arg->IsVariable())
    {
      variables.insert(arg);
      continue;
    }

    for(size_t i = 0;i < arg->args.size();i++)
    {
      S.push(arg->args[i]);
    }
  }
  return variables;
}

Argument* Argument::CopyWithMapping(const Argument* arg,
                                    const VariableMap& o_v_map,
                                    VariableMap& v_map)
{
  Argument* out = new Argument();
  out->t = arg->t;
  out->value = arg->value;
  out->val = arg->val;

  if(arg->t == Argument::Var)
  {
    auto it = o_v_map.find(arg);
    if(it == o_v_map.end())
      v_map[out] = arg;
    else
    {
      delete out;
      out = CopyWithMapping(it->second, o_v_map, v_map);
    }

    return out;
  }

  for(auto it : arg->args)
  {
    out->args.push_back(CopyWithMapping(it, o_v_map, v_map));
  }

  return out;
}

VariableMap Argument::ConvertMapToArg(const Argument* arg,
                                      const Argument* con_to,
                                      const VariableMap& v_map)
{
  VariableMap out;

  stack<pair<const Argument*, const Argument*>> s;
  s.emplace(arg, con_to);

  while(!s.empty())
  {
    auto& tup = s.top();

    const Argument* arg1 = get<0>(tup);
    const Argument* arg2 = get<1>(tup);

    s.pop();

    if(arg1->t == Argument::Var)
    {
      out[arg2] = v_map.find(arg1)->second;
      continue;
    }

    for(size_t i = 0;i < arg->args.size();i++)
    {
      s.emplace(arg2->args[i], arg2->args[i]);
    }
  }

  return out;
}
