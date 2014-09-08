/**
 * @file unify.cpp
 * @author Sumedh Ghaisas
 *
 * Declaration of unification functions.
 */
#include "unify.hpp"

#include <stack>
#include <utility>

using namespace std;
using namespace libgdl;
using namespace libgdl::core;
using namespace libgdl::gdlreasoner::logicbase;
using namespace libgdl::gdlreasoner;
using namespace libgdl::gdlparser;

//! set occur check to false
bool Unify::doOccurCheck = false;

/*
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
*/

bool Unify::mgu(const Argument& arg1, const Argument& arg2, VariableMap& theta)
{
  std::stack<std::pair<const Argument*, const Argument*> > S;
  S.push(std::pair<const Argument*, const Argument*>(&arg1, &arg2));

  while(!S.empty())
  {
    std::pair<const Argument*, const Argument*>& p = S.top();
    S.pop();

    const Argument* s = p.first;
    const Argument* t = p.second;

    VariableMap::iterator it;
    while(s->IsVariable() && (it = theta.find(s)) != theta.end())
    {
      s = it->second;
    }
    while(t->IsVariable() && (it = theta.find(t)) != theta.end())
    {
      t = it->second;
    }

    if(s->IsVariable() && t->IsVariable())
    {
      theta[s] = t;
    }
    else if(s->IsVariable())
    {
      theta[s] = t;
    }
    else if(t->IsVariable())
    {
      theta[t] = s;
    }
    else if(s->value == t->value)
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

bool Unify::EquateWithSubstitution(const Argument& arg1,
                                   const Argument& arg2,
                                   const VariableMap& v_map)
{
    typedef std::pair<const Argument*, const Argument*> ArgPair;

    std::stack<ArgPair> S;
    S.push(ArgPair(&arg1, &arg2));

    while(!S.empty())
    {
        ArgPair& p = S.top();
        S.pop();

        const Argument* s = p.first;
        const Argument* t = p.second;

        if(s->IsVariable() && t->IsVariable())
        {
            S.push(ArgPair(v_map.find(s)->second, v_map.find(t)->second));
        }
        else if(s->IsVariable())
        {
            S.push(ArgPair(v_map.find(s)->second, t));
        }
        else if(t->IsVariable())
        {
            S.push(ArgPair(s, v_map.find(t)->second));
        }
        else if(s->val == t->val && s->args.size() == t->args.size())
        {
            for(size_t i = 0; i < s->args.size(); i++)
            {
                S.push(ArgPair(s->args[i], t->args[i]));
            }
        }
        else return false;
    }

    return true;
}

Argument* Unify::GetSubstitutedArgument(const Argument* arg,
                                        const VariableMap& v_map)
{
    if(arg->IsVariable())
      return GetSubstitutedArgument(v_map.find(arg)->second, v_map);

    Argument* out = new Argument();
    out->t = arg->t;
    out->val = arg->val;

    for(size_t i = 0; i < arg->args.size(); i++)
        out->args.push_back(GetSubstitutedArgument(arg->args[i], v_map));

    return out;
}


Clause* Unify::GetSubstitutedClause(const Clause* c, const VariableMap& v_map)
{
    Clause* out = new Clause();

    out->head = GetSubstitutedArgument(c->head, v_map);

    for(size_t i = 0; i < c->premisses.size(); i++)
        out->premisses.push_back(GetSubstitutedArgument(c->premisses[i], v_map));

    return out;
}


Unify::VariableMap Unify::DecodeSubstitutions(const VariableMap& v_map,
                                              const Argument* question,
                                              const VariableMap& o_v_map,
                                              list<Argument*>& to_del)
{
  VariableMap out = o_v_map;

  std::stack<const Argument*> S;
  S.push(question);

  while(!S.empty())
  {
    const Argument* t = S.top();
    S.pop();

    if(t->IsVariable())
    {
      const Argument* temp = t;
      while(temp->IsVariable())
      {
        temp = v_map.find(temp)->second;
      }
      if(temp->HasVariables())
      {
        Argument* temp2 = Unify::GetSubstitutedArgument(temp, v_map);
        to_del.push_back(temp2);
        out[t] = temp2;
      }
      else out[t] = temp;
    }
    else
    {
      for(size_t i = 0;i < t->args.size();i++)
      {
        S.push(t->args[i]);
      }
    }
  }
  return out;
}

bool Unify::IsGroundQuestion(const Argument* arg, const VariableMap& v_map)
{
    std::stack<const Argument*> S;
    S.push(arg);

    while(!S.empty())
    {
        const Argument* temp = S.top();
        S.pop();

        if(temp->IsVariable())
        {
          VariableMap::const_iterator it;
          while(temp->IsVariable() && (it = v_map.find(temp)) != v_map.end())
          {
            temp = it->second;
          }
          if(temp->IsVariable()) return false;
        }
        else for(size_t i = 0;i < temp->args.size();i++)
        {
            S.push(temp->args[i]);
        }
    }
    return true;
}
