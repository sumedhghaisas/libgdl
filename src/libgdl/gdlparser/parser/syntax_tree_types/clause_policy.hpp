/**
 * @file clause_policy.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of ClausePolicy.
 */
#ifndef _LIBGDL_GDLPARSER_PARSER_CLAUSE_POLICY_HPP_INCLUDED
#define _LIBGDL_GDLPARSER_PARSER_CLAUSE_POLICY_HPP_INCLUDED

#include <libgdl/core.hpp>
#include <libgdl/core/data_types/str_var_map.hpp>
#include <libgdl/core/data_types/location.hpp>

#include "basic_construct.hpp"
#include "relation_policy.hpp"

namespace libgdl
{
namespace gdlparser
{
namespace parser
{

struct ClausePolicy
{
  typedef RelationPolicy::Term Term;
  typedef RelationPolicy::Sentence Sentence;
  typedef BasicConstruct<RelationPolicy, std::string, Sentence> Premiss;

  static bool CodeGen(core::Argument*& out,
                      KIFDriver& driver,
                      Sentence* command,
                      std::list<Premiss*>& terms,
                      core::StrVarMap& v_map,
                      const core::Location& command_loc);

  static bool CodeGen(KIFDriver& driver,
                      Sentence* command,
                      std::list<Premiss*>& terms,
                      core::StrVarMap& v_map,
                      const core::Location& command_loc);
}; // struct ClausePolicy

}; // namespace parser
}; // namespace gdlparser
}; // namespace libgdl

#endif // _LIBGDL_GDLPARSER_PARSER_CLAUSE_POLICY_HPP_INCLUDED
