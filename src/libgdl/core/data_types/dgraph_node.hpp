/**
 * @file dgraph_node.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of DGraphNode data type.
 */
#ifndef _LIBGDL_CORE_DATATYPES_DGRAPH_NODE_HPP_INCLUDED
#define _LIBGDL_CORE_DATATYPES_DGRAPH_NODE_HPP_INCLUDED

#include <string>
#include <vector>

#include "argument.hpp"
#include <libgdl/gdlparser/parser/location.hh>

namespace libgdl
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
struct DGraphNode
{
  typedef gdlparser::parser::yy::location location_type;

  //! constructor
  DGraphNode(const std::string& name, size_t arity)
    : name(name), arity(arity), index(-1), low_link(-1) {}

  //! represents command name of this node
  std::string name;
  //! arity
  size_t arity;
  //! out edges from this node
  std::vector<DGraphNode*> out;
  //! in edges
  std::vector<DGraphNode*> in;
  //! clause number associated out edge
  std::vector<size_t> c_index;
  //! argument of out edge
  std::vector<Argument> arg;
  //! location of the rule associated with every out edge
  std::vector<location_type> out_loc;
  //! edge type associated with every out edge
  //! If true then its a negative dependency
  std::vector<bool> isNot;

  //! extra variables for Tarjan's algorithm
  int index;
  int low_link;
}; // struct DGraphNode

}; // namespace libgdl


#endif // _LIBGDL_CORE_DATATYPES_DGRAPH_NODE_HPP_INCLUDED
