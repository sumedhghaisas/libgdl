/**
 * @file variable_map.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation of VariableMap.
 */
#include "variable_map.hpp"

#include "argument.hpp"

std::ostream& operator<<(std::ostream& s, const libgdl::VariableMap& vmap)
{
  for(libgdl::VariableMap::const_iterator it = vmap.begin();it != vmap.end();it++)
    s << it->first << " -> " << *it->second << std::endl;
  return s;
}
