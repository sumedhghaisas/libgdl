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
  typedef BasicConstruct<FunctionPolicy, std::string, TerminalHold> Term;

  static void CodeGen(Argument*& out,
                      Driver& driver,
                      std::string* command,
                      std::list<Term*>& terms,
                      VariableMap& v_map)
  {

  }

  static void CodeGen(Driver& driver,
                      std::string* command,
                      std::list<Term*>& terms,
                      VariableMap& v_map)
  {

  }
};

}
}
}

#endif // _LIBGDL_GDLPARSER_PARSER_FUNCTION_POLICY_HPP_INCLUDED
