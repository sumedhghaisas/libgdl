/**
 * @file str_var_map.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation of StrVarMap.
 */
#include "str_var_map.hpp"

#include "argument.hpp"

using namespace std;
using namespace libgdl;
using namespace libgdl::core;

std::ostream& operator<<(std::ostream& s, const StrVarMap& vmap)
{
  for(StrVarMap::const_iterator it = vmap.begin();it != vmap.end();it++)
    s << it->first << " -> " << *it->second << std::endl;
  return s;
}
