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

/**
 * ClausePolicy handles the semantical analysis part of parsing all the
 * Clauses in GDL. This policy takes into consideration restriction GDL poses
 * on Head and other stratification rules.
 *
 * @see BasicConstruct
 */
struct ClausePolicy
{
  //! For simplicity
  typedef RelationPolicy::Term Term;
  //! For simplicity
  typedef RelationPolicy::Sentence Sentence;
  //! For simplicity
  typedef BasicConstruct<RelationPolicy, std::string, Sentence> Premiss;

  //! Invalid function
  static bool CodeGen(core::Argument*& out,
                      KIFDriver& driver,
                      Sentence* command,
                      std::list<Premiss*>& terms,
                      core::StrVarMap& v_map,
                      const core::Location& command_loc);

  //! This function constructs a 'Clause' from given data and registers it in
  //! driver object. It also performs semantical analysis on the constructed
  //! Clause and returns the success state.
  //!
  //! \param driver KIFDriver object
  //! \param command Head of the clause
  //! \param terms
  //! \param v_map Mapping from gdl variable to Argument
  //! \param command_loc Location of this occurrence
  //! \return bool
  //!
  //!
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
