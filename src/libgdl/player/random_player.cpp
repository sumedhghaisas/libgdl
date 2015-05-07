#include "random_player.hpp"

using namespace std;
using namespace libgdl;
using namespace libgdl::player;

RandomPlayer::RandomPlayer(int argc, char* argv[])
  : GamePlayer(argc, argv)
{
  current_state = GetStateMachine().InitState();
}

void RandomPlayer::MetaGame(size_t meta_time)
{

}

string RandomPlayer::SelectMove(size_t play_time)
{
  AMove move = GetStateMachine().GetRandomLegalMove(current_state);
  return move.GetStringRep(RoleID());
}
