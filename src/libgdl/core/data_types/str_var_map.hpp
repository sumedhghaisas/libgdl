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
namespace core
{

class Argument;

typedef std::map<std::string, Argument*> StrVarMap;

}; // namespace core
}; // namespace libgdl

std::ostream& operator<<(std::ostream& s,
                         const libgdl::core::StrVarMap& vmap);

#endif // _LIBGDL_CORE_DATATYPES_VARIABLE_MAP_HPP_INCLUDED
