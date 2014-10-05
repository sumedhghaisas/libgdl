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

/**
 * RelationPolicy handles the semantical analysis part of parsing all the
 * relations(Predicates) in GDL. This policy also handles 'or' and 'not'.
 * This policy takes into consideration validity of name and arity.
 * SymbolTable of driver is appropriately updated with new information.
 *
 * @see BasicConstruct
 */
struct RelationPolicy
{
  //! For simplicity
  typedef FunctionPolicy::Term Term;
  //! For simplicity
  typedef BasicConstruct<RelationPolicy, std::string, Term> Sentence;

  //! This function analyzes predicates when they occur in tail of the clause.
  //! It takes into consideration validity of name and arity along with
  //! restrictions which GDL poses on premisses.
  //! Returns success state
  //!
  //! \param out Generated 'Argument' is returned with this parameter
  //! \param driver KIFDriver object
  //! \param command Name of the gdl predicate
  //! \param terms Terms inside GDL predicate
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
                      const core::Location& loc);

  //! This function analyzes predicates when they occur as fact.
  //! It takes into consideration validity of name and arity along with
  //! restrictions which GDL poses on Facts. The generated Fact is registered
  //! in driver object.
  //! Returns success state
  //!
  //! \param driver KIFDriver object
  //! \param command Name of the gdl predicate
  //! \param terms Terms inside GDL predicate
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

  //! This function analyzes 'not' and 'or' premisses.
  //! Returns success state
  //!
  //! \param out Generated 'Argument' is returned with this parameter
  //! \param driver KIFDriver object
  //! \param command Name of the gdl predicate
  //! \param terms Terms inside GDL predicate
  //! \param v_map Mapping from gdl variable to Argument
  //! \param command_loc Location of this occurrence
  //! \return bool
  //!
  //!
  static bool CodeGen(core::Argument*& out,
                      KIFDriver& driver,
                      std::string* command,
                      std::list<Sentence*>& sentences,
                      core::StrVarMap& v_map,
                      const core::Location& loc);

  //! Invalid function, function is present for avoiding substitution failure
  //!
  //! \param out Generated 'Argument' is returned with this parameter
  //! \param driver KIFDriver object
  //! \param command Name of the gdl predicate
  //! \param terms Terms inside GDL predicate
  //! \param v_map Mapping from gdl variable to Argument
  //! \param command_loc Location of this occurrence
  //! \return bool
  //!
  //!
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
