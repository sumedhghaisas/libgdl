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
    (size_t)(exploration_parameter)(= 40),
    (size_t)(role_size)(= 0)
  )

  size_t& ExplorationParameter()
  {
    return exploration_parameter;
  }

  const size_t& ExplorationParameter() const
  {
    return exploration_parameter;
  }

  size_t& RoleSize()
  {
    return role_size;
  }

  const size_t& RoleSize() const
  {
    return role_size;
  }
)

}
}
}


#endif // LIBGDL_PLAYER_MCTS_PLAYER_CONFIG_HPP_INCLUDED
