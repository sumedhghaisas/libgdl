/**
 * @file kif_driver.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation driver class.
 */
#include "kif_driver.hpp"

#include <cctype>
#include <fstream>
#include <cassert>
#include <fstream>

#include <libgdl/gdlparser/kif.hpp>

// Color code escape sequences
#define BASH_RED "\033[0;31m"
#define BASH_YELLOW "\033[0;33m"
#define BASH_CLEAR "\033[0m"

using namespace std;
using namespace libgdl;
using namespace libgdl::util;
using namespace libgdl::gdlparser;
using namespace libgdl::gdlparser::parser;

KIFDriver::KIFDriver(KIF& kif)
    : dgraph(kif.dgraph),
    kif(kif),
    streams(kif.streams)
{
    scanner = new KIFScanner(*this);
    parser = NULL;

    current_id = 0;

    anyError = false;
}

KIFDriver::~KIFDriver()
{
    delete(scanner);
    scanner = NULL;
    delete(parser);
    parser = NULL;

    for(std::list<std::string*>::iterator it = to_free.begin();it != to_free.end();it++)
        delete *it;
}

const SymbolTable* KIFDriver::GetSymbolTable() const
{
  return kif.symbol_table;
}

SymbolTable* KIFDriver::GetSymbolTable()
{
  return kif.symbol_table;
}

void KIFDriver::Error(const ErrorType& error)
{
  kif.errors.push_back(error);
}

void KIFDriver::Warning(const ErrorType& warn)
{
  kif.warnings.push_back(warn);
}

void KIFDriver::Error(const location_type& loc, const std::string& msg) const
{
  kif.log.Fatal << loc << ": " << msg << std::endl;
  anyError = true;
}

void KIFDriver::Warn(const location_type& loc, const std::string& msg) const
{
    if(kif.isWarn) kif.log.Warn << loc << ": " << msg << std::endl;
}

void KIFDriver::Error(const std::string& msg) const
{
    kif.log.Fatal << msg << std::endl;
    anyError = true;
}

void KIFDriver::Warn(const std::string& msg) const
{
    if(kif.isWarn) kif.log.Warn << msg << std::endl;
}

bool KIFDriver::Parse()
{
    // create a new parser object
    parser = new yy::KIFParser( (*scanner), (*this));

    // perform syntactical stage of parsing
    if( parser->parse() != 0 || anyError == true)
    {
        // if failed
        Error("Parse Failed!");
        return false;
    }

    //! check if role, terminal, goal and legal relations are defined
    std::vector<std::string> c_rels;
    c_rels.push_back("role");
    c_rels.push_back("terminal");
    c_rels.push_back("goal");
    c_rels.push_back("legal");
    for(size_t i = 0; i < c_rels.size(); i++)
    {
        std::map<std::string, Symbol>::const_iterator it = symbol_table.find(c_rels[i]);
        if(it == symbol_table.end() || (it->second).isDefined == false)
            Error("No '" + c_rels[i] + "' relations found in the input.");
    }

    // check for stratification
    CheckCycles();

    // check that relation init, base and input are not dependent on true or does
    // check that relation legal is not dependent on relation legal
    CheckRecursiveDependencies();

    // show undefiend symbol warnings
    for(std::map<std::string, Symbol>::iterator it = symbol_table.begin(); it != symbol_table.end(); it++)
    {
        if((it->second).isDefined == false) Warn((it->second).first_occurrence, "Undefined symbol " + (it->second).name);
    }

    if(anyError)
    {
        Error("Parse Failed!");
        return false;
    }

    return true;
}

