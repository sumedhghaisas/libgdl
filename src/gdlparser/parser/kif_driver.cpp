/**
 * @file kif_driver.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation driver class.
 */
#include <cctype>
#include <fstream>
#include <cassert>

#include "kif_driver.hpp"
#include <gdlparser/util/setop.hpp>

#include <fstream>

// Color code escape sequences
#define BASH_RED "\033[0;31m"
#define BASH_YELLOW "\033[0;33m"
#define BASH_CLEAR "\033[0m"

using namespace gdlparser::parser;
using namespace gdlparser::util;

KIFDriver::KIFDriver(std::ostream& stream, const std::string& output_filename,
                    const std::string& graph_filename, bool toGraph, bool isWarn)
                    : stream(&stream), graph_filename(graph_filename),
                    output_filename(output_filename), toGraph(toGraph),
                    isWarn(isWarn)
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
}

void KIFDriver::Error(const location_type& loc, const std::string& msg) const
{
    *stream << BASH_RED "[ERROR] " BASH_CLEAR << loc << ": " << msg << std::endl;
    anyError = true;
}

void KIFDriver::Warn(const location_type& loc, const std::string& msg) const
{
    if(isWarn) *stream << BASH_YELLOW "[WARN] " BASH_CLEAR << loc << ": " << msg << std::endl;
}

void KIFDriver::Error(const std::string& msg) const
{
    *stream << BASH_RED "[ERROR] " BASH_CLEAR << msg << std::endl;
    anyError = true;
}

void KIFDriver::Warn(const std::string& msg) const
{
    if(isWarn) *stream << BASH_YELLOW "[WARN] " BASH_CLEAR << msg << std::endl;
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

    // save the constucted dependency graph to DOT file
    if(toGraph)
    {
        if(graph_filename == "")
        {
            ToGraph(output_filename + ".dot");
        }
        else ToGraph(graph_filename);
    }

    //! check if role, terminal, goal and legal relations are defined
    std::vector<std::string> c_rels;
    c_rels.push_back("role");
    c_rels.push_back("terminal");
    c_rels.push_back("goal");
    c_rels.push_back("legal");
    for(size_t i = 0;i < c_rels.size();i++)
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
    for(std::map<std::string, Symbol>::iterator it = symbol_table.begin();it != symbol_table.end();it++)
    {
        if((it->second).isDefined == false) Warn((it->second).first_occurrence, "Undefined symbol " + (it->second).name);
    }

    if(anyError)
    {
        Error("Parse Failed!");
        return false;
    }

    // if output filename is blank no output is generated
    if(output_filename == "") return true;

    // store all the facts and clauses in given output file
    std::ofstream out(output_filename.c_str());
    if(!out.is_open())
    {
        Error("Unable to open file " + output_filename);
        return false;
    }

    for(int i = facts.size() - 1;i >= 0;i--) out << facts[i] << std::endl;
    for(int i = clauses.size() - 1;i >= 0;i--) out << clauses[i] << std::endl;

    out.close();
    return true;
}

int KIFDriver::AddEntry(const TokenValue& tok, bool isRelation, char arity,
                        const location_type& loc, std::string& msg)
{
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
    Clause c(tok);
    clauses.push_back(c);

    const std::vector<TokenValue>& args = tok.Arguments();
    const std::string& hcommand = args[0].Command();

    const std::set<std::string>& head_vars = tok.HeadVars();
    const std::set<std::string>& bounded_vars = tok.BoundedVars();
    const std::set<std::string>& unbounded_vars = tok.UnboundedVars();

    std::set<std::string> diff;
    diff = setop::setDifference(head_vars, bounded_vars);

    // check if each variable appearing in head appears in bounded variables of body
    if(diff.size() != 0)
    {
        for(std::set<std::string>::const_iterator it = diff.begin();it != diff.end();it++)
        {
            parser->error(loc, "Unsafe Rule: " + c.text + " :Variable " + *it + " appearing in head must appear in a positive relation in the rule's body.");
        }
    }

    diff = setop::setDifference(unbounded_vars, bounded_vars);

    // check if each variable appearing in negation or distinct appears in positive body
    if(diff.size() != 0)
    {
        for(std::set<std::string>::const_iterator it = diff.begin();it != diff.end();it++)
        {
            parser->error(loc, "Unsafe Rule: " + c.text + " :Variable " + *it + " must appear in a positive relation in the rule's body.");
        }
    }

    // modify dependency graph
    std::map<std::string, Node*>::iterator it = dgraph.find(hcommand);
    Node* head;
    if(it == dgraph.end())
    {
        head = new Node(hcommand);
        dgraph[hcommand] = head;
    }
    else head = it->second;

    // add dependency to head of the clause against all arguments
    for(size_t i = 1;i < args.size();i++) AddDependency(head, args[i], loc, false);
}

