/**
 * @file b_state.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation of BState.
 */
#include "b_state.hpp"

#include <sstream>

using namespace std;
using namespace boost;
using namespace libgdl;
using namespace libgdl::core;

RawBState::RawBState (const list<Argument*>& facts)
  : facts(facts), count(0u)
{
  calcHash();
}

RawBState::RawBState(const RawBState& s) noexcept
  : count(0u)
{
  for(list<Argument*>::const_iterator it = s.facts.begin();it != s.facts.end();
                                                                          it++)
    facts.push_back(new Argument(**it));
  hash = s.hash;
}

RawBState& RawBState::operator=(const RawBState& s)
{
  for(list<Argument*>::iterator it = facts.begin();it != facts.end();it++)
    delete *it;
  facts.clear();

  for(list<Argument*>::const_iterator it = s.facts.begin();it != s.facts.end();
                                                                          it++)
    facts.push_back(new Argument(**it));
  hash = s.hash;

  return *this;
}

void RawBState::calcHash()
{
  hash = 0;
  for(list<Argument*>::const_iterator it = facts.begin();it != facts.end();it++)
  {
    hash ^= (*it)->Hash();
  }
}

std::string RawBState::DecodeToString(const core::SymbolTable& symbol_table) const
{
  stringstream ss;
  ss << "State: ";
  for(std::list<libgdl::core::Argument*>::const_iterator it = facts.begin();
                                                    it != facts.end();it++)
  {
    ss << "\t" << (*it)->DecodeToString(symbol_table) << std::endl;
  }

  ss << "\tHash = " << std::hex << hash << std::dec;
  return ss.str();
}

std::string BState::DecodeToString(const core::SymbolTable& symbol_table) const
{
  stringstream ss;
  ss << (*this)->DecodeToString(symbol_table) << std::endl;
  return ss.str();
}

