/**
 * @file token_value.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of custom value type used in bison parsing.
 */
#ifndef __GDLPARSER_TOKEN_VALUE_HPP_INCLUDED
#define __GDLPARSER_TOKEN_VALUE_HPP_INCLUDED

#include <string>
#include <iostream>
#include <set>
#include <vector>

namespace gdlparser
{
namespace parser
{
/**
 * This class implements custom value type used in bison parser. Custom value type
 * is used to store all the information related to the token being passed.
 * Relevent information about a token includeds -
 *    1) Value - Represents string value of the token.
 *    2) Command - Represents the command of relation of function. e.g (role robot)
                   command 'role'
 *    3) Arguments - arguments to function or relation. 'robot' will be the argument
 *    4) bounded_vars - set of variable bound in this token. variable inside relation
 *                      'not' and 'distinct' are considered unbounded. For relation
 *                      'or', variables common to all the arguments(again considering
 *                      their bounded variables) are considered bounded.
 *    5) head_vars - variables appearing inside head of a rule
 *    6) type - Type of this token Relation, function or variable, term without any
 *              arguments is considered a function with 0 arity. Rule is special
 *              relation with clause_command('<=') as command.
 *
 * @see KIFParser
 *
 */
class TokenValue
{
public:
    //! Custom type
    enum TType { Relation, Function, Var};

    //! Empty constructor - required by BISON
    TokenValue() { }

    //! Constructs token from c++ string
    TokenValue(const std::string& val) : val(val) {}

    //! Adds variables to the token.
    //!
    //! @param var TokenValue& variables to be added
    //!
    void AddVariable(const TokenValue& var) { bounded_vars.insert(var.Value()); }

    //! All bounded variables will be convereted to unbounded variables.
    void PerformNot();

    //! stores variables of head as special variables
    //!
    //! @param tok TokenValue& : token representation of head
    //!
    void HeadRestrict(const TokenValue& tok) { head_vars = tok.BoundedVars(); }

    //! Check bounded variables of 'or' token
    //!
    //! @param tok TokenValue& : tokens to be added as arguments to 'or'
    //!
    void OrRestrict(const TokenValue& tok);

    //! Adds given token as arguments
    //!
    //! @param tok TokenValue& : token to be added
    //!
    void AddArgument(const TokenValue& tok) { arguments.push_back(tok); }

    //! Adds vector of tokens as arguments
    //!
    //! @param toks vector<TokenValue&>
    //!
    void AddArguments(const std::vector<TokenValue>& toks) { for(size_t i = 0;i < toks.size();i++) arguments.push_back(toks[i]); }

    //! opetaor+ for token with c++ string
    TokenValue operator+(const std::string& str) { val += str; return *this; }
    //! operator+ for token with another token
    //! return new token where its value is addition of argument values
    //! bounded vars of new token will be union of bounded vars of arguments
    //! unbounded vars of new token will be union of unbounded vars of arguments
    TokenValue operator+(const TokenValue& tok);

    //! Comparison operators
    //! Only dependent on values
    bool operator==(const std::string& str) { return (val == str); }
    bool operator!=(const std::string& str) { return !(val == str); }

    //! returns argument count for this token
    int Count() const { return arguments.size(); }

    //! returns vector of arguments
    const std::vector<TokenValue>& Arguments() const { return arguments; }

    //! Getter Setter fot value
    const std::string& Value() const { return val; }
    std::string& Value() { return val; }

    //! Getter setter for bounded vars
    const std::set<std::string>& BoundedVars() const { return bounded_vars; }
    std::set<std::string>& BoundedVars() { return bounded_vars; }

    //! Getter setter for unbounded vars
    const std::set<std::string>& UnboundedVars() const { return unbounded_vars; }
    std::set<std::string>& UnboundedVars() { return unbounded_vars; }

    //! Getter setter for variables in head
    const std::set<std::string>& HeadVars() const { return head_vars; }
    std::set<std::string>& HeadVars() { return head_vars; }

    //! Getter setter for command
    const std::string& Command() const { return command; }
    std::string& Command() { return command; }

    //! Getter setter for type
    const TType& Type() const { return t; }
    TType& Type() { return t; }

private:
    //! represents value
    std::string val;

    //! variables
    std::set<std::string> bounded_vars;
    std::set<std::string> unbounded_vars;
    std::set<std::string> head_vars;

    //! vector of arguments
    std::vector<TokenValue> arguments;

    //! command
    std::string command;

    //! token type
    TType t;
};

} // namespace parser
} // namespace gdlparser

std::ostream& operator<<(std::ostream& stream, const gdlparser::parser::TokenValue& tok);

#endif // TOKEN_VALUE_HPP_INCLUDED
