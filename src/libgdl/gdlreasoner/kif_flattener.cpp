/**
 * @file kif_flattener.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation of KIFFlattener class.
 */
#include "kif_flattener.hpp"

#include <map>
#include <stack>

#include <libgdl/core/util/setop.hpp>

using namespace libgdl;
using namespace libgdl::gdlreasoner;
using namespace libgdl::gdlparser;
using namespace libgdl::gdlreasoner::logicbase;

void KIFFlattener::Flatten(KIF& kif)
{
    // clear all the lists
    flattened_facts.clear();
    flattened_clauses.clear();

    // construct a knowledge base consisting of entire knowledge
    KnowledgeBase all_kb(kif);

    // the temporary knowledge base
    KnowledgeBase m_kb;

    // dependency graph
    const std::map<std::string, DGraphNode*>& dgraph = kif.DependencyGraph();

    // stores marked relations in Dfs
    std::set<std::string> marked;

    // stores data relations(which are also state independent)
    std::set<std::string> state_independent;

    // mark all the relation dependent on 'does'
    std::map<std::string, DGraphNode*>::const_iterator it;
    if((it = dgraph.find("does")) != dgraph.end())
        DFSMarking(it->second, marked);
    //mark all the relations dependent on 'true'
    if((it = dgraph.find("true")) != dgraph.end())
        DFSMarking(it->second, marked);

    // compute data knowledge and add it t temporary knowledge base
    std::set<std::string>::iterator mit;
    for(it = dgraph.begin(); it != dgraph.end(); it++)
    {
        if((mit = marked.find(it->first)) == marked.end())
        {
            std::stringstream stream;
            stream << it->second->arity;
            std::string sig = it->first + "/" + stream.str();
            if(sig != "base/1" && sig != "input/2" && sig != "init/1" && sig != "role/1" &&
                    sig != "goal/2" && sig != "terminal/0" && sig != "legal/2" && sig != "next/1"
                    && sig != "")
            {
                state_independent.insert(sig);

                const KnowledgeBase::FactVec* data_facts = all_kb.GetFacts(sig);
                if(data_facts != NULL) for(std::list<Fact>::const_iterator it = data_facts->begin();it != data_facts->end();it++)
                        m_kb.Tell(*it);

                const KnowledgeBase::ClauseVec* data_clauses = all_kb.GetClauses(sig);
                if(data_clauses != NULL) for(std::list<Clause>::const_iterator it = data_clauses->begin();it != data_clauses->end();it++)
                        m_kb.Tell(*it);
            }
        }
    }

    // start bottom up dependency flattening
    // start with those relation which are not dependent on other
    // then use dfs to flatten all the relations bottom up
    std::set<std::string> relations_done;
    for(it = dgraph.begin(); it != dgraph.end(); it++)
    {
        if((mit = relations_done.find(it->first)) == relations_done.end())
        {
            std::stack<const DGraphNode*> S_n;
            std::stack<size_t> S_i;

            S_n.push(it->second);
            S_i.push(0);

            while(!S_n.empty())
            {
                const DGraphNode* temp = S_n.top();
                S_n.pop();
                size_t index = S_i.top();
                S_i.pop();

                if((mit = relations_done.find(temp->name)) != relations_done.end()) continue;

                const std::vector<DGraphNode*>& in = temp->in;
                if(in.size() == 0)
                {
                    std::stringstream stream;
                    stream << temp->arity;
                    std::string sig = temp->name + "/" + stream.str();
                    if((mit = state_independent.find(sig)) == state_independent.end())
                        FlattenRelation(temp, all_kb, state_independent, m_kb, flattened_clauses, flattened_facts);
                    relations_done.insert(temp->name);
                }
                else
                {
                    if(index == in.size())
                    {
                        std::stringstream stream;
                        stream << temp->arity;
                        std::string sig = temp->name + "/" + stream.str();
                        if((mit = state_independent.find(sig)) == state_independent.end())
                            FlattenRelation(temp, all_kb, state_independent, m_kb, flattened_clauses, flattened_facts);
                        relations_done.insert(temp->name);
                    }
                    else
                    {
                        S_n.push(temp);
                        S_i.push(index + 1);

                        S_n.push(in[index]);
                        S_i.push(0);
                    }
                }
            }
        }
    }
}

bool KIFFlattener::PrintToFile(const std::string& filename)
{
    std::ofstream myfile(filename.c_str());
    if(!myfile.is_open()) return false;
    for(std::list<Fact>::iterator it = flattened_facts.begin();it != flattened_facts.end();it++)
    {
        if((*it).isLocation) myfile << ";#line " << (*it).loc << std::endl;
        myfile << *it << std::endl;
    }
    for(std::list<Clause>::iterator it = flattened_clauses.begin();it != flattened_clauses.end();it++)
    {
        if((*it).isLocation) myfile << ";#line " << (*it).loc << std::endl;
        myfile << *it << std::endl;
    }
    myfile.close();
    return true;
}

