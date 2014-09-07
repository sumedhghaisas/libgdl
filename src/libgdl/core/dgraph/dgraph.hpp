/**
 * @file dgraph.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration SGraph class.
 */
#ifndef _LIBGDL_CORE_DGRAPH_HPP_INCLUDED
#define _LIBGDL_CORE_DGRAPH_HPP_INCLUDED

#include <iostream>
#include <map>

#include <libgdl/core/symbol_table/symbol_table.hpp>

#include "dgraph_node.hpp"

namespace libgdl
{

class DGraph
{
 public:
  ~DGraph();

  bool AddNode(size_t id);

  bool AddDependency(size_t head,
                     size_t rel,
                     const Clause* c,
                     const Argument* arg,
                     bool isNot = false);

  const std::map<size_t, DGraphNode*>& GetGraph() const
  {
    return graph;
  }

  std::string DecodeToString(const SymbolTable& symbol_table) const;

 private:
  std::map<size_t, DGraphNode*> graph;
};

};

#endif // _LIBGDL_CORE_DGRAPH_HPP_INCLUDED
