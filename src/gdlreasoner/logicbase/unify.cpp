/**
 * @file unify.cpp
 * @author Sumedh Ghaisas
 *
 * Declaration of unification functions.
 */
#include "unify.hpp"

#include <stack>
#include <utility>

using namespace gdlreasoner::logicbase;
using namespace gdlreasoner;
using namespace std;
using namespace gdlparser;

//! set occur check to false
bool Unify::doOccurCheck = false;

bool Unify::OccurCheck(const Argument* var, const Argument* t,const VariableSet& theta)
{
    std::stack<const Argument*> S;
    S.push(t);
    while(!S.empty())
    {
        const Argument* temp = S.top();
        S.pop();

        for(size_t i = 0; i < temp->args.size(); i++)
        {
            if(temp->args[i]->IsVariable())
            {
                VariableSet::const_iterator it;
                if(var->val == temp->args[i]->val) return false;
                else if((it = theta.find(temp->args[i])) != theta.end()) S.push(temp->sub);
            }
            else S.push(temp->args[i]);
        }
    }
    return true;
}

bool Unify::mgu(const Argument& arg1, const Argument& arg2, VariableSet& theta)
{
    arg1.sub = &arg1;
    arg2.sub = &arg2;

    std::stack<std::pair<const Argument*, const Argument*> > S;
    S.push(std::pair<const Argument*, const Argument*>(&arg1, &arg2));

    while(!S.empty())
    {
        std::pair<const Argument*, const Argument*>& p = S.top();
        S.pop();

        const Argument* s = p.first;
        const Argument* t = p.second;

        VariableSet::iterator it;
        while(s->IsVariable() && (it = theta.find(s)) != theta.end())
        {
            s = s->sub;
        }
        while(t->IsVariable() && (it = theta.find(t)) != theta.end())
        {
            t = t->sub;
        }


        if(s->IsVariable() && t->IsVariable())
        {
            theta.insert(s);
            s->sub = t;
        }
        else if(s->IsVariable())
        {
            if(doOccurCheck)
            {
                if(OccurCheck(s, t, theta))
                {
                    theta.insert(s);
                    s->sub = t;
                }
                else return false;
            }
            else
            {
                theta.insert(s);
                s->sub = t;
            }
        }
        else if(t->IsVariable())
        {
            if(doOccurCheck)
            {
                if(OccurCheck(t, s, theta))
                {
                    theta.insert(t);
                    t->sub = s;
                }
                else return false;
            }
            else
            {
                theta.insert(t);
                t->sub = s;
            }

        }
        else if(s->val == t->val)
        {
            if(s->args.size() != t->args.size()) return false;
            for(size_t i = 0; i < s->args.size(); i++)
            {
                S.push(std::pair<const Argument*, const Argument*>(s->args[i], t->args[i]));
            }
        }
        else return false;

    }
    return true;
}

bool Unify::EquateWithSubstitution(const Argument& arg1, const Argument& arg2)
{
    std::stack<std::pair<const Argument*, const Argument*> > S;
    S.push(std::pair<const Argument*, const Argument*>(&arg1, &arg2));

    while(!S.empty())
    {
        std::pair<const Argument*, const Argument*>& p = S.top();
        S.pop();

        const Argument* s = p.first;
        const Argument* t = p.second;

        if(s->IsVariable() && t->IsVariable())
            S.push(std::pair<const Argument*, const Argument*>(s->sub, t->sub));
        else if(s->IsVariable())
            S.push(std::pair<const Argument*, const Argument*>(s->sub, t));
        else if(t->IsVariable())
            S.push(std::pair<const Argument*, const Argument*>(s, t->sub));
        else if(s->val == t->val && s->args.size() == t->args.size())
        {
            for(size_t i = 0; i < s->args.size(); i++)
            {
                S.push(std::pair<const Argument*, const Argument*>(s->args[i], t->args[i]));
            }
        }
        else return false;
    }

    return true;
}

Argument* Unify::GetSubstitutedArgument(const Argument* arg)
{

    //std::cout << *arg << std::endl;
    if(arg->IsVariable()) return GetSubstitutedArgument(arg->sub);

    Argument* out = new Argument();
    out->t = arg->t;
    out->val = arg->val;

    for(size_t i = 0; i < arg->args.size(); i++)
        out->args.push_back(GetSubstitutedArgument(arg->args[i]));

    return out;
}

Clause* Unify::GetSubstitutedClause(const Clause* c)
{
    Clause* out = new Clause();

    out->head = GetSubstitutedArgument(c->head);

    for(size_t i = 0; i < c->premisses.size(); i++)
        out->premisses.push_back(GetSubstitutedArgument(c->premisses[i]));

    return out;
}

std::ostream& Unify::PrintSubstitution(std::ostream& o, const Argument& arg)
{
    if(arg.args.size() == 0)
    {
        if(!arg.IsVariable()) o << arg.val;
        else PrintSubstitution(o, *(arg.sub));
        return o;
    }

    else o << "( " + arg.val;

    for(size_t i = 0; i < arg.args.size(); i++)
    {
        o << " ";
        PrintSubstitution(o, *(arg.args[i]));
    }
    o << " " << ")";
    return o;
}
