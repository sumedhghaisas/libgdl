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

State::State (const list<Argument*>& facts,
              const unordered_map<string, size_t>& id_map)
  : facts(facts)
{
    calcHash(id_map);
}

void State::calcHash(const unordered_map<string, size_t>& id_map)
{
  hash = 0;
  size_t index = 1;
  for(list<Argument*>::const_iterator it = facts.begin();it != facts.end();it++)
  {
    hash += (*it)->Hash(id_map) * index++;
  }
}
