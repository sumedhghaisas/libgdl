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
#include <atomic>
#include <boost/intrusive_ptr.hpp>

#include <libgdl/core/symbol_table/symbol_table.hpp>
#include <libgdl/core/data_types/error_type.hpp>

#include "dgraph_node.hpp"

namespace libgdl
{
namespace core
{

class RawDGraph
{
 public:
  RawDGraph() : current_id(0) {}

  ~RawDGraph();

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

  //! Refernce count stored in atomic variable (useful for multi-threading)
  std::atomic_size_t count;

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

//! Intrusive pointer release function for RawDGraph
//! Decrements reference count of RawDGraph object
//!
//! \param p object to decrement
//!
//!
inline void intrusive_ptr_release(RawDGraph* p)
{
   if (--p->count == 0u)
        delete p;
}

//! Intrusive pointer add function for RawDGraph
//! Increments reference count
//!
//! \param p object to increment
//!
//!
inline void intrusive_ptr_add_ref(RawDGraph* p)
{
  ++p->count;
}

class DGraph : public boost::intrusive_ptr<RawDGraph>
{
 public:
  DGraph()
    : boost::intrusive_ptr<RawDGraph>(new RawDGraph()) {}

  DGraph(RawDGraph* graph)
    : boost::intrusive_ptr<RawDGraph>(graph) {}

  bool AddNode(size_t id) { return get()->AddNode(id); }

  bool AddDependency(size_t head,
                     size_t rel,
                     const Clause* c,
                     const Argument* arg,
                     bool isNot = false)
  {
    return get()->AddDependency(head, rel, c, arg, isNot);
  }

  std::list<ErrorType> CheckCycles(const SymbolTable& symbol_table)
  {
    return get()->CheckCycles(symbol_table);
  }

  std::list<ErrorType> CheckRecursiveDependencies()
  {
    return get()->CheckRecursiveDependencies();
  }

  const std::map<size_t, DGraphNode*>& GetGraph() const
  {
    return get()->GetGraph();
  }

  std::string DecodeToString(const SymbolTable& symbol_table) const
  {
    return get()->DecodeToString(symbol_table);
  }
};

}; // namespace core
}; // namespace libgdl

#endif // _LIBGDL_CORE_DGRAPH_HPP_INCLUDED
