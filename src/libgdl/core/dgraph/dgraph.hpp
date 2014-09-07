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
#include <stack>
#include <set>
#include <list>

#include <libgdl/core/symbol_table/symbol_table.hpp>
#include <libgdl/core/data_types/error_type.hpp>

#include "dgraph_node.hpp"

namespace libgdl
{

class DGraph
{
 public:
  DGraph() : current_id(0) {}

  ~DGraph();

  bool AddNode(size_t id);

  bool AddDependency(size_t head,
                     size_t rel,
                     const Clause* c,
                     const Argument* arg,
                     bool isNot = false);

  std::list<ErrorType> CheckCycles(const SymbolTable& symbol_table);
  std::list<ErrorType> CheckRecursiveDependencies();

  const std::map<size_t, DGraphNode*>& GetGraph() const
  {
    return graph;
  }

  std::string DecodeToString(const SymbolTable& symbol_table) const;

 private:
  void StrongConnect(DGraphNode* v,
                     std::stack<DGraphNode*>& nstack,
                     std::set<DGraphNode*>& nset,
                     std::vector<std::set<DGraphNode*> >& scc);

  void CheckDef15(const Clause* clause,
                  const Argument* arg,
                  const std::set<DGraphNode*>& scc,
                  const SymbolTable& symbol_table,
                  std::list<ErrorType>& errors);

  std::map<size_t, DGraphNode*> graph;

  size_t current_id;
};

};

#endif // _LIBGDL_CORE_DGRAPH_HPP_INCLUDED