int KIFDriver::AddEntry(const TokenValue& tok, bool isRelation, char arity,
                        const location_type& loc, std::string& msg)
{
    boost::unordered_map<string, size_t>::const_iterator it_k = kif.id_map->find(tok.Value());

    if(it_k == kif.id_map->end())
    {
      (*kif.id_map)[tok.Value()] = kif.id_index++;
    }

    // check if an entry exists against this symbol
    std::map<std::string, Symbol>::iterator it = symbol_table.find(tok.Value());
    // else create a new one
    if(it == symbol_table.end())
    {
        Symbol entry(current_id, tok.Value(), isRelation, arity, true, loc);
        symbol_table[tok.Value()] = entry;
        current_id++;
        return 0;
    }

    Symbol& entry = it->second;

    // check type compability with old entry
    if(entry.isRelation != isRelation)
    {
        msg = "Symbol cannot be used both as function and relation. Symbol '" + tok.Value() + "' is previously defined as";
        if(entry.isRelation) msg += " function at ";
        else msg += " relation at ";
        std::stringstream temp;
        temp << loc;
        msg += temp.str();
        return 1;
    }

    // check arity compability with old entry
    if(entry.arity != arity)
    {
        msg = "Symbol cannot be associated with more than one arity. Symbol '" + tok.Value();
        msg = msg + "' is previously defined with arity " + entry.arity + " at ";
        std::stringstream temp;
        temp << loc;
        return 1;
    }

    // if old entry marks symbol to be undefined
    // mark it as defined
    if(!entry.isDefined)
    {
        entry.isDefined = true;
        entry.first_occurrence = loc;
    }

    return 0;
}

int KIFDriver::CheckEntry(const TokenValue& tok, bool isRelation, char arity,
                          const location_type& loc, std::string& msg)
{
    boost::unordered_map<string, size_t>::const_iterator it_k = kif.id_map->find(tok.Value());

    if(it_k == kif.id_map->end())
    {
      (*kif.id_map)[tok.Value()] = kif.id_index++;
    }

    // check if an entry exists against this symbol
    std::map<std::string, Symbol>::iterator it = symbol_table.find(tok.Value());
    // else create a new one
    if(it == symbol_table.end())
    {
        Symbol entry(current_id, tok.Value(), isRelation, arity, false, loc);
        symbol_table[tok.Value()] = entry;
        current_id++;
        return 0;
    }

    Symbol& entry = it->second;

    // check type compability with old entry
    if(entry.isRelation != isRelation)
    {
        msg = "Symbol cannot be used both as function and relation. Symbol '" + tok.Value() + "' is previously defined as";
        if(entry.isRelation) msg += " function at ";
        else msg += " relation at ";
        std::stringstream temp;
        temp << loc;
        msg += temp.str();
        return 1;
    }

    // check arity compability with old entry
    if(entry.arity != arity)
    {
        msg = "Symbol cannot be associated with more than one arity. Symbol '" + tok.Value();
        msg = msg + "' is previously defined with arity " + entry.arity + " at ";
        std::stringstream temp;
        temp << loc;
        return 1;
    }

    return 0;
}

void KIFDriver::AddClause(const TokenValue& tok, const location_type& loc)
{
    // construct a clause from given token and add to vector of clauses
    Clause c_t(tok, kif.clauses.size());
    const Clause& c = kif.AddClause(c_t, loc);

    const std::vector<Argument*>& args = c.premisses;
    const std::string& hcommand = c.head->val;
    const size_t h_arity = c.head->args.size();

    const std::set<std::string>& head_vars = tok.HeadVars();
    const std::set<std::string>& bounded_vars = tok.BoundedVars();
    const std::set<std::string>& unbounded_vars = tok.UnboundedVars();

    std::set<std::string> diff;
    diff = setop::setDifference(head_vars, bounded_vars);

    std::stringstream temp;
    temp << c;

    // check if each variable appearing in head appears in bounded variables of body
    if(diff.size() != 0)
    {
        for(std::set<std::string>::const_iterator it = diff.begin(); it != diff.end(); it++)
        {
            parser->error(loc, "Unsafe Rule: " + temp.str() + " :Variable " + *it + " appearing in head must appear in a positive relation in the rule's body.");
        }
    }

    diff = setop::setDifference(unbounded_vars, bounded_vars);

    // check if each variable appearing in negation or distinct appears in positive body
    if(diff.size() != 0)
    {
        for(std::set<std::string>::const_iterator it = diff.begin(); it != diff.end(); it++)
        {
            parser->error(loc, "Unsafe Rule: " + temp.str() + " :Variable " + *it + " must appear in a positive relation in the rule's body.");
        }
    }

    // modify dependency graph
    std::map<std::string, DGraphNode*>::iterator it = dgraph.find(hcommand);
    DGraphNode* head;
    if(it == dgraph.end())
    {
        head = new DGraphNode(hcommand, h_arity);
        dgraph[hcommand] = head;
    }
    else head = it->second;

    // add dependency to head of the clause against all arguments
    for(size_t i = 0; i < args.size(); i++) AddDependency(head, *args[i], kif.Clauses().size() - 1, loc, false);
}

