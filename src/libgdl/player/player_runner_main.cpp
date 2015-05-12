//#define PLAYER_INCLUDE <libgdl/player/mcts/player_include.hpp>

#include <libgdl/core.hpp>
#include <libgdl/player_runner.hpp>
#include PLAYER_INCLUDE

using namespace std;
using namespace libgdl;

int main(int argc, char* argv[])
{
  srand (time(NULL));
  PlayerRunner<Player> player_runner(9147, GLOBAL_LOG, argc, argv);
  player_runner.Run();
  return 0;
}
