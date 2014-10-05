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

/**
 * FunctionPolicy handles the semantical analysis part of parsing all the
 * functions in GDL. This policy takes into consideration validity of name and
 * arity. SymbolTable of driver is appropriately updated with new information.
 *
 * @see BasicConstruct
 */
struct FunctionPolicy
{
  //! For simplicity
  typedef BasicConstruct<FunctionPolicy, std::string, TerminalHold> Term;

  //! Generate 'Argument' corresponding to given data
  //! Returns success state
  //!
  //! \param out Generated 'Argument' is returned with this parameter
  //! \param driver KIFDriver object
  //! \param command Name of the gdl function
  //! \param terms Terms inside GDL function
  //! \param v_map Mapping from gdl variable to Argument
  //! \param command_loc Location of this occurrence
  //! \return bool
  //!
  //!
  static bool CodeGen(core::Argument*& out,
                      KIFDriver& driver,
                      std::string* command,
                      std::list<Term*>& terms,
                      core::StrVarMap& v_map,
                      const core::Location& command_loc);

  //! This function is invalid for this policy
  //!
  //! \param driver KIFDriver object
  //! \param command Name of the gdl function
  //! \param terms Premisses
  //! \param v_map Mapping from gdl variable to Argument
  //! \param command_loc Location of this occurrence
  //! \return bool
  //!
  //!
  static bool CodeGen(KIFDriver& driver,
                      std::string* command,
                      std::list<Term*>& terms,
                      core::StrVarMap& v_map,
                      const core::Location& command_loc);
}; // struct ClausePolicy

}; // namespace parser
}; // namespace gdlparser
}; // namespace libgdl

#endif // _LIBGDL_GDLPARSER_PARSER_FUNCTION_POLICY_HPP_INCLUDED
