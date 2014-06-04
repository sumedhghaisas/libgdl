/**
 * @file token_types.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation of data types used by driver.
 */
#include "token_types.hpp"

using namespace gdlparser::parser;

Argument::Argument(const TokenValue& tok)
{
    // copy information from token
    if(tok.Type() == TokenValue::Relation) t = Relation;
    else if(tok.Type() == TokenValue::Function) t = Function;
    else t = Var;

    val = tok.Command();

    const std::vector<TokenValue>& args = tok.Arguments();

    for(size_t i = 0;i < args.size();i++) AddArgument(args[i]);
}

void Argument::AddArgument(const TokenValue& tok)
{
    args.push_back(Argument(tok));
}

void Fact::AddArgument(const TokenValue& tok)
{
    args.push_back(Argument(tok));
}

Clause::Clause(const TokenValue& tok)
{
    text = tok.Value();

    const std::vector<TokenValue>& args = tok.Arguments();

    head = Argument(args[0]);

    for(size_t i = 1;i < args.size();i++) AddPremiss(args[i]);
}

void Clause::AddPremiss(const TokenValue& tok)
{
    premisses.push_back(Argument(tok));
}

std::ostream& operator<<(std::ostream& o, const Argument::Type& t)
{
    if(t == Argument::Relation) o << "Relation";
    else if(t == Argument::Function) o << "Function";
    else o << "Variable";
    return o;
}

std::ostream& operator<<(std::ostream& o,const Argument& arg)
{
    if(arg.args.size() == 0)
    {
        o << arg.val;
        return o;
    }
    else o << "( " + arg.val;

    for(size_t i = 0;i < arg.args.size();i++) o << " " << arg.args[i];
    o << " " << ")";
    return o;
}

std::ostream& operator<<(std::ostream& o, const Fact& f)
{
    o << f.text;
    return o;
}

std::ostream& operator<<(std::ostream& o, const Clause& clause)
{
    o << clause.text;
    return o;
}
