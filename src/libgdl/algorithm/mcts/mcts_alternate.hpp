#ifndef LIBGDL_ALGORITHM_MCTS_HPP_INCLUDED
#define LIBGDL_ALGORITHM_MCTS_HPP_INCLUDED

#include <libgdl/core.hpp>
#include <libgdl/core/data_types/move_list.hpp>

namespace libgdl
{
namespace algorithm
{
namespace mcts_alternate
{

template<class NT, class CT>
class Algorithm
{
 public:
  typedef NT NodeType;
  typedef CT ConfigType;

  void Apply(NodeType* root, ConfigType& config, size_t stop_time);
};

}
}
}

#include "mcts_alternate_impl.hpp"

#endif // LIBGDL_ALGORITHM_MCTS_HPP_INCLUDED
