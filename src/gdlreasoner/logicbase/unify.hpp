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

#include <gdlparser/data_types.hpp>

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
    typedef gdlparser::Argument Argument;
    typedef gdlparser::Clause Clause;

public:
    typedef std::map<const gdlparser::Argument*, const gdlparser::Argument*> VariableMap;

    //! most general unifier function
    //! this function returns true if unification is successful and updates sub field in Argument
    //! to appropriate substitution
    //! variable set m stores pointers to variables being substituted
    static bool mgu(const gdlparser::Argument& f, const gdlparser::Argument& arg, VariableMap& m);

    //! Prints Argument with substituted values(For variables)
    //static std::ostream& PrintSubstitution(std::ostream& stream, const gdlparser::Argument& arg, int index = -1);

    //! Equates the two arguments with substitution
    static bool EquateWithSubstitution(const gdlparser::Argument& arg1, const gdlparser::Argument& arg2, const VariableMap& v_map);

    //! Applies substitution to given argument and returns newly constructed argument
    //! the newly constructed argument is alloted in heap
    //! delete the result for yourself
    static Argument* GetSubstitutedArgument(const Argument* arg, const VariableMap& v_map);

    static Clause* GetSubstitutedClause(const Clause* clause, const VariableMap& v_map);

    //! Applies substitution to given clause(head and premisses) and returns newly constructed clause
    //! the newly constructed clause is alloted in heap
    //! delete the result for yourself
    static Clause* GetSubstitutedClause(const Clause* c);

    static VariableMap DecodeSubstitutions(const VariableMap& v_set, const Argument* question, const VariableMap& o_v_map);

    static bool IsGroundQuestion(const Argument* question, const VariableMap& v_map);

    //! perform occur check or not
    static bool doOccurCheck;

private:
    //! occur check involved in unification
    //static bool OccurCheck(const gdlparser::Argument* var, const gdlparser::Argument* t, const VariableSet& m);
};

}
}

#endif // UNIFY_HPP_INCLUDED
