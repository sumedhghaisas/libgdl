/**
 * @file variable_map.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of VariableMap.
 */
#ifndef _LIBGDL_CORE_DATATYPES_VARIABLE_MAP_HPP_INCLUDED
#define _LIBGDL_CORE_DATATYPES_VARIABLE_MAP_HPP_INCLUDED

#include <iostream>
#include <string>
#include <map>

#include "argument.hpp"

namespace libgdl
{

typedef std::map<std::string, Argument*> VariableMap;

}

inline std::ostream& operator<<(std::ostream& s, const libgdl::VariableMap& vmap)
{
  for(libgdl::VariableMap::const_iterator it = vmap.begin();it != vmap.end();it++)
    s << it->first << " -> " << *it->second << std::endl;
  return s;
}


#endif // _LIBGDL_CORE_DATATYPES_VARIABLE_MAP_HPP_INCLUDED
