/**
 * @file variable_map.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of VariableMap.
 */
#ifndef _LIBGDL_CORE_VARIABLE_MAP_HPP_INCLUDED
#define _LIBGDL_CORE_VARIABLE_MAP_HPP_INCLUDED

#include <iostream>
#include <map>

#include "argument.hpp"

namespace libgdl
{
namespace core
{

/**
 * VariableMap represents variable(Argument*, as each variable is given an
 * unique memory location) to variable(Argument*) mapping. VariableMap is used
 * extensively in the process of unification and backchaining.
 *
 * @see Unify, KnowledgeBase
 */
typedef std::map<const Argument*, const Argument*> VariableMap;

inline std::ostream& operator<<(std::ostream& s,
                                const libgdl::core::VariableMap& v_map)
{
  typedef libgdl::core::VariableMap VariableMap;

  for(VariableMap::const_iterator it = v_map.begin();it != v_map.end();it++)
  {
    s << *it->first << "[" << it->first << "] --> ";
    s << *it->second << "[" << it->second << "]" << std::endl;
  }
  return s;
}

}; // namespace core
}; // namespace libgdl

#endif // _LIBGDL_CORE_VARIABLE_MAP_HPP_INCLUDED