void KIFDriver::AddLineMark(const TokenValue& tok)
{
    // get value from the tken
    const std::string& val = tok.Value();

    size_t start = val.find("#line");

    // get line number
    // if invalid return
    size_t i = val.find(" ", start + 1);
    if(i == std::string::npos) return;
    size_t j = val.find(" ", i + 1);
    if(j == std::string::npos) return;
    std::string str_lineNo = val.substr(i + 1, j - i - 1).c_str();

    for (size_t i = 0; i < str_lineNo.length(); i++)
    {
        if (!isdigit(str_lineNo[i])) return;
    }
    size_t lineNo = atoi(str_lineNo.c_str());

    // get filename
    // if none provided assume it to be ""
    i = val.find("\"");
    std::string filename = "";
    if(i != std::string::npos)
    {
        j = val.find("\"", i + 1);
        if(j == std::string::npos) return;
        filename = val.substr(i + 1, j - i - 1);
    }

    std::string* temp = new std::string(filename);

    to_free.push_back(temp);

    // process facts and clauses accordingly
    kif.AddLineMark(location_type(temp, lineNo, 0));
    return;

}

void KIFDriver::AddDependency(DGraphNode* head, const Argument& arg, size_t c_index,
                              const location_type& loc, bool isNot)
{
    const std::string& command = arg.val;
    const size_t arity = arg.args.size();

    // ignore arguments(relations) 'distinct'
    if(command == "distinct") return;

    // if not incountered add negative dependency to it lone argument
    if(command == "not")
    {
        AddDependency(head, *(arg.args[0]), c_index, loc, !isNot);
        return;
    }
    // if 'or' encountered add dependency recursively to its arguments
    else if(command  == "or")
    {
        const std::vector<Argument*>& args = arg.args;
        for(size_t i = 0; i < args.size(); i++) AddDependency(head, *(args[i]), c_index, loc, isNot);
        return;
    }

    // else drectly add dependency to command of the argument
    std::map<std::string, DGraphNode*>::iterator it = dgraph.find(command);
    DGraphNode* rel;
    if(it == dgraph.end())
    {
        rel = new DGraphNode(command, arity);
        dgraph[command] = rel;
    }
    else rel = it->second;

    // add head to its out edges
    (rel->out).push_back(head);
    // add location at which this dependency is found
    (rel->out_loc).push_back(loc);
    // add if its a negative edge
    (rel->isNot).push_back(isNot);
    // add the unique clause number to identify the clause
    (rel->c_index).push_back(c_index);
    // add the argument causing this dependency
    (rel->arg).push_back(arg);

    // add in edge in head
    (head->in).push_back(rel);
}

void KIFDriver::AddFact(Fact&& f_t)
{
    const Fact& f = kif.AddFact(std::move(f_t));

    std::string command = f.arg->val;
    size_t arity = f.arg->args.size();
    std::map<std::string, DGraphNode*>::iterator it = dgraph.find(command);
    if(it == dgraph.end())
    {
        DGraphNode* rel = new DGraphNode(command, arity);
        dgraph[command] = rel;
    }
}

void KIFDriver::AddClause(Clause&& c_t)
{
    kif.AddClause(std::move(c_t));
}

