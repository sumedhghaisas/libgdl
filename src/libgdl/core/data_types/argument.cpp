/**
 * @file Argument.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation of Argument data type.
 */
#include "argument.hpp"

#include <stack>
#include <sstream>

#include <libgdl/core/data_types/error_type.hpp>

using namespace std;
using namespace boost;
using namespace libgdl;

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
                   SymbolTable& symbol_table,
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
    size_t id = symbol_table.CheckEntry(str, sym);
    if(sym != NULL)
    {
      if(sym->Arity() != 0)
      {
        ARITY_ERROR(error,
                    str, 0,
                    sym->Arity(),
                    Location(),
                    sym->GetLocation());
        log.Fatal << error << endl;
        return;
      }
      else if(isRel && sym->SymbolType() != Symbol::RELATION)
      {
        RF_ERROR(error,
                str,
                "Relation", "Function",
                Location(),
                sym->GetLocation());
        log.Fatal << error << endl;
        return;
      }
      else if(!isRel && sym->SymbolType() != Symbol::FUNCTION)
      {
        RF_ERROR(error,
                str,
                "Function", "Relation",
                Location(),
                sym->GetLocation());
        log.Fatal << error << endl;
        return;
      }
    }
    else id = symbol_table.AddEntry(str, Location(), 0, true);

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
  size_t id = symbol_table.CheckEntry(cmd, sym);
  if(sym != NULL)
  {
    if(sym->Arity() != args.size())
    {
      ARITY_ERROR(error,
                  cmd, args.size(),
                  sym->Arity(),
                  Location(),
                  sym->GetLocation());
      log.Fatal << error << endl;
      return;
    }
    else if(isRel && sym->SymbolType() != Symbol::RELATION)
    {
      RF_ERROR(error,
              cmd,
              "Relation", "Function",
              Location(),
              sym->GetLocation());
      log.Fatal << error << endl;
      return;
    }
    else if(!isRel && sym->SymbolType() != Symbol::FUNCTION)
    {
      RF_ERROR(error,
              cmd,
              "Function", "Relation",
              Location(),
              sym->GetLocation());
      log.Fatal << error << endl;
      return;
    }
  }
  else id = symbol_table.AddEntry(cmd, Location(), args.size(), isRel);

  if(isRel)
    t = Argument::Relation;
  else t = Argument::Function;
  value = id;

  for(size_t i = 0;i < args.size();i++)
  {
    Argument* arg = ConstructArgument(args[i], v_map, symbol_table, false, log);
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
  const std::string withoutBraces = input.substr (1, input.length() - 2);
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
    size_t id = symbol_table.CheckEntry(str, sym);
    if(sym != NULL)
    {
      if(sym->Arity() != 0)
      {
        ARITY_ERROR(error,
                    str, 0,
                    sym->Arity(),
                    Location(),
                    sym->GetLocation());
        log.Fatal << error << endl;
        return NULL;
      }
      else if(isRel && sym->SymbolType() != Symbol::RELATION)
      {
        RF_ERROR(error,
                str,
                "Relation", "Function",
                Location(),
                sym->GetLocation());
        log.Fatal << error << endl;
        return NULL;
      }
      else if(!isRel && sym->SymbolType() != Symbol::FUNCTION)
      {
        RF_ERROR(error,
                 str,
                 "Function", "Relation",
                 Location(),
                 sym->GetLocation());
        log.Fatal << error << endl;
        return NULL;
      }
    }
    else id = symbol_table.AddEntry(str, Location(), 0, isRel);

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
  size_t id = symbol_table.CheckEntry(cmd, sym);
  if(sym != NULL)
  {
    if(sym->Arity() != args.size())
    {
      ARITY_ERROR(error,
                  cmd, args.size(),
                  sym->Arity(),
                  Location(),
                  sym->GetLocation());
      log.Fatal << error << endl;
      return NULL;
    }
    else if(isRel && sym->SymbolType() != Symbol::RELATION)
    {
      RF_ERROR(error,
               cmd,
               "Relation", "Function",
               Location(),
               sym->GetLocation());
      log.Fatal << error << endl;
      return NULL;
    }
    else if(!isRel && sym->SymbolType() != Symbol::FUNCTION)
    {
      RF_ERROR(error,
               cmd,
               "Function", "Relation",
               Location(),
               sym->GetLocation());
      log.Fatal << error << endl;
      return NULL;
    }
  }
  else id = symbol_table.AddEntry(cmd, Location(), args.size(), isRel);

  Argument* out = new Argument();

  if(isRel)
    out->t = Argument::Relation;
  else out->t = Argument::Function;
  out->value = id;

  for(size_t i = 0;i < args.size();i++)
  {
    Argument* arg = ConstructArgument(args[i], v_map, symbol_table, false, log);
    if(arg != NULL) out->args.push_back(arg);
  }
  return out;
}

size_t Argument::Hash(const unordered_map<string, size_t>& id_map)
{
  size_t out = id_map.find(val)->second;
  size_t total = 0;
  for(size_t i = 0;i < args.size();i++)
  {
    total += (i + 1) * args[i]->Hash(id_map);
  }
  if(total != 0) return total * out;
  else return out;
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

std::string Argument::DecodeToString(const SymbolTable& symbol_table) const
{
  string out = "";
  if(args.size() == 0 && t != Argument::Var)
  {
    return symbol_table.GetCommandName(value);
  }
  else if(args.size() == 0)
  {
    return val;
  }
  else out = "( " + symbol_table.GetCommandName(value);

  for(size_t i = 0;i < args.size();i++)
    out += " " + (args[i])->DecodeToString(symbol_table);
  out += " )";
  return out;
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
