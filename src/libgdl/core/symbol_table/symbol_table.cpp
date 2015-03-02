/**
 * @file symbol_table.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation of SymbolTable.
 */
#include "symbol_table.hpp"

#include <sstream>

using namespace std;
using namespace libgdl;
using namespace libgdl::core;
using namespace libgdl::gdlparser::parser;

bool SymbolTable::loader = SymbolTable::LoadPrimes();
size_t SymbolTable::primes[10000] = {};

bool SymbolTable::LoadPrimes();
{
  ifstream file("prime_nums.txt");
  if(!file.is_open())
  {
    cerr << "Prime number file not available!!" << endl;
    exit(1);
  }

  size_t index = 0;

  string line;
  while(getline(file, line))
  {
    std::stringstream stream;
    stream << line;

    for(size_t i = 0;i < 10;i++)
    {
      size_t temp;
      stream >> temp;
      primes[index++] = temp;
    }
  }

  file.close();
  return true;
}


RawSymbolTable::RawSymbolTable(const Log& log)
  : count(0u),
  log(log)

{
  index = 0;

  AddEntry("not", Location(), 0, true); // 0
  AddEntry("or", Location(), 0, true); // 1
  AddEntry("distinct", Location(), 2, true); // 2

  AddEntry("role", Location(), 1, true); // 3
  AddEntry("base", Location(), 1, true); // 4
  AddEntry("input", Location(), 2, true); // 5
  AddEntry("init", Location(), 1, true); // 6
  AddEntry("legal", Location(), 2, true); // 7
  AddEntry("next", Location(), 1, true); // 8
  AddEntry("does", Location(), 2, true); // 9
  AddEntry("true", Location(), 1, true); // 10
  AddEntry("goal", Location(), 2, true); // 11
  AddEntry("terminal", Location(), 0, true); // 12
}

RawSymbolTable::~RawSymbolTable()
{
  for(SymbolMap::iterator it = symbol_table.begin();it != symbol_table.end();it++)
    delete it->second;
}

std::string RawSymbolTable::GetMangledName(const std::string& name,
                                           size_t arity,
                                           bool isRelation)
{
  std::string uname = "_";
  if(isRelation)
    uname += "r";
  else uname += "f";

  uname += "_" + name + "_" + ToString(arity);
  return uname;
}

std::string RawSymbolTable::UnmangleName(const std::string& uname,
                                         size_t* arity,
                                         bool* isRelation)
{
  std::string name;

  size_t index = uname.find("_", 1);
  name = uname.substr(1, index - 1);

  if(uname[index + 1] == 'r' && isRelation)
    *isRelation = true;
  else if(isRelation)
    *isRelation = false;

  if(arity)
  {
    stringstream stream;
    stream << uname.substr(index + 3);
    stream >> *arity;
  }

  return name;
}

size_t RawSymbolTable::AddEntry(const std::string& name,
                                const Location& loc,
                                size_t arity,
                                bool isRelation)
{
  if(name == "not" || name == "or")
    arity = 0;

  std::string uname = GetMangledName(name, arity, isRelation);
  Symbol* sym = new Symbol(uname, name, loc);

  id_table[uname] = index;
  symbol_table[index] = sym;
  index++;
  return index - 1;
}

size_t RawSymbolTable::CheckEntry(const std::string& name,
                                  size_t arity,
                                  bool isRelation,
                                  Symbol*& symbol)
{
  if(name == "not" || name == "or")
    arity = 0;

  IDMap::const_iterator it = id_table.find(GetMangledName(name, arity, isRelation));
  if(it == id_table.end())
  {
    symbol = NULL;
    return 0;
  }
  symbol = symbol_table[it->second];
  return it->second;
}

bool RawSymbolTable::AddDefined(size_t id, const Location& loc)
{
  if(id != NotID && id != OrID && id != DistinctID && id != RoleID &&
     id != BaseID && id != InputID && id != InitID && id != LegalID &&
     id != NextID && id != DoesID && id != TrueID && id != GoalID &&
     id != TerminalID)
  {
    SymbolMap::iterator it = symbol_table.find(id);
    if(it == symbol_table.end()) return false;

    it->second->AddDef(loc);
  }
  return true;
}

bool RawSymbolTable::AddUsed(size_t id, const Location& loc)
{
  if(id != NotID && id != OrID && id != DistinctID && id != RoleID &&
     id != BaseID && id != InputID && id != InitID && id != LegalID &&
     id != NextID && id != DoesID && id != TrueID && id != GoalID &&
     id != TerminalID)
  {
    SymbolMap::iterator it = symbol_table.find(id);
    if(it == symbol_table.end()) return false;

    it->second->AddUsed(loc);
  }
  return true;
}

std::string RawSymbolTable::GetCommandName(size_t id) const
{
  const SymbolMap::const_iterator it = symbol_table.find(id);
  if(it != symbol_table.end()) return it->second->Name();
  else
  {
    log.Warn << "Identifier " << ToString(id) << " does not exist" << std::endl;
    return "";
  }
}

void RawSymbolTable::GenerateWarnings(list<ErrorType>& out) const
{
  for(auto it : symbol_table)
  {
    Symbol* sym = it.second;

    const std::string& uname = sym->UName();
    if(uname.find("_r_") != string::npos)
    {
      if(sym->IsDef() && !sym->IsUsed())
      {
        ErrorType warn;
        warn.AddEntry("Relation '" + sym->Name() +
                      "' defined but not used.", sym->loc_def);
        out.push_back(warn);
      }
      else if(sym->IsUsed() && !sym->IsDef())
      {
        ErrorType warn;
        warn.AddEntry("Relation '" + sym->Name() +
                      "' used but not defined.", sym->loc_used);
        out.push_back(warn);
      }
    }
  }
}
