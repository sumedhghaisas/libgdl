#ifndef LIBGDL_PLAYER_MCTS_PLAYER_HPP_INCLUDED
#define LIBGDL_PLAYER_MCTS_PLAYER_HPP_INCLUDED

#include <list>
#include <string>
#include <cmath>

#include <libgdl/core.hpp>
#include <libgdl/state_machine/propnet_fcdrc.hpp>
#include <libgdl/algorithm/mcts/default_node.hpp>
#include <libgdl/algorithm/mcts/mcts_alternate.hpp>
#include <libgdl/algorithm/mcts/config.hpp>

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
  struct AlgoConfig;

 public:
  typedef algorithm::mcts_alternate::DefaultNode<AlgoConfig> NodeType;
  typedef algorithm::mcts_alternate::Algorithm<NodeType, AlgoConfig> AlgorithmType;

 private:
  struct AlgoConfig : public PlayerConfig, algorithm::mcts_alternate::Config<NodeType>
  {
    typedef AState StateType;
    typedef AMove MoveType;

    AlgoConfig(StateMachineType& state_machine, size_t role_size)
      : state_machine(state_machine), role_size(role_size)
    {

    }

    StateMachineType& GetStateMachine()
    {
      return state_machine;
    }

    NodeType*& CurrentNode()
    {
      return current_node;
    }

    size_t& NumSimulations()
    {
      return num_simulations;
    }

    size_t RoleSize() const
    {
      return role_size;
    }

    StateMachineType& state_machine;
    size_t num_simulations = 0;
    size_t role_size = 0;
    NodeType* current_node = NULL;
  };

public:
  Player(int argc, char* argv[]);

  void MetaGame(size_t meta_time);

  virtual AMove SelectMovePolicy();

  std::string SelectMove(size_t play_time);

  void ApplyMove(const MoveType& m);

  void StopGame(const MoveType& m);

  const std::string Name() const
  {
    return "MCTS";
  }

 protected:
  AlgoConfig algo_config;
  NodeType* current_node;
  size_t role_size;
  AlgorithmType algo;
  AState init_state;
};

}
}
}


#endif // LIBGDL_PLAYER_MCTS_PLAYER_HPP_INCLUDED
