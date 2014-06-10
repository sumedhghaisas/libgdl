/**
 * @file node.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of data types used by driver.
 */
#ifndef __GDLPARSER_PARSER_NODE_HPP_INCLUDED
#define __GDLPARSER_PARSER_NODE_HPP_INCLUDED

#include <string>
#include <vector>
#include <gdlparser/parser/kif_parser.tab.hh>
#include <gdlparser/data_types.hpp>

namespace gdlparser
{
namespace parser
{
/**
 * Represents dependency graph node.
 * Graph is stored as adjecency list. Each Node stores out degree.
 * Edge also carries extra information like edge type, location and index
 * of the rule where this dependency is observed.
 * Carries extra variables for Targan's algorithm for obtaining
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
    //! clause number associated out edge
    std::vector<size_t> c_index;
    //! argument
    std::vector<Argument> arg;
    //! location of the rule associated with every out edge
    std::vector<location_type> out_loc;
    //! edge type associated with every out edge
    //! If true then its a negative dependency
    std::vector<bool> isNot;

    //! extra variables for Tarjan's algorithm
    int index;
    int low_link;
};

} // namespace parser
} // namespace gdlparser

#endif
