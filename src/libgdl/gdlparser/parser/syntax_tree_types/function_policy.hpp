/**
 * @file function_policy.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of FunctionPolicy.
 */
#ifndef _LIBGDL_GDLPARSER_PARSER_FUNCTION_POLICY_HPP_INCLUDED
#define _LIBGDL_GDLPARSER_PARSER_FUNCTION_POLICY_HPP_INCLUDED

#include <libgdl/core/data_types/variable_map.hpp>

#include "basic_construct.hpp"

namespace libgdl
{
namespace gdlparser
{
namespace parser
{

struct FunctionPolicy
{
  typedef BasicConstruct<FunctionPolicy, size_t, TerminalHold> Term;

  static void CodeGen(Driver& driver,
                      size_t* command,
                      std::list<Term*>& terms,
                      VariableMap& v_map);
};

}
}
}

#include "function_policy_impl.hpp"

#endif // _LIBGDL_GDLPARSER_PARSER_FUNCTION_POLICY_HPP_INCLUDED
