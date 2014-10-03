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

#include <libgdl/core/data_types/clause.hpp>
#include <libgdl/core/data_types/argument.hpp>

namespace libgdl
{
namespace core
{

/**
 * Represents dependency graph node.
 * Graph is stored as adjacency list. Each Node stores out edges and in edges.
 * Each edge also carries extra information like edge type(negative or positive),
 * location where the dependency is observed(pointer to the clause).
 *
 * @see DGraph, KIF
 */
struct DGraphNode
{
  //! Empty constructor
  DGraphNode(size_t id)
    : id(id), index(-1), low_link(-1) {}

  //! SymbolTable ID
  size_t id;
  //! Out edges from this node
  std::vector<DGraphNode*> out;
  //! In edges
  std::vector<DGraphNode*> in;
  //! Pointer to clauses associated with out edges
  std::vector<const Clause*> clauses;
  //! Pointer to argument associated with out edge
  std::vector<const Argument*> args;
  //! edge type associated with every out edge
  //! If true then its a negative dependency
  std::vector<bool> isNot;

  //! Extra variable for Tarjan's algorithm
  mutable int index;
  //! Extra variable for Tarjan's algorithm
  mutable int low_link;
}; // struct DGraphNode

}; // namespace core
}; // namespace libgdl


#endif // _LIBGDL_CORE_DATATYPES_DGRAPH_NODE_HPP_INCLUDED
