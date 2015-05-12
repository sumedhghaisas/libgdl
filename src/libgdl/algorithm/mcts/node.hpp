#ifndef LIBGDL_ALGORITHM_MCTSALTERNATE_NODE_HPP_INCLUDED
#define LIBGDL_ALGORITHM_MCTSALTERNATE_NODE_HPP_INCLUDED

namespace libgdl
{
namespace algorithm
{
namespace mcts_alternate
{

template<class NodeType>
struct Node
{
  virtual NodeType* AddChild() = 0;
  virtual bool IsExpanded() const = 0;
  virtual const size_t* Simulate() const = 0;
  virtual bool IsTerminal() const = 0;
  virtual NodeType* BestChild() = 0;
  virtual NodeType* Parent() = 0;
  virtual void Update(const size_t* goals) = 0;
};

}
}
}


#endif // LIBGDL_ALGORITHM_MCTSALTERNATE_NODE_HPP_INCLUDED
