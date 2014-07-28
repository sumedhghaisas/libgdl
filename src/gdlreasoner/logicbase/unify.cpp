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
                //else if((it = theta.find(temp->args[i])) != theta.end()) S.push(temp->sub);
            }
            else S.push(temp->args[i]);
        }
    }
    return true;
}

bool Unify::mgu(const Argument& arg1, const Argument& arg2, VariableSet& theta)
{
    std::stack<std::pair<const Argument*, const Argument*> > S;
    S.push(std::pair<const Argument*, const Argument*>(&arg1, &arg2));

    while(!S.empty())
    {
        std::pair<const Argument*, const Argument*>& p = S.top();
        S.pop();

        const Argument* s = p.first;
        const Argument* t = p.second;

        VariableSet::iterator it;
        if(s->IsVariable() && (it = theta.find(s)) != theta.end())
        {
            const Argument* temp = s->sub[s->sub_count - 1];
            char index = s->sub_no[s->sub_count - 1];
            while(temp->IsVariable())
            {
                const Argument* temp2 = temp;
                temp = temp->sub[(size_t)index];
                index = temp2->sub_no[(size_t)index];
            }
            s = temp;
        }
        while(t->IsVariable() && (it = theta.find(t)) != theta.end())
        {
            const Argument* temp = t->sub[t->sub_count - 1];
            char index = t->sub_no[t->sub_count - 1];
            while(temp->IsVariable())
            {
                const Argument* temp2 = temp;
                temp = temp->sub[(size_t)index];
                index = temp2->sub_no[(size_t)index];
            }
            t = temp;
        }


        if(s->IsVariable() && t->IsVariable())
        {
            theta.insert(s);
            s->sub[(size_t)(s->sub_count)] = t;
            s->sub_no[(size_t)(s->sub_count)] = t->sub_count;
            (s->sub_count)++;
        }
        else if(s->IsVariable())
        {
            if(doOccurCheck)
            {
                if(!OccurCheck(s, t, theta))
                    return false;
            }
            theta.insert(s);
            s->sub[(size_t)(s->sub_count)] = t;
            (s->sub_count)++;
        }
        else if(t->IsVariable())
        {
            if(doOccurCheck)
            {
                if(!OccurCheck(t, s, theta))
                    return false;
            }
            else
            {
                theta.insert(t);
                t->sub[(size_t)(t->sub_count)] = s;
                (t->sub_count++);
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

    std::stack<std::pair<char, char> > S_no;
    S_no.push(std::pair<char, char>(arg1.sub_count - 1, arg2.sub_count - 1));

    while(!S.empty())
    {
        std::pair<const Argument*, const Argument*>& p = S.top();
        S.pop();

        std::pair<char, char>& s_no = S_no.top();
        S_no.pop();

        const Argument* s = p.first;
        const Argument* t = p.second;

        if(s->IsVariable() && t->IsVariable())
        {
            S.push(std::pair<const Argument*, const Argument*>(s->sub[(size_t)(s_no.first)], t->sub[(size_t)s_no.second]));
            S_no.push(std::pair<char, char>(s->sub_no[(size_t)s_no.first], t->sub_no[(size_t)s_no.second]));
        }
        else if(s->IsVariable())
        {
            S.push(std::pair<const Argument*, const Argument*>(s->sub[(size_t)s_no.first], t));
            S_no.push(std::pair<char, char>(s->sub_no[(size_t)s_no.first], 0));
        }
        else if(t->IsVariable())
        {
            S.push(std::pair<const Argument*, const Argument*>(s, t->sub[(size_t)s_no.second]));
            S_no.push(std::pair<char, char>(0, t->sub_no[(size_t)s_no.second]));
        }
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
    if(arg->IsVariable()) return GetSubstitutedArgument(arg->sub[arg->sub_count - 1], arg->sub_no[arg->sub_count - 1]);

    Argument* out = new Argument();
    out->t = arg->t;
    out->val = arg->val;

    for(size_t i = 0; i < arg->args.size(); i++)
        out->args.push_back(GetSubstitutedArgument(arg->args[i]));

    return out;
}

Argument* Unify::GetSubstitutedArgument(const Argument* arg, char s_no)
{
    //std::cout << *arg << std::endl;
    if(arg->IsVariable()) return GetSubstitutedArgument(arg->sub[(size_t)s_no], arg->sub_no[(size_t)s_no]);

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

std::set<const Argument*> Unify::EncodeSubstitutions(const Argument* arg, const std::set<const Argument*>& v_set)
{
    std::set<const Argument*> out;

    std::stack<const Argument*> S;
    S.push(arg);

    while(!S.empty())
    {
        const Argument* temp = S.top();
        S.pop();

        if(temp->IsVariable() && v_set.find(temp) != v_set.end())
        {
            out.insert(temp);
        }
        else
        {
            for(size_t i = 0;i < temp->args.size();i++)
            {
                S.push(temp->args[i]);
            }
        }
    }

    return out;
}

void Unify::DecodeSubstitutions(const std::set<const Argument*>& o_v_set, const std::set<const Argument*>& v_set)
{
    for(std::set<const Argument*>::iterator it = v_set.begin();it != v_set.end();it++)
    {
        if(o_v_set.find(*it) == o_v_set.end())
            (*it)->sub_count--;
    }
}

bool Unify::IsGroundQuestion(const Argument* arg, const std::set<const Argument*>& v_set)
{
    std::stack<const Argument*> S;
    S.push(arg);

    std::stack<int> s_no;
    s_no.push(-1);

    while(!S.empty())
    {
        const Argument* temp = S.top();
        S.pop();

        int index = s_no.top();
        s_no.pop();

        if(temp->IsVariable())
        {
            if(index == -1)
            {
                if(v_set.find(temp) == v_set.end()) return false;
                S.push(temp->sub[temp->sub_count - 1]);
                s_no.push(temp->sub_no[temp->sub_count - 1]);
            }
            else
            {
                S.push(temp->sub[index]);
                s_no.push(temp->sub_no[index]);
            }
        }
        else for(size_t i = 0;i < temp->args.size();i++)
        {
            S.push(temp->args[i]);
            s_no.push(-1);
        }
    }
    return true;
}

std::ostream& Unify::PrintSubstitution(std::ostream& o, const Argument& arg, int index)
{
    if(arg.args.size() == 0)
    {
        if(!arg.IsVariable()) o << arg.val;
        else if(index == -1) PrintSubstitution(o, *(arg.sub[arg.sub_count - 1]), arg.sub_no[arg.sub_count - 1]);
        else PrintSubstitution(o, *(arg.sub[index]), arg.sub_no[index]);
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
