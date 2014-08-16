/**
 * @file clause.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation of Clause data type.
 */
#include "clause.hpp"

#include <stack>
#include <sstream>

using namespace libgdl;

Clause::Clause(const TokenValue& tok, const size_t id)
  : id(id), isLocation(false)
{
  const std::vector<TokenValue>& args = tok.Arguments();

  std::map<std::string, Argument*> v_map;

  head = Argument::ConstructArgument(args[0], v_map);

  for(size_t i = 1;i < args.size();i++)
    premisses.push_back(Argument::ConstructArgument(args[i], v_map));
}

Clause::Clause(const std::string& str) : isLocation(false)
{
  std::map<std::string, Argument*> v_map;

  std::string cmd;
  std::vector<std::string> args;
  if(!Argument::SeparateCommand(str, cmd, args) || cmd != "<=" || args.size() < 2)
  {
    std::cerr << "Unable to construct clause from " << str << std::endl;
    return;
  }

  head = Argument::ConstructArgument(args[0], v_map);

  for(size_t i = 1;i < args.size();i++)
    premisses.push_back(Argument::ConstructArgument(args[i], v_map));
}

Clause::Clause(const Clause& c)
{
  std::map<std::string, Argument*> v_map;

  id = c.id;

  isLocation = c.isLocation;
  loc = c.loc;

  head = Argument::ConstructArgument(*c.head, v_map);

  for(size_t i = 0;i < c.premisses.size();i++)
    premisses.push_back(Argument::ConstructArgument(*c.premisses[i], v_map));
}

Clause& Clause::operator=(const Clause& c)
{
  premisses.clear();

  std::map<std::string, Argument*> v_map;

  id = c.id;

  isLocation = c.isLocation;
  loc = c.loc;

  head = Argument::ConstructArgument(*c.head, v_map);

  for(size_t i = 0;i < c.premisses.size();i++)
    premisses.push_back(Argument::ConstructArgument(*c.premisses[i], v_map));

  return *this;
}

Clause::~Clause()
{
  // to hold the set of variables already deleted
  std::set<Argument*> v_set;

  // delete head if it is not NULL
  if(head != NULL)
  {
    head->Destroy(v_set);
    delete head;
  }

  for(size_t i = 0;i < premisses.size();i++)
  {
    premisses[i]->Destroy(v_set);
    delete premisses[i];
  }
}

bool Clause::IsGround()
{
  if(!(head->IsGround())) return false;

  for(size_t i = 0;i < premisses.size();i++)
    if(!(premisses[i]->IsGround())) return false;

  return true;
}