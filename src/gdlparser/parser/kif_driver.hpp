/**
 * @file kif_driver.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration driver class.
 */
#ifndef __GDLPARSER_KIF_DRIVER_HPP_INCLUDED
#define __GDLPARSER_KIF_DRIVER_HPP_INCLUDED

#include <string>
#include <iostream>
#include <stdlib.h>
#include <map>
#include <vector>
#include <stack>
#include "kif_scanner.hpp"
#include "kif_parser.tab.hh"
#include "token_value.hpp"
#include "token_types.hpp"

namespace gdlparser
{
namespace parser
{
/**
 * This class represents all the semantical analysis side of parsing.
 * Appropriate booking is done in this class to analyze the semantics of GDL after
 * parsing. This class implements symbol table to keep track of symbols and their
 * associated type and arity. Class generates dependency graph at the end of the parsing
 * to check for Unstratified rules( supports DOT format string of dependency graph).
 * This driver supports multiple file parsing.
 *
 */
class KIFDriver
{
    //! some useful typedefs
    typedef gdlparser::parser::yy::KIFParser::location_type location_type;

    /**
     * Represents symbol entry in symbol tables.
     */
    struct Symbol
    {
        //! empty constructor
        Symbol() {}
        //! Easy construct of symbol
        Symbol(size_t id, const std::string& name, bool isRelation, char arity, bool isDefined, const location_type& loc)
            : id(id), name(name), isRelation(isRelation), arity(arity), isDefined(isDefined), first_occurrence(loc) {}

        //! unique ID given to every symbol
        size_t id;
        //! value of the symbol
        std::string name;
        //! is the symbol is used as relation or function
        bool isRelation;
        //! assiciated arity
        char arity;
        //! is the symbol defined
        //! symbol is considered defined if it occurs in at least one fact or head of at least one clause
        bool isDefined;
        //! the location of its first occurance
        //! location of definition if its defined
        location_type first_occurrence;
    };

public:
    //! constructs KIFDriver object
    KIFDriver(std::ostream& stream,
            const std::string& output_filename,
            const std::string& graph_filename,
            bool toGraph = false,
            bool isWarn = true);

    //! destructor, deletes parser and scanner
    virtual ~KIFDriver();

    //! method to start the parsing
    bool Parse();

    //! Add file to parse
    void AddFile(const std::string& filename) { scanner->AddFile(filename); }

    //! stores the dependency graph to a file in DOT format
    void ToGraph(const std::string& filename);

private:
    friend yy::KIFParser;
    friend KIFScanner;

    //! construct a fact from given token
    void AddFact(const TokenValue& fact, const location_type& loc);

    //! construct a rule from given token and location for verbore warnings and errors
    void AddClause(const TokenValue& clause, const location_type& loc);

    //! add entry of the given symbol in symbol table
    int AddEntry(const TokenValue& tok, bool isRelation, char arity, const location_type& loc, std::string& msg);

    //! check entry of the given symbol in symbol table
    //! if entry is not there create the entry with 'not defined' flag
    int CheckEntry(const TokenValue& tok, bool isRelation, char arity, const location_type& loc, std::string& msg);

    //! error and warning logging
    void Warn(const location_type& loc, const std::string& msg) const;
    void Warn(const std::string& msg) const;
    void Error(const location_type& loc, const std::string& msg) const;
    void Error(const std::string& msg) const;

    //! marks dependency of head to given token
    void AddDependency(Node* head, const Argument& arg, size_t c_index, const location_type& loc, bool isNot);

    //! check for stratified negation and stratified recursion
    void CheckCycles();

    //! check if the given clause satisfies Definition 15 mentioned in GDL specifications
    //! with respect to given argument
    void CheckDef15(size_t c_index, const Argument& arg, const std::set<Node*>& scc,
                    const location_type& loc);

    //! checks whether init, base, input is dependent on true, does as its invalid
    //! checks whether legal is dependent on does as its invalid
    void CheckRecursiveDependencies();

    //! recursive function in Tarjan's algorithm
    void StrongConnect(Node* v, std::stack<Node*>& nstack, std::set<Node*>& nset, std::vector<std::set<Node*> >& scc);

    //! pointer to parser and scanner
    gdlparser::parser::yy::KIFParser *parser;
    gdlparser::parser::KIFScanner *scanner;

    //! symbol table
    std::map<std::string, Symbol> symbol_table;

    //! dependency graph
    std::map<std::string, Node*> dgraph;

    //! all the facts and clauses
    std::vector<Fact> facts;
    std::vector<Clause> clauses;

    //! keep track of used ids
    size_t current_id;

    //! pointer to logging stream
    mutable std::ostream* stream;

    //! output and graph filenames
    const std::string& graph_filename;
    const std::string& output_filename;

    //! is graph to be stored as DOT file
    bool toGraph;

    //! enable/disable warnings
    bool isWarn;

    //! stores if any error has occured in parsing
    mutable bool anyError;

};

} // namespace parser
} // namespace gdlparser

#endif // _GDLPARSER_PARSER_KIF_DRIVER_HPP_INCLUDED