void KIFDriver::AddDependency(Node* head, const TokenValue& tok, const location_type& loc,
                              bool isNot)
{
    const std::string& command = tok.Command();

    // ignore arguments(relations) 'distinct'
    if(command == "distinct") return;

    // if not incountered add negative dependency to it lone argument
    if(command == "not")
    {
        AddDependency(head, tok.Arguments()[0], loc, !isNot);
        return;
    }
    // if 'or' encountered add dependency recursively to its arguments
    else if(command  == "or")
    {
        const std::vector<TokenValue>&  args = tok.Arguments();
        for(size_t i = 0;i < args.size();i++) AddDependency(head, args[i],loc, isNot);
        return;
    }

    // else drectly add dependency to command of the argument
    std::map<std::string, Node*>::iterator it = dgraph.find(command);
    Node* rel;
    if(it == dgraph.end())
    {
        rel = new Node(command);
        dgraph[command] = rel;
    }
    else rel = it->second;

    bool isFound = false;
    for(size_t i = 0;i < (rel->out).size();i++)
    {
        if(head == (rel->out)[i])
        {
            isFound = true;
            if((rel->isNot)[i] == false && isNot == true)
            {
                (rel->isNot)[i] = true;
                (rel->out_loc)[i] = loc;
            }
        }
    }
    if(!isFound)
    {
        (rel->out).push_back(head);
        (rel->out_loc).push_back(loc);
        (rel->isNot).push_back(isNot);
    }
}

void KIFDriver::AddFact(const TokenValue& tok, const location_type& loc)
{
    Fact f(tok.Command(), tok.Value());

    const std::vector<TokenValue>& args = tok.Arguments();

    for(size_t i = 0;i < args.size();i++) f.AddArgument(args[i]);

    if(f.name == "terminal") Warn(loc, "'terminal' is defined as a fact.");
    else if(f.name == "goal" && f.args[1].val != "100")
    {
        std::cout << f.args[1].val << std::endl;
        Warn(loc, "Goal relation is defined with goal value not equal to 100. Unsupported by the winnable criterion of GDL.");
    }
    facts.push_back(f);
}

void KIFDriver::CheckCycles()
{
    // stores set of strongly connected components
    std::vector<std::set<Node*> > scc;
    // stack of processed nodes
    std::stack<Node*> nstack;
    // set of processed nodes(for faster lookup)
    std::set<Node*> nset;

    //! Tarjan's strongly connected component algorithm
    // current(unique) id to be given as index to every node
    current_id = 0;
    // run until all nodes in the graph are visited
    for(std::map<std::string, Node*>::iterator it = dgraph.begin();it != dgraph.end();it++)
    {
        Node* v = it->second;
        // get strongly connected component of each unvisited Node
        if(v->index == -1) StrongConnect(v, nstack, nset, scc);
    }

    //! check if there is any edge between nodes of same SCC marked negative
    //! if yes then it can be proved easily using SCC properties that a cycle
    //! exists with negative edge in it. Hence unstratified recursion.
    for(size_t i = 0;i < scc.size();i++)
    {
        const std::set<Node*>& sc = scc[i];

        for(std::set<Node*>::const_iterator it = sc.begin();it != sc.end();it++)
        {
            const std::vector<Node*>& out = (*it)->out;
            const std::vector<bool>& isNot = (*it)->isNot;
            const std::vector<location_type>& locs = (*it)->out_loc;
            for(size_t i = 0;i < out.size();i++)
            {
                if(isNot[i] && sc.find(out[i]) != sc.end()) parser->error(locs[i], "Unstratified Recursion : Check relation " + (*it)->name);
            }
        }
    }
}