void KIFFlattener::FlattenRelation(const DGraphNode* n, const KnowledgeBase& all_kb, const std::set<std::string>& state_independent,
                                   KnowledgeBase& m_kb, std::list<Clause>& f_clauses, std::list<Fact>& f_facts)
{
    //compute signature f relation
    std::stringstream stream;
    stream << n->arity;
    std::string sig = n->name + "/" + stream.str();

    KnowledgeBase::ClauseVec clauses;

    // get all the facts and clauses associated with this signature
    const KnowledgeBase::FactVec* facts = all_kb.GetFacts(sig);
    const KnowledgeBase::ClauseVec* p_clauses = all_kb.GetClauses(sig);
    if(p_clauses != NULL) clauses = *(all_kb.GetClauses(sig));

    // to store the heads of the flattened clauses
    // these will be added later to temporary knowledge base
    std::list<Argument*> f_heads;

    // start flattening clauses
    for(std::list<Clause>::iterator it = clauses.begin();it != clauses.end();it++)
    {
        // if the clause is already ground add it directly
        // add its head to heads list
        if((*it).IsGround())
        {
            Clause* to_add = new Clause(*it);
            f_clauses.push_back(*to_add);
            f_heads.push_back(to_add->head);
            to_add->head = NULL;
            delete to_add;
            continue;
        }

        // pre-process the clause
        // adjust the extra variables which are present in body but not head
        // adjust 'not' relation appropriately
        Clause* p_clause = ProcessClause(*it, state_independent);

        // add the processed clause temporarily to knowledge base
        size_t c_index = m_kb.Tell(*p_clause);

        // after adding the head of the clause will be the question to ask
        Answer* ans = m_kb.GetAnswer(*p_clause->head, Unify::VariableMap(), std::set<size_t>());
        while(ans->next())
        {
            // compute the answer with substitution
            Clause* to_add = Unify::GetSubstitutedClause(&(*it), ans->GetVariableMap());

            // remove all the occurrences of data relations
            Clause* temp = RemoveDataFromClause(to_add, state_independent);

            // check whether clause can be converted to fact after data relations removal
            if(temp == NULL)
            {
                Argument* h = to_add->head;
                to_add->head = NULL;
                delete to_add;
                f_facts.push_back(*h);
                f_facts.back().loc = (*it).loc;
                f_facts.back().isLocation = (*it).isLocation;
                f_heads.push_back(h);
            }
            else
            {
                f_clauses.push_back(*temp);
                f_clauses.back().loc = (*it).loc;
                f_clauses.back().isLocation = (*it).isLocation;
                f_heads.push_back(temp->head);
                temp->head = NULL;
                delete temp;
            }
        }
        // delete answer
        delete ans;

        // erase the temporary knowledge from knowledge base
        m_kb.Erase(*p_clause, c_index);

        // delete the processed clause(without deleting the variables
        SpecialClauseDelete(p_clause);
    }

    // add all the facts related to this relation to knowledge base
    if(facts != NULL) for(std::list<Fact>::const_iterator it = facts->begin();it != facts->end();it++)
    {
        f_facts.push_back(*it);
        m_kb.Tell(*it);
    }

    // add heads of all the clauses to knowledge base
    for(std::list<Argument*>::iterator it = f_heads.begin();it != f_heads.end();it++)
    {
        m_kb.Tell(Fact(**it));
        delete *it;
    }
}

Clause* KIFFlattener::RemoveDataFromClause(Clause* c, const std::set<std::string>& state_independent)
{
    std::vector<Argument*> args;
    for(size_t i = 0;i < c->premisses.size();i++)
    {
        Argument* p = c->premisses[i];

        std::stringstream stream;
        stream << p->args.size();
        std::string cmd = p->val + "/" + stream.str();

        // delete premiss if data relation
        if(state_independent.find(cmd) != state_independent.end()) delete p;
        // these premisses are no longer required
        else if(p->val == "distinct" || p->val == "role" || p->val == "input" || p->val == "base") delete p;
        // check if there is a data relation inside 'not' relation
        else if(p->val == "not")
        {
            std::stringstream s;
            s << p->args[0]->args.size();
            std::string cmd = p->args[0]->val + "/" + s.str();
            if(state_independent.find(cmd) != state_independent.end()) delete p;
            else args.push_back(p);
        }
        // remove relation inside or relation
        else if(p->val == "or")
        {
            Argument* temp = RemoveDataFromArgument(p, state_independent);
            if(temp != NULL) args.push_back(temp);
            else delete p;
            continue;
        }
        else args.push_back(p);
    }
    if(args.size() == 0)
    {
        c->premisses.clear();
        return NULL;
    }

    c->premisses = args;
    return c;
}

