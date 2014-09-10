/**
 * @file relation_policy.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of RelationPolicy.
 */
#ifndef _LIBGDL_GDLPARSER_PARSER_RELATION_POLICY_HPP_INCLUDED
#define _LIBGDL_GDLPARSER_PARSER_RELATION_POLICY_HPP_INCLUDED

#include <libgdl/core/data_types/str_var_map.hpp>
#include <libgdl/core/data_types/location.hpp>

#include "basic_construct.hpp"
#include "function_policy.hpp"

namespace libgdl
{
namespace gdlparser
{
namespace parser
{

struct RelationPolicy
{
  typedef FunctionPolicy::Term Term;
  typedef BasicConstruct<RelationPolicy, std::string, Term> Sentence;

  static bool CodeGen(core::Argument*& out,
                      KIFDriver& driver,
                      std::string* command,
                      std::list<Term*>& terms,
                      core::StrVarMap& v_map,
                      const core::Location& loc);

  static bool CodeGen(KIFDriver& driver,
                      std::string* command,
                      std::list<Term*>& terms,
                      core::StrVarMap& v_map,
                      const core::Location& command_loc);

  static bool CodeGen(core::Argument*& out,
                      KIFDriver& driver,
                      std::string* command,
                      std::list<Sentence*>& sentences,
                      core::StrVarMap& v_map,
                      const core::Location& loc);

  static bool CodeGen(KIFDriver& driver,
                      std::string* command,
                      std::list<Sentence*>& sentences,
                      core::StrVarMap& v_map,
                      const core::Location& command_loc);
}; // struct RelationPolicy

}; // namespace parser
}; // namespace gdlparser
}; // namespace libgdl

#endif // _LIBGDL_GDLPARSER_PARSER_RELATION_POLICY_HPP_INCLUDED
