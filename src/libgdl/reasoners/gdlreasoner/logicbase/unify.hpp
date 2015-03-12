/**
 * @file unify.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of unification functions.
 */
#ifndef UNIFY_HPP_INCLUDED
#define UNIFY_HPP_INCLUDED

#include <set>
#include <string>
#include <map>

#include <libgdl/core.hpp>
#include <libgdl/core/data_types/argument.hpp>
#include <libgdl/core/data_types/clause.hpp>

namespace libgdl
{
namespace gdlreasoner
{
namespace logicbase
{
/**
 * This class implements unification functions and all the supporting functions
 * in query satisfaction.
 *
 * @see KnowledgeBase, Answer
 */
class Unify
{
    //! For simplicity
    typedef libgdl::core::Argument Argument;
    typedef libgdl::core::Clause Clause;
    typedef libgdl::core::VariableMap VariableMap;

public:
    //! Most general unifier function
    //! This function returns true if unification is successful and updates the
    //! the given VariableMap accordingly
    //!
    //! \param f Argument to unify
    //! \param arg Argument to unify with
    //! \param m VariableMap of substitution
    //! \return bool
    //!
    //!
    static bool mgu(const Argument& f,
                    const Argument& arg,
                    VariableMap& m);

    //! Equates the two arguments with substitution from VariableMap
    //!
    //! \param arg1 Argument to compare
    //! \param arg2 Argument to compare with
    //! \param v_map VariableMap for substitution
    //! \return bool
    //!
    //!
    static bool EquateWithSubstitution(const Argument& arg1,
                                       const Argument& arg2,
                                       const VariableMap& v_map);

    //! Applies substitution to given argument and returns newly constructed
    //! argument. The newly constructed argument is alloted memory in heap.
    //! Delete the result for yourself.
    //!
    //! \param arg Argument to copy
    //! \param v_map VariableMap for substitution
    //! \return Argument*
    //!
    //!
    static Argument* GetSubstitutedArgument(const Argument* arg,
                                            const VariableMap& v_map);

    //! Applies substitution to given clause and returns newly constructed
    //! clause. The newly constructed clause is alloted memory in heap.
    //! Delete the result for yourself.
    //!
    //! \param clause Clause to copy
    //! \param v_map VariableMap for substitution
    //! \return Argument*
    //!
    //!
    static Clause* GetSubstitutedClause(const Clause* clause,
                                        const VariableMap& v_map);

    //! This function removes the occurrence of variable of clauses which are
    //! not present in the visited set of current stack. For more information
    //! read about visited set in Answer.hpp
    //!
    //! \param v_map VariableMap of current substitution
    //! \param question Question to be answered
    //! \param o_v_map VariableMap of previous substitutions
    //! \param to_del List of Arguments to delete in the end of substitution
    //! \return VariableMap
    //!
    //! @see Answer
    //!
    //!
    static VariableMap DecodeSubstitutions(const VariableMap& v_map,
                                           const Argument* question,
                                           const VariableMap& o_v_map,
                                           std::list<Argument*>& to_del);

    //! This function returns if the Argument is Ground(No variables) after
    //! given substitution is applied
    //!
    //! \param question Question to be checked
    //! \param v_map VariableMap to substitute
    //! \return bool
    //!
    //!
    static bool IsGroundQuestion(const Argument* question,
                                 const VariableMap& v_map);

    //! Returns the newly constructed Argument after partially substituting
    //! the given argument with given substitution.
    //! The newly constructed Argument is alloted memory in heap.
    //! Delete the Argument for yourself.
    //!
    //! \param arg Argument to substitute
    //! \param o_v_map VariableMap for all the previous substitutions
    //! \param v_map VariableMap of current substitution
    //! \return Argument*
    //!
    //!
    static Argument* GetPartiallySubstitutedArgument(const Argument* arg,
                                                     const VariableMap& o_v_map,
                                                     VariableMap& v_map);
    //! While satisfying the recursive dependent queries the copy of the question
    //! is created to obtain unambiguous substitutions. This function is used
    //! in re-mapping the substitution to original question
    //!
    //!
    //! \param v_map Current substitution
    //! \param e_map Mapping of original question to newly generated question
    //! \param o_v_map VariableMap of all the previous substitutions
    //!
    //!
    static void SpecialMapCompression(VariableMap& v_map,
                                      const VariableMap& e_map,
                                      const VariableMap& o_v_map);

    //! To perform occur check in MGU
    static bool doOccurCheck;
};

};
};
};

#endif // UNIFY_HPP_INCLUDED
