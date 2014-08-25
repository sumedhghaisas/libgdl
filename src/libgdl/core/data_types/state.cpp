/**
 * @file state_types.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation of State.
 */

#include "state.hpp"

using namespace std;
using namespace boost;
using namespace libgdl;

RawState::RawState (const list<Argument*>& facts,
                    const unordered_map<string, size_t>& id_map)
  : facts(facts), count(0u)
{
  calcHash(id_map);
}

RawState::RawState(const RawState& s)
  : count(0u)
{
  for(list<Argument*>::const_iterator it = s.facts.begin();it != s.facts.end();it++)
    facts.push_back(new Argument(**it));
  hash = s.hash;
}

RawState& RawState::operator=(const RawState& s)
{
  for(list<Argument*>::iterator it = facts.begin();it != facts.end();it++)
    delete *it;
  facts.clear();

  for(list<Argument*>::const_iterator it = s.facts.begin();it != s.facts.end();it++)
    facts.push_back(new Argument(**it));
  hash = s.hash;

  return *this;
}

void RawState::calcHash(const unordered_map<string, size_t>& id_map)
{
  hash = 0;
  for(list<Argument*>::const_iterator it = facts.begin();it != facts.end();it++)
  {
    hash ^= (*it)->Hash(id_map);
  }
}
