/**
 * @file token_value.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation of custom value type used in bison parsing.
 */
#include "token_value.hpp"

#include <gdlparser/util/setop.hpp>

using namespace gdlparser::parser;
using namespace gdlparser::util;

TokenValue TokenValue::operator+(const TokenValue& tok)
{
    TokenValue out;

    const std::set<std::string>& tok_b = tok.BoundedVars();
    const std::set<std::string>& tok_u = tok.UnboundedVars();

    // union variables
    out.BoundedVars() = setop::setUnion(bounded_vars, tok_b);
    out.UnboundedVars() = setop::setUnion(unbounded_vars, tok_u);

    // add value
    out.Value() = val;
    out.Value() += tok.Value();

    return out;
}

void TokenValue::PerformNot()
{
    unbounded_vars = setop::setUnion(bounded_vars, unbounded_vars);
    bounded_vars = std::set<std::string>();
}

void TokenValue::OrRestrict(const TokenValue& tok)
{
    const std::vector<TokenValue>& args = tok.Arguments();

    // compute variables common to all arguments
    // put all other arguments in unbounded variables
    for(size_t i = 0;i < args.size();i++)
    {
        const std::set<std::string>& b = args[i].BoundedVars();
        const std::set<std::string>& u = args[i].UnboundedVars();

        std::set<std::string> temp = setop::setIntersection(bounded_vars, b);

        unbounded_vars = setop::setUnion(unbounded_vars, setop::setDifference(bounded_vars, temp));
        unbounded_vars = setop::setUnion(unbounded_vars, setop::setDifference(b, temp));
        unbounded_vars = setop::setUnion(unbounded_vars, u);

        bounded_vars = temp;
    }
}

std::ostream& operator<<(std::ostream& stream, const TokenValue& tok)
{
    stream << tok.Command() << "/" << tok.Value() << std::endl;

    stream << "Bounded Vars: " << std::endl;
    const std::set<std::string>& b = tok.BoundedVars();
    for(std::set<std::string>::const_iterator it = b.begin();it != b.end();it++) stream << *it << std::endl;
    stream << std::endl;

    stream << "Unbounded Vars" << std::endl;
    const std::set<std::string>& u = tok.UnboundedVars();
    for(std::set<std::string>::const_iterator it = u.begin();it != u.end();it++) stream << *it << std::endl;

    stream << std::endl << "Arguments" << std::endl;

    for(size_t i = 0;i < tok.Arguments().size();i++)
    {
        stream << (tok.Arguments())[i] << std::endl;
    }

    stream << "---END---" << std::endl;

    return stream;
}

