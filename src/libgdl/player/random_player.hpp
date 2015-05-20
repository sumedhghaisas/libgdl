#ifndef LIBGDL_PLAYER_RANDOM_PLAYER_HPP_INCLUDED
#define LIBGDL_PLAYER_RANDOM_PLAYER_HPP_INCLUDED

#include <libgdl/core.hpp>
#include <libgdl/state_machine/propnet_fcdrc.hpp>
#include <libgdl/core/data_types/a_move.hpp>
#include <libgdl/core/data_types/a_state.hpp>

#include "game_player.hpp"

namespace libgdl
{
namespace player
{

class RandomPlayer : public GamePlayer<RandomPlayer, state_machine::ForwardDeadReckoningPropnetStateMachine>
{
 public:
  RandomPlayer(int argc, char* argv[]);

  void MetaGame(size_t meta_time);

  std::string SelectMove(size_t play_time);

 private:
  AState current_state;
};

}
}


#endif // LIBGDL_PLAYER_RANDOM_PLAYER_HPP_INCLUDED
