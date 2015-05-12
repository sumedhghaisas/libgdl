#include "player.hpp"

using namespace std;
using namespace libgdl;
using namespace libgdl::state_machine;
using namespace libgdl::player::mcts_with_ols;

Player::Player(int argc, char* argv[])
  : GamePlayer(argc, argv)
{
  role_size = GetStateMachine().GetRoleNum();

  current_node = new MCTSNode();
  current_node->value = new size_t[role_size];
  for(size_t i = 0;i < role_size;i++)
    current_node->value[i] = 0;
  current_node->state = GetStateMachine().InitState();
  GetStateMachine().GetLegalMoves(current_node->state,
                                  current_node->legal_moves,
                                  current_node->turn_role,
                                  current_node->is_terminal);
  current_node->it = current_node->legal_moves.begin();
  //cout << current_node->legal_moves << endl;
}

void Player::MetaGame(size_t meta_time)
{

}

string Player::SelectMove(size_t play_time)
{
  size_t num_simulations = 0;
  size_t num_terminal = 0;

  MCTSNode* node = current_node;
  size_t stop_time = util::Timer::microtimer() + play_time * 1000000 - Config().SelectMoveSafetyTime();
  while(util::Timer::microtimer() < stop_time)
  {
    //cout << stop_time - util::Timer::microtimer() << endl;
    node = current_node;
    while(true)
    {
      //cout << node->state << endl;
      //cout << node->turn_role << endl;

      if(node->is_terminal)
      {
        num_terminal++;
        break;
      }

      if(!node->isExpanded)
      {
        MCTSNode* temp = new MCTSNode();
        temp->parent = node;
        temp->value = new size_t[role_size];
        for(size_t i = 0;i < role_size;i++)
          temp->value[i] = 0;
        //cout << *node->it << endl;
        temp->state = GetStateMachine().GetNextState(node->state,
                                                     *node->it);
        GetStateMachine().GetLegalMoves(temp->state,
                                        temp->legal_moves,
                                        temp->turn_role,
                                        temp->is_terminal);
        temp->parent = node;
        node->nexts.push_back(temp);
        node->it++;
        if(node->it == node->legal_moves.end())
          node->isExpanded = true;
        if(temp->is_terminal)
        {
          temp->isExpanded = true;
        }
        else temp->it = temp->legal_moves.begin();
        node = temp;
        break;
      }

      MCTSNode* max_node = NULL;
      double max_value = 0;

      for(auto it : node->nexts)
      {
        double uct = it->GetUCT(Config().ExplorationParameter());
        //cout << uct << endl;
        if(uct > max_value)
        {
          max_value = uct;
          max_node = it;
        }
      }
      node = max_node;
    }
    num_simulations++;
    const size_t* goals;
    if(num_simulations < 50000)
    {
      goals = GetStateMachine().Simulate5(node->state);
    }
    else goals = GetStateMachine().Simulate2(node->state);

//    for(size_t i = 0;i < role_size;i++)
//      cout << goals[i] << " ";
//    cout << endl;
    MCTSNode* up_node = node;
    while(up_node != NULL)
    {
      for(size_t i = 0;i < role_size;i++)
        up_node->value[i] += goals[i];
      up_node->n_sim++;
      up_node = up_node->parent;
    }

//    if(num_simulations > 10000)
//      break;
  }

  auto it = current_node->legal_moves.begin();
  AMove max_move;
  size_t max_n = 0;
  for(auto it2 : current_node->nexts)
  {
//    cout << *it << " " << it2->n_sim << " ";
//    for(size_t i = 0;i < role_size;i++)
//      cout << it2->value[i] << " ";
//    cout << endl;
    if(it2->n_sim > max_n)
    {
      max_move = *it;
      max_n = it2->n_sim;
    }
    it++;
  }
//  if(current_node->nexts.empty())
//  {
//    cout << "nexts empty!" << endl;
//    cout << "in state: " << endl;
//    cout << current_node->state << endl;
//  }

  cout << "Performed " << num_simulations << endl;
  cout << "Num terminal : " << num_terminal << endl;
  //cout << max_move << endl;
  return max_move.GetStringRep(RoleID());
}

void Player::ApplyMove(const MoveType& m)
{
  auto it = current_node->nexts.begin();
  for(auto it2 : current_node->legal_moves)
  {
    //cout << it2 <<endl;
    if(m == it2)
    {
      MCTSNode* temp = *it;
      *it = NULL;
      delete current_node;
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
  for(size_t i = 0;i < role_size;i++)
    cout << goals[i] << " ";
  cout << endl;
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
}