void KIFDriver::CheckCycles()
{
    // stores set of strongly connected components
    std::vector<std::set<DGraphNode*> > scc;
    // stack of processed DGraphNodes
    std::stack<DGraphNode*> nstack;
    // set of processed DGraphNodes(for faster lookup)
    std::set<DGraphNode*> nset;

    //! Tarjan's strongly connected component algorithm
    // current(unique) id to be given as index to every DGraphNode
    current_id = 0;
    // run until all DGraphNodes in the graph are visited
    for(std::map<std::string, DGraphNode*>::iterator it = dgraph.begin(); it != dgraph.end(); it++)
    {
        DGraphNode* v = it->second;
        // get strongly connected component of each unvisited DGraphNode
        if(v->index == -1) StrongConnect(v, nstack, nset, scc);
    }

    //! check if there is any edge between DGraphNodes of same SCC marked negative
    //! if yes then it can be proved easily using SCC properties that a cycle
    //! exists with negative edge in it. Hence unstratified negation.
    for(size_t i = 0; i < scc.size(); i++)
    {
        const std::set<DGraphNode*>& sc = scc[i];

        for(std::set<DGraphNode*>::const_iterator it = sc.begin(); it != sc.end(); it++)
        {
            const std::vector<DGraphNode*>& out = (*it)->out;
            const std::vector<bool>& isNot = (*it)->isNot;
            const std::vector<location_type>& locs = (*it)->out_loc;

            for(size_t i = 0; i < out.size(); i++)
            {
                if(isNot[i] && sc.find(out[i]) != sc.end()) parser->error(locs[i], "Unstratified Negation : Check relation " + (*it)->name);
            }
        }
    }

    //! check that clause for any edge between DGraphNodes of same SCC
    //! definition 15(GDL_spec) is satisfied, else unstratified recursion
    for(size_t i = 0; i < scc.size(); i++)
    {
        const std::set<DGraphNode*>& sc = scc[i];

        for(std::set<DGraphNode*>::const_iterator it = sc.begin(); it != sc.end(); it++)
        {
            const std::vector<DGraphNode*>& out = (*it)->out;
            const std::vector<bool>& isNot = (*it)->isNot;
            const std::vector<size_t>& c_index = (*it)->c_index;
            const std::vector<location_type>& locs = (*it)->out_loc;
            const std::vector<Argument>& arg = (*it)->arg;

            // for every out edge
            for(size_t i = 0; i < out.size(); i++)
            {
                // find non negative edges as negative edges are already considered for unstratified negation
                // also check if the other DGraphNode is also in same scc
                // if it is then check if the clause corresponding to this edge satisfies Definition 15(GDL_spec)
                if(!isNot[i] && sc.find(out[i]) != sc.end()) CheckDef15(c_index[i], arg[i], sc, locs[i]);
            }
        }
    }
}

void KIFDriver::CheckDef15(size_t c_index, const Argument& arg, const std::set<DGraphNode*>& scc,
                           const location_type& loc)
{
    const Clause& c = kif.Clauses()[c_index];

    const std::vector<Argument*>& premisses = c.premisses;

    // find the index of the given argument in the clause
    size_t arg_index = 0;
    for(size_t i = 0;i < premisses.size();i++)
        if(premisses[i]->OrEquate(arg))
        {
            arg_index = i;
            break;
        }

    // check for each argument in given argument
    bool isValid = true;
    size_t invalid_index = 0;
    for(size_t i = 0;i < arg.args.size();i++)
    {
        // check if the argument is ground
        if(arg.args[i]->IsGround()) continue;
        // check if this argument is also argument to head
        if(c.head->HasAsArgument(*(arg.args[i]))) continue;

        bool isFound = false;

        for(size_t j = 0;j < premisses.size();j++)
        {
            // avoid checking in itself
            if(j == arg_index) continue;

            // if another premiss has same same argument and is not in the same SCC
            if(premisses[j]->HasAsArgument(*(arg.args[i])) && scc.find(dgraph[premisses[j]->val]) == scc.end())
            {
                isFound = true;
                break;
            }
        }

        if(!isFound)
        {
            isValid = false;
            invalid_index = i;
            break;
        }
    }

    if(!isValid)
    {
        std::stringstream stream;
        stream << *arg.args[invalid_index];
        Error(loc, "Unstratified Recursion: Relation involved in the cycle is " + arg.val +
                       ". Restriction violated for variable " + stream.str());
    }

}

