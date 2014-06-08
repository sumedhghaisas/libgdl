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

bool Argument::operator==(const Argument& arg) const
{
    // for 'or', if arg matches any argument return true
    if(val == "or")
    {
        for(size_t i = 0;i < args.size();i++)
            if(args[i] == arg) return true;
        return false;
    }

    if(val != arg.val) return false;
    if(args.size() != arg.args.size()) return false;

    for(size_t i = 0;i < args.size();i++)
        if(args[i] != arg.args[i]) return false;

    return true;
}

void Argument::AddArgument(const TokenValue& tok)
{
    args.push_back(Argument(tok));
}

bool Argument::HasAsArgument(const Argument& arg) const
{
    if(val == "not") return false;
    else if(val == "or")
    {
        bool has = true;
        for(size_t i = 0;i < args.size();i++)
            if(!args[i].HasAsArgument(arg))
            {
                has = false;
                break;
            }

        return has;
    }

    for(size_t i = 0;i < args.size();i++)
        if(args[i] == arg) return true;

    return false;
}

bool Argument::IsGround() const
{
    if(args.size() == 0) return (val[0] == '?') ? false : true;

    for(size_t i = 0;i < args.size();i++)
        if(!args[i].IsGround()) return false;

    return true;
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
