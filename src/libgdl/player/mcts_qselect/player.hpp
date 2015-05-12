#ifndef LIBGDL_PLAYER_MCTS_QSELECT_PLAYER_HPP_INCLUDED
#define LIBGDL_PLAYER_MCTS_QSELECT_PLAYER_HPP_INCLUDED

#include <libgdl/core.hpp>
#include <libgdl/player/mcts/player.hpp>

namespace libgdl
{
namespace player
{
namespace mcts_qselect
{

class Player : public mcts::Player
{
 public:
  Player(int argc, char* argv[])
    : mcts::Player(argc, argv)
  {

  }

  AMove SelectMovePolicy()
  {

    auto it = current_node->legal_moves.begin();
    AMove max_move;
    double max_q = 0;
    for(auto it2 : current_node->nexts)
    {
      if(it2->Eval() > max_q)
      {
        max_move = *it;
        max_q = it2->Eval();
      }
      it++;
    }
    return max_move;
  }
};

}
}
}


#endif // LIBGDL_PLAYER_MCTS_QSELECT_PLAYER_HPP_INCLUDED
