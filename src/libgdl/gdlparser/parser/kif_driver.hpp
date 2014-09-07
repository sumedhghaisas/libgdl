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
#include <boost/unordered_map.hpp>

#include <libgdl/core.hpp>
#include <libgdl/core/util/gdl_stream.hpp>
#include <libgdl/core/symbol_table/symbol_table.hpp>
#include <libgdl/core/dgraph/dgraph.hpp>

#include "kif_scanner.hpp"
#include "kif_parser.tab.hh"
#include "token_value.hpp"
#include "error_type.hpp"

namespace libgdl
{
namespace gdlparser
{
//! forward declaration of class KIF
class KIF;

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
    KIFDriver(KIF &kif);

    //! destructor, deletes parser and scanner
    virtual ~KIFDriver();

    //! method to start the parsing
    bool Parse();

    const SymbolTable* GetSymbolTable() const;
    SymbolTable* GetSymbolTable();

    const DGraph* GetDGraph() const;
    DGraph* GetDGraph();

    void Error(const ErrorType& error);

    void Warning(const ErrorType& warn);

    const Fact& AddFact(Fact&& f_t);
    const Clause& AddClause(Clause&& c_t);

private:
    friend yy::KIFParser;
    friend KIFScanner;

    //! construct a rule from given token and location for verbore warnings and errors
    void AddClause(const TokenValue& clause, const location_type& loc);

    //! add "#line" location mark
    void AddLineMark(const TokenValue& mark);

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

    void AddDependency(DGraphNode* head, const Argument& arg, size_t c_index, const location_type& loc, bool isNot);

    //! check for stratified negation and stratified recursion
    void CheckCycles();

    //! check if the given clause satisfies Definition 15 mentioned in GDL specifications
    //! with respect to given argument
    void CheckDef15(size_t c_index, const Argument& arg, const std::set<DGraphNode*>& scc,
                    const location_type& loc);

    //! checks whether init, base, input is dependent on true, does as its invalid
    //! checks whether legal is dependent on does as its invalid
    void CheckRecursiveDependencies();

    //! recursive function in Tarjan's algorithm
    void StrongConnect(DGraphNode* v, std::stack<DGraphNode*>& nstack, std::set<DGraphNode*>& nset, std::vector<std::set<DGraphNode*> >& scc);

    //! pointer to parser and scanner
    gdlparser::parser::yy::KIFParser *parser;
    gdlparser::parser::KIFScanner *scanner;

    //! symbol table
    std::map<std::string, Symbol> symbol_table;

    //! keep track of used ids
    size_t current_id;

    //! stores if any error has occured in parsing
    mutable bool anyError;

    //! reference to the calling object
    KIF& kif;

    std::vector<util::GDLStream>& streams;

    //! pointers which needs to be freed in destruction
    std::list<std::string*> to_free;
};

}; // namespace parser
}; // namespace gdlparser
}; // namespace libgdl

#endif // _GDLPARSER_PARSER_KIF_DRIVER_HPP_INCLUDED