void KIFDriver::StrongConnect(DGraphNode* v, std::stack<DGraphNode*>& nstack, std::set<DGraphNode*>& nset,
                              std::vector<std::set<DGraphNode*> >& scc)
{
    //! Tarjan's Strongly Connected Component Algorithm

    v->index = current_id;
    v->low_link = current_id;
    current_id++;
    nstack.push(v);
    nset.insert(v);

    std::vector<DGraphNode*>& out = v->out;

    for(size_t i = 0; i < out.size(); i++)
    {
        if(out[i]->index == -1)
        {
            StrongConnect(out[i], nstack, nset, scc);
            v->low_link = (v->low_link > out[i]->low_link) ? out[i]->low_link : v->low_link;
        }
        else if(nset.find(out[i]) != nset.end())
        {
            v->low_link = (v->low_link > out[i]->index) ? out[i]->index : v->low_link;
        }
    }

    if(v->index == v->low_link)
    {
        std::set<DGraphNode*> nscc;
        DGraphNode* temp = NULL;
        while(temp != v)
        {
            temp = nstack.top();
            nstack.pop();
            nset.erase(nset.find(temp));
            nscc.insert(temp);
        }
        scc.push_back(nscc);
    }
}

void KIFDriver::CheckRecursiveDependencies()
{
    std::map<std::string, DGraphNode*>::const_iterator it = dgraph.find("does");

    const DGraphNode* does = NULL;
    const DGraphNode* ttrue = NULL;

    const DGraphNode* base = NULL;
    const DGraphNode* init = NULL;
    const DGraphNode* input = NULL;
    const DGraphNode* legal = NULL;

    if((it = dgraph.find("base")) != dgraph.end()) base = it->second;
    if((it = dgraph.find("init")) != dgraph.end()) init = it->second;
    if((it = dgraph.find("input")) != dgraph.end()) input = it->second;
    if((it = dgraph.find("legal")) != dgraph.end()) legal = it->second;

    if((it = dgraph.find("does")) != dgraph.end())
    {
        does = it->second;

        // perform DFS from does to detect legal, init, input, base
        // DGraphNode stack (used for DFS)
        std::stack<const DGraphNode*> st;
        std::set<const DGraphNode*> n_set;

        st.push(does);
        n_set.insert(does);

        while(!st.empty())
        {
            const DGraphNode* temp = st.top();
            st.pop();

            // if destination is found return true
            if(temp == base) Error("Invalid dependency. Relation 'base' is dependent on relation 'does'.");
            else if(temp == init) Error("Invalid dependency. Relation 'init' is dependent on relation 'does'.");
            else if(temp == legal) Error("Invalid dependency. Relation 'legal' is dependent on relation 'does'.");
            else if(temp == input) Error("Invalid dependency. Relation 'input' is dependent on relation 'does'.");

            // push all the children in the stack
            for(size_t i = 0; i < (temp->out).size(); i++)
            {
                if(n_set.find((temp->out)[i]) == n_set.end())
                {
                    st.push((temp->out)[i]);
                    n_set.insert((temp->out)[i]);
                }
            }
        }
    }

    if((it = dgraph.find("true")) != dgraph.end())
    {
        ttrue = it->second;

        // perform DFS from does to detect legal, init, input, base
        // DGraphNode stack (used for DFS)
        std::stack<const DGraphNode*> st;
        std::set<const DGraphNode*> n_set;

        st.push(ttrue);
        n_set.insert(ttrue);

        while(!st.empty())
        {
            const DGraphNode* temp = st.top();
            st.pop();

            // if destination is found return true
            if(temp == base) Error("Invalid dependency. Relation 'base' is dependent on relation 'true'.");
            else if(temp == init) Error("Invalid dependency. Relation 'init' is dependent on relation 'true'.");
            else if(temp == input) Error("Invalid dependency. Relation 'input' is dependent on relation 'true'.");

            // push all the children in the stack
            for(size_t i = 0; i < (temp->out).size(); i++)
            {
                if(n_set.find((temp->out)[i]) == n_set.end())
                {
                    st.push((temp->out)[i]);
                    n_set.insert((temp->out)[i]);
                }
            }
        }
    }
}
