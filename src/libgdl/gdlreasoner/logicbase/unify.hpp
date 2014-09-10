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

namespace libgdl
{
namespace gdlreasoner
{
namespace logicbase
{
/**
 * Unification class. This class implements most general unifier and other
 * supporting functions.
 */
class Unify
{
    //! some useful typedefs
    typedef libgdl::core::Argument Argument;
    typedef libgdl::core::Clause Clause;
    typedef libgdl::core::VariableMap VariableMap;

public:
    //! most general unifier function
    //! this function returns true if unification is successful and updates sub
    //! field in Argument
    //! to appropriate substitution
    //! variable set m stores pointers to variables being substituted
    static bool mgu(const Argument& f,
                    const Argument& arg,
                    VariableMap& m);

    //! Equates the two arguments with substitution
    static bool EquateWithSubstitution(const Argument& arg1,
                                       const Argument& arg2,
                                       const VariableMap& v_map);

    //! Applies substitution to given argument and returns newly constructed
    //! argument
    //! the newly constructed argument is alloted in heap
    //! delete the result for yourself
    static Argument* GetSubstitutedArgument(const Argument* arg,
                                            const VariableMap& v_map);

    static Clause* GetSubstitutedClause(const Clause* clause,
                                        const VariableMap& v_map);

    //! Applies substitution to given clause(head and premisses) and
    //! returns newly constructed clause
    //! the newly constructed clause is alloted in heap
    //! delete the result for yourself
    static Clause* GetSubstitutedClause(const Clause* c);

    static VariableMap DecodeSubstitutions(const VariableMap& v_set,
                                           const Argument* question,
                                           const VariableMap& o_v_map,
                                           std::list<Argument*>& to_del);

    static bool IsGroundQuestion(const Argument* question,
                                 const VariableMap& v_map);

    //! perform occur check or not
    static bool doOccurCheck;

private:
    //! occur check involved in unification
    //static bool OccurCheck(const gdlparser::Argument* var,
    //                       const gdlparser::Argument* t,
    //                       const VariableSet& m);
};

};
};
};

#endif // UNIFY_HPP_INCLUDED
