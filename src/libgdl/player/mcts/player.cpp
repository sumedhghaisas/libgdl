#include "player.hpp"

using namespace std;
using namespace libgdl;
using namespace libgdl::state_machine;
using namespace libgdl::player::mcts;

Player::Player(int argc, char* argv[])
  : GamePlayer(argc, argv), algo_config(GetStateMachine(), GetStateMachine().GetRoleNum())
{
  role_size = GetStateMachine().GetRoleNum();
  init_state = GetStateMachine().InitState();
  current_node = new NodeType(init_state, algo_config);
}

void Player::MetaGame(size_t meta_time)
{

}

AMove Player::SelectMovePolicy()
{
  auto it = current_node->legal_moves.begin();
  AMove max_move;
  size_t max_n = 0;
  for(auto it2 : current_node->nexts)
  {
    if(it2->n_sim > max_n)
    {
      max_move = *it;
      max_n = it2->n_sim;
    }
    it++;
  }
  return max_move;
}

string Player::SelectMove(size_t play_time)
{
  algo_config.NumSimulations() = 0;
  size_t stop_time = util::Timer::microtimer() + play_time * 1000000 - Config().SelectMoveSafetyTime();
  algo.Apply(current_node, algo_config, stop_time);

  cout << "Performed " << algo_config.NumSimulations() << endl;
  return SelectMovePolicy().GetStringRep(RoleID());
}

void Player::ApplyMove(const MoveType& m)
{
  auto it = current_node->nexts.begin();
  for(auto it2 : current_node->legal_moves)
  {
    if(m == it2)
    {
      NodeType* temp = *it;
      *it = NULL;
      //size_t start = util::Timer::microtimer();
      delete current_node;
      //buffer += util::Timer::microtimer() - start;
      current_node = temp;
      current_node->parent = NULL;
      break;
    }
    it++;
  }
}

void Player::StopGame(const MoveType& m)
{
  ApplyMove(m);
  const size_t* goals = GetStateMachine().GetGoals(current_node->state);
  size_t val = goals[RoleID()];
  for(size_t i = 0;i < role_size;i++)
  {
    if(i == RoleID())
      continue;
    if(goals[i] >= val)
      return;
  }
  string filename = Name();
  filename += "_winfile.txt";
  ofstream out_file(filename, std::ios_base::app);
  for(size_t i = 0;i < role_size;i++)
    out_file << goals[i] << " ";
  out_file << endl;
  cout << "History written..." << endl;
}
