#ifndef LIBGDL_PLAYER_MCTS_PLAYER_CONFIG_HPP_INCLUDED
#define LIBGDL_PLAYER_MCTS_PLAYER_CONFIG_HPP_INCLUDED

#include "../default_player_config.hpp"

namespace libgdl
{
namespace player
{
namespace mcts
{

LIBGDL_PLAYER_CONFIG
(
  DefaultPlayerConfig,

  LIBGDL_PLAYER_CONFIG_PARAMS
  (
    (size_t)(exploration_parameter)(= 40);
  )

  size_t ExplorationParameter()
  {
    return exploration_parameter;
  }
)

}
}
}


#endif // LIBGDL_PLAYER_MCTS_PLAYER_CONFIG_HPP_INCLUDED
