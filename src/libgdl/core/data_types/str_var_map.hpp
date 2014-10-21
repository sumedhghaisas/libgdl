/**
 * @file str_var_map.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of StrVarMap.
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

struct Argument;

/**
 * StrVarMap represents string to variable (Argument*) map which is used in
 * creating clauses where a each variable is assigned a unique Argument* or
 * in other words unique memory location.
 *
 * @see Argument, Clause, VariableMap
 */
typedef std::map<std::string, Argument*> StrVarMap;

std::ostream& operator<<(std::ostream& s,
                         const libgdl::core::StrVarMap& vmap);

}; // namespace core
}; // namespace libgdl

#endif // _LIBGDL_CORE_DATATYPES_VARIABLE_MAP_HPP_INCLUDED
