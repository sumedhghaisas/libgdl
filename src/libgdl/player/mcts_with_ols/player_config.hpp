#ifndef LIBGDL_PLAYER_MCTS_WITH_OLS_PLAYER_CONFIG_HPP_INCLUDED
#define LIBGDL_PLAYER_MCTS_WITH_OLS_PLAYER_CONFIG_HPP_INCLUDED

#include <libgdl/player/default_player_config.hpp>
#include <libgdl/player/mcts/player_config.hpp>

namespace libgdl
{
namespace player
{
namespace mcts_with_ols
{

LIBGDL_PLAYER_CONFIG
(
  mcts::PlayerConfig,

  LIBGDL_PLAYER_CONFIG_PARAMS
  (
    (size_t)(percentage_ols_simulations)(= 30);
  )

  size_t PercentageOLSSimulations()
  {
    return percentage_ols_simulations;
  }
)

}
}
}

#endif // LIBGDL_PLAYER_MCTS_WITH_OLS_PLAYER_CONFIG_HPP_INCLUDED