void KIFDriver::StrongConnect(Node* v, std::stack<Node*>& nstack, std::set<Node*>& nset,
                              std::vector<std::set<Node*> >& scc)
{
    //! Tarjan's Strongly Connected Component Algorithm

    v->index = current_id;
    v->low_link = current_id;
    current_id++;
    nstack.push(v);
    nset.insert(v);

    std::vector<Node*>& out = v->out;

    for(size_t i = 0;i < out.size();i++)
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
        std::set<Node*> nscc;
        Node* temp = NULL;
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

void KIFDriver::ToGraph(const std::string& filename)
{
    std::ofstream graph(filename.c_str());
    if(!graph.is_open())
    {
        Error("Unable to open file" + filename + "for saving graph");
        return;
    }

    graph << "digraph dependency_graph {" << std::endl;

    for(std::map<std::string, Node*>::const_iterator it = dgraph.begin();it != dgraph.end();it++)
    {
        graph << it->first << " [label = \"" << it->first << "\"];" << std::endl;
    }

    for(std::map<std::string, Node*>::const_iterator it = dgraph.begin();it != dgraph.end();it++)
    {
        const std::vector<Node*> out = (it->second)->out;
        const std::vector<bool> isNot = (it->second)->isNot;

        for(size_t i = 0;i < out.size();i++)
        {
            if(isNot[i] == true) graph << it->first << " -> " << out[i]->name << " [color = red];" << std::endl;
            else graph << it->first << " -> " << out[i]->name << ";" << std::endl;
        }
    }

    graph << "}";

    graph.close();
}

void KIFDriver::CheckRecursiveDependencies()
{
    std::map<std::string, Node*>::const_iterator it = dgraph.find("does");

    const Node* does = NULL;
    const Node* ttrue = NULL;

    if((it = dgraph.find("does")) != dgraph.end())
    {
        does = it->second;
        if((it = dgraph.find("true")) != dgraph.end()) ttrue = it->second;
    }
    else if(dgraph.find("true") != dgraph.end()) ttrue = it->second;
    // return if true and does relations are not defined
    else return;


    // check that base relation is not dependent on either true or does
    if((it = dgraph.find("base")) != dgraph.end())
    {
        if(does != NULL && CheckDependency((it->second), does))
            Error("Relation 'base' cannot be dependent on relation 'does'.");
        if(ttrue != NULL && CheckDependency((it->second), ttrue))
            Error("Relation 'base' cannot be dependent on relation 'true'.");
    }

    // check that init relation is not dependent on either true or does
    if((it = dgraph.find("int")) != dgraph.end())
    {
        if(does != NULL && CheckDependency((it->second), does))
            Error("Relation 'int' cannot be dependent on relation 'does'.");
        if(ttrue != NULL && CheckDependency(it->second, ttrue))
            Error("Relation 'init' cannot be dependent on relation 'true'.");
    }

    // check that input relation is not dependent on either true or does
    if((it = dgraph.find("input")) != dgraph.end())
    {
        if(does != NULL && CheckDependency(it->second, does))
            Error("Relation 'input' cannot be dependent on relation 'does'.");
        if(ttrue != NULL && CheckDependency(it->second, ttrue))
            Error("Relation 'input' cannot be dependent on relation 'true'.");
    }

    // check that legal relation is not dependent on does
    if((it = dgraph.find("legal")) != dgraph.end())
    {
        if(does != NULL && CheckDependency(it->second, does))
            Error("Relation 'legal' cannot be dependent on relation 'does'.");
    }
}

bool KIFDriver::CheckDependency(const Node* source, const Node* destination)
{
    // node stack (used for DFS)
    std::stack<const Node*> st;

    st.push(source);
    while(!st.empty())
    {
        const Node* temp = st.top();
        st.pop();

        // if destination is found return true
        if(temp == destination) return true;

        // push all the children in the stack
        for(size_t i = 0;i < (temp->out).size();i++) st.push((temp->out)[i]);
    }
    // return false if destination is not found
    return false;
}
