/**
 * @file token_types.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of data types used by driver.
 */
#ifndef __GDLPARSER_TOKEN_TYPES_H
#define __GDLPARSER_TOKEN_TYPES_H

#include <vector>
#include <iostream>
#include <string>

#include "token_value.hpp"

#include "kif_parser.tab.hh"

namespace gdlparser
{
namespace parser
{
/**
 * Represents dependency graph node.
 * Graph is stored as adjecency list. Each Node stores out degree.
 * Edge also carries extra information like edge type, location of
 * the rule where this dependency is observed.
 * Carries extra variables for Targan's algorithm of obtaining
 * strongly connected components.
 *
 * @see KIFDriver
 */
struct Node
{
    typedef yy::KIFParser::location_type location_type;

    Node(const std::string& name) : name(name), index(-1), low_link(-1) {}

    //! represents command name of this node
    std::string name;
    //! out edges from this node
    std::vector<Node*> out;
    //! location of the rule associated with every out edge
    std::vector<location_type> out_loc;
    //! edge type associated with every out edge
    //! If true then its a negative dependency
    std::vector<bool> isNot;

    //! extra variables for Tarjan's algorithm
    int index;
    int low_link;
};

/**
 * Represents argument of fact or clause.
 * Can be relation, function or variable depending on type.
 * Stored as command and arguments.
 *
 * @see KIFDriver
 */
struct Argument
{
    //! enum type
    enum Type { Relation, Function, Var };

    //! empty constructor
    Argument() {}
    //! constucts argument from given token
    Argument(const TokenValue& tok);

    //! adds argument to this command
    void AddArgument(const TokenValue& tok);

    //! type of this argument
    Type t;
    //! command value
    std::string val;
    //! vector of arguments
    std::vector<Argument> args;
};

/**
 * Represents a fact.
 * Fact is stored as command and its arguments.
 * Text representation is also stored for easy printing.
 *
 * @see KIFDriver
 */
struct Fact
{
    //! empty constructor
    Fact() {}

    //! constructs a fact with given command name and text
    Fact(const std::string& name, const std::string& text)
            : name(name), text(text) {}

    //! Adds argument to this fact
    void AddArgument(const TokenValue& tok);

    //! comparison operators
    bool operator==(const Fact& fact) const;
    bool operator!=(const Fact& fact) const { return !(*this == fact); }

    //! name of the fact (or command)
    std::string  name;
    //! the original text of the fact
    std::string  text;
    //! stores arguments
    std::vector<Argument> args;
};

/**
 * Represents a Clause.
 * Clause is stored as Head and its arguments(body).
 * Text representation is also stored for easy printing.
 *
 * @see KIFDriver
 */
struct Clause
{
    Clause() {}
    Clause(const TokenValue& tok);

    //! add body to this premiss
    void AddPremiss(const TokenValue& tok);

    //! text representation
    std::string text;

    //! Head of the clause
    Argument head;
    //! body
    std::vector<Argument> premisses;
};

}; // namespace parser
}; // namespace gdlparser

/// operator<< for above defined types
std::ostream& operator<< (std::ostream& o, const gdlparser::parser::Argument& arg);
std::ostream& operator<< (std::ostream& o, const gdlparser::parser::Argument::Type& t);
std::ostream& operator<< (std::ostream& o, const gdlparser::parser::Fact& fact);
std::ostream& operator<< (std::ostream& o, const gdlparser::parser::Clause& clause);

#endif
