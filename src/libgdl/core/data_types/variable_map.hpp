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

namespace libgdl
{

class Argument;

typedef std::map<std::string, Argument*> VariableMap;

}

std::ostream& operator<<(std::ostream& s, const libgdl::VariableMap& vmap);

#endif // _LIBGDL_CORE_DATATYPES_VARIABLE_MAP_HPP_INCLUDED
