/**
 * @file data_types.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation of data types used by driver.
 */
#include "data_types.hpp"

#include <stack>

using namespace gdlparser;

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
            if(*(args[i]) == arg) return true;
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
    args.push_back(new Argument(tok));
}

bool Argument::HasAsArgument(const Argument& arg) const
{
    if(val == "not") return false;
    else if(val == "or")
    {
        bool has = true;
        for(size_t i = 0;i < args.size();i++)
            if(!args[i]->HasAsArgument(arg))
            {
                has = false;
                break;
            }

        return has;
    }

    for(size_t i = 0;i < args.size();i++)
        if(*(args[i]) == arg) return true;

    return false;
}

bool Argument::IsGround() const
{
    std::stack<const Argument*> S;
    S.push(this);

    while(!S.empty())
    {
        const Argument* temp = S.top();
        S.pop();

        if(temp->IsVariable()) return false;
        else for(size_t i = 0;i < temp->args.size();i++)
            S.push(temp->args[i]);
    }
    return true;
}

bool Argument::IsEqualTo(const Argument& arg) const
{
    if(val != arg.val) return false;
    if(args.size() != arg.args.size()) return false;

    for(size_t i = 0;i < args.size();i++)
        if(args[i] != arg.args[i]) return false;

    return true;
}

Clause::Clause(const TokenValue& tok, const size_t id) : id(id)
{
    text = tok.Value();

    const std::vector<TokenValue>& args = tok.Arguments();

    std::map<std::string, Argument*> v_map;

    head = ConstructArgument(args[0], v_map);

    for(size_t i = 1;i < args.size();i++) premisses.push_back(ConstructArgument(args[i], v_map));
}

Argument* Clause::ConstructArgument(const TokenValue& tok, std::map<std::string, Argument*>& v_map)
{
    std::map<std::string, Argument*>::iterator it;
    if(tok.Type() == TokenValue::Var && (it = v_map.find(tok.Command())) != v_map.end())
        return it->second;
    else if(tok.Type() == TokenValue::Var)
    {
        Argument *out = new Argument(tok);
        v_map[tok.Command()] = out;
        return out;
    }

    Argument *out = new Argument();

    if(tok.Type() == TokenValue::Relation) out->t = Argument::Relation;
    else if(tok.Type() == TokenValue::Function) out->t = Argument::Function;
    else out->t = Argument::Var;

    out->val = tok.Command();

    const std::vector<TokenValue>& args = tok.Arguments();

    for(size_t i = 0;i < args.size();i++) out->args.push_back(ConstructArgument(args[i], v_map));

    return out;
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

    for(size_t i = 0;i < arg.args.size();i++) o << " " << *(arg.args[i]);
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
