#ifndef LIBGDL_ALGORITHM_MCTSALTERNATE_CONFIG_HPP_INCLUDED
#define LIBGDL_ALGORITHM_MCTSALTERNATE_CONFIG_HPP_INCLUDED

namespace libgdl
{
namespace algorithm
{
namespace mcts_alternate
{

template<class NodeType>
struct Config
{
  virtual size_t RoleSize() const = 0;
  virtual size_t& NumSimulations() = 0;
  virtual NodeType*& CurrentNode() = 0;
};

}
}
}


#endif // LIBGDL_ALGORITHM_MCTSALTERNATE_CONFIG_HPP_INCLUDED
