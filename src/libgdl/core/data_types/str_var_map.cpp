/**
 * @file str_var_map.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation of VariableMap.
 */
#include "str_var_map.hpp"

#include "argument.hpp"

std::ostream& operator<<(std::ostream& s, const libgdl::StrVarMap& vmap)
{
  for(libgdl::StrVarMap::const_iterator it = vmap.begin();it != vmap.end();it++)
    s << it->first << " -> " << *it->second << std::endl;
  return s;
}
