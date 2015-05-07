#ifndef LIBGDL_PLAYER_MCTS_PLAYER_HPP_INCLUDED
#define LIBGDL_PLAYER_MCTS_PLAYER_HPP_INCLUDED

#include <list>
#include <string>
#include <cmath>

#include <libgdl/core.hpp>
#include <libgdl/state_machine/propnet_fcdrc.hpp>

#include "../game_player.hpp"
#include "player_config.hpp"

namespace libgdl
{
namespace player
{
namespace mcts
{

class Player : public GamePlayer<state_machine::PropnetFCDRC, PlayerConfig>
{
 private:
  struct MCTSNode
  {
    ~MCTSNode()
    {
        delete[] value;
        for(auto it : nexts)
          delete it;
    }

    size_t n_sim = 0;
    size_t* value = 0;
    size_t turn_role;
    StateType state;
    bool is_terminal = false;

    std::list<MCTSNode*> nexts;

    bool isExpanded = false;
    MCTSNode* parent = NULL;

    MoveList<MoveType> legal_moves = MoveList<MoveType>("");
    MoveList<MoveType>::iterator it;

    inline double GetUCT(size_t exploration_parameter)
    {
      return ((double)value[parent->turn_role]/n_sim) + exploration_parameter * std::sqrt(std::log(parent->n_sim) / n_sim);
    }
  };

 public:

  Player(int argc, char* argv[]);

  void MetaGame(size_t meta_time);

  std::string SelectMove(size_t play_time);

  void ApplyMove(const MoveType& m);

  void StopGame(const MoveType& m);

  const std::string Name() const
  {
    return "MCTS";
  }

 private:
  MCTSNode* current_node;
  size_t role_size;
};

}
}
}


#endif // LIBGDL_PLAYER_MCTS_PLAYER_HPP_INCLUDED
