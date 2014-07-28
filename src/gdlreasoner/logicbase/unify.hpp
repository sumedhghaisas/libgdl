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
    typedef std::set<const gdlparser::Argument*> VariableSet;
    typedef std::map<std::string, const gdlparser::Argument*> VariableMap;

    //! most general unifier function
    //! this function returns true if unification is successful and updates sub field in Argument
    //! to appropriate substitution
    //! variable set m stores pointers to variables being substituted
    static bool mgu(const gdlparser::Argument& f, const gdlparser::Argument& arg, VariableSet& m);

    //! Prints Argument with substituted values(For variables)
    static std::ostream& PrintSubstitution(std::ostream& stream, const gdlparser::Argument& arg, int index = -1);

    //! Equates the two arguments with substitution
    static bool EquateWithSubstitution(const gdlparser::Argument& arg1, const gdlparser::Argument& arg2);

    //! Applies substitution to given argument and returns newly constructed argument
    //! the newly constructed argument is alloted in heap
    //! delete the result for yourself
    static Argument* GetSubstitutedArgument(const Argument* arg);
    static Argument* GetSubstitutedArgument(const Argument* arg, char s_no);

    //! Applies substitution to given clause(head and premisses) and returns newly constructed clause
    //! the newly constructed clause is alloted in heap
    //! delete the result for yourself
    static Clause* GetSubstitutedClause(const Clause* c);

    static void DecodeSubstitutions(const std::set<const Argument*>& o_v_set, const std::set<const Argument*>& v_set);
    static std::set<const Argument*> EncodeSubstitutions(const Argument* arg, const std::set<const Argument*>& v_set);

    static bool IsGroundQuestion(const Argument* arg, const std::set<const Argument*>& v_set);

    //! perform occur check or not
    static bool doOccurCheck;

private:
    //! occur check involved in unification
    static bool OccurCheck(const gdlparser::Argument* var, const gdlparser::Argument* t, const VariableSet& m);
};

}
}

#endif // UNIFY_HPP_INCLUDED