Argument* KIFFlattener::RemoveDataFromArgument(Argument* arg, const std::set<std::string>& state_independent)
{
    std::vector<Argument*> args;
    for(size_t i = 0;i < arg->args.size();i++)
    {
        Argument* p = arg->args[i];

        std::stringstream stream;
        stream << p->args.size();
        std::string cmd = p->val + "/" + stream.str();

        if(state_independent.find(cmd) != state_independent.end()) delete p;
        else if(p->val == "distinct" || p->val == "role" || p->val == "input" || p->val == "base") delete p;
        else if(p->val == "not")
        {
            std::stringstream s;
            s << p->args[0]->args.size();
            std::string cmd = p->args[0]->val + "/" + s.str();
            if(state_independent.find(cmd) != state_independent.end()) delete p;
            else args.push_back(p);
        }
        else if(p->val == "or")
        {
            Argument* temp = RemoveDataFromArgument(p, state_independent);
            if(temp != NULL) args.push_back(temp);
            else delete p;
            continue;
        }
        else args.push_back(p);
    }
    if(args.size() == 0)
    {
        arg->args.clear();
        return NULL;
    }

    arg->args = args;
    return arg;
}

Clause* KIFFlattener::ProcessClause(Clause& c, const std::set<std::string>& state_independent)
{
    std::set<Argument*> head_vars;

    Clause* out = new Clause();

    // make special copy of head
    // shallow copy for variables
    out->head = SpecialArgCopy(c.head, head_vars);

    // make special copy of all the premisses and pre-process them
    for(size_t i = 0; i < c.premisses.size(); i++)
    {
        std::set<Argument*> p_args;

        Argument* p = SpecialArgCopy(c.premisses[i], p_args);
        p = ProcessPremiss(p, state_independent);

        //std::cout << *p << std::endl;

        out->premisses.push_back(p);

        std::set<Argument*> e_vars = util::setop::setDifference(p_args, head_vars);
        for(std::set<Argument*>::iterator it = e_vars.begin(); it != e_vars.end(); it++)
        {
            out->head->args.push_back(*it);
            head_vars.insert(*it);
        }
    }

    return out;
}

Argument* KIFFlattener::SpecialArgCopy(Argument* arg, std::set<Argument*>& vars)
{
    if(arg->IsVariable())
    {
        vars.insert(arg);
        return arg;
    }

    Argument* out = new Argument();
    out->t = arg->t;
    out->val = arg->val;

    for(size_t i = 0; i < arg->args.size(); i++)
        out->args.push_back(SpecialArgCopy(arg->args[i], vars));

    return out;
}

void KIFFlattener::SpecialArgDelete(Argument* arg)
{
    if(arg->IsVariable()) return;

    for(size_t i = 0;i < arg->args.size();i++)
        SpecialArgDelete(arg->args[i]);
    arg->args.clear();
    delete arg;
}

void KIFFlattener::SpecialClauseDelete(Clause* c)
{
    SpecialArgDelete(c->head);
    c->head = NULL;

    for(size_t i = 0;i < c->premisses.size();i++)
        SpecialArgDelete(c->premisses[i]);
    c->premisses.clear();

    delete c;
}

Argument* KIFFlattener::ProcessPremiss(Argument* p, const std::set<std::string>& state_independent)
{
    // convert does premisses to input premisses
    if(p->val == "does") p->val = "input";
    // convert true premisses to base premisses
    else if(p->val == "true") p->val = "base";
    // if relation inside is not is state independent keep the not
    // else remove it
    else if(p->val == "not")
    {
        Argument* arg = p->args[0];
        p->args.clear();

        const std::string& cmd = p->args[0]->val;
        std::set<std::string>::const_iterator it = state_independent.find(cmd);
        if(it == state_independent.end() && cmd != "distinct" && cmd != "role" && cmd != "input" && cmd == "base" && cmd == "init")
        {
            delete p;
            return ProcessPremiss(arg, state_independent);
        }

        p->args.push_back(ProcessPremiss(arg, state_independent));
    }
    // recursively call process on each argument of 'or'
    else if(p->val == "or")
    {
        std::vector<Argument*> args = p->args;
        p->args.clear();

        for(size_t i = 0; i < args.size(); i++)
            p->args.push_back(ProcessPremiss(args[i], state_independent));
    }

    return p;
}

void KIFFlattener::DFSMarking(const DGraphNode* n, std::set<std::string>& marked)
{
    std::stack<const DGraphNode*> S_n;
    std::stack<size_t> S_i;

    S_n.push(n);
    S_i.push(0);

    while(!S_n.empty())
    {
        const DGraphNode* n = S_n.top();
        S_n.pop();

        size_t index = S_i.top();
        S_i.pop();

        marked.insert(n->name);

        if(index == n->out.size()) continue;

        S_n.push(n);
        S_i.push(index + 1);

        S_n.push(n->out[index]);
        S_i.push(0);
    }
}
