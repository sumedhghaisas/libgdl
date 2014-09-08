/**
 * @file function_policy.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of FunctionPolicy.
 */
#ifndef _LIBGDL_GDLPARSER_PARSER_FUNCTION_POLICY_HPP_INCLUDED
#define _LIBGDL_GDLPARSER_PARSER_FUNCTION_POLICY_HPP_INCLUDED

#include <libgdl/core.hpp>
#include <libgdl/core/data_types/str_var_map.hpp>
#include <libgdl/core/data_types/location.hpp>

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

  static bool CodeGen(Argument*& out,
                      KIFDriver& driver,
                      std::string* command,
                      std::list<Term*>& terms,
                      StrVarMap& v_map,
                      const Location& command_loc);

  static bool CodeGen(KIFDriver& driver,
                      std::string* command,
                      std::list<Term*>& terms,
                      StrVarMap& v_map,
                      const Location& command_loc);
};

}
}
}

#endif // _LIBGDL_GDLPARSER_PARSER_FUNCTION_POLICY_HPP_INCLUDED
