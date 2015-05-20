#ifndef LIBGDL_PLAYER_GAME_PLAYER_HPP_INCLUDED
#define LIBGDL_PLAYER_GAME_PLAYER_HPP_INCLUDED

#include "default_player_config.hpp"

namespace libgdl
{
namespace player
{

template<class PT, class SMT, class CT = DefaultPlayerConfig>
class GamePlayer
{
 public:
  typedef PT PlayerType;
  typedef SMT StateMachineType;
  typedef typename StateMachineType::StateType StateType;
  typedef typename StateMachineType::MoveType MoveType;

  template<typename... Args>
  GamePlayer(Args... args)
    : state_machine(args...)
  {

  }

  void MetaGame(std::string game_desc, size_t metaGame)
  {
    static_cast<PlayerType*>(this)->MetaGame(metaGame);
  }

  void SetRole(const std::string& r)
  {
    current_role = r;
    current_role_id = state_machine.GetRoleID(current_role);
  }

  const size_t RoleID() const
  {
    return current_role_id;
  }

  inline StateMachineType& GetStateMachine()
  {
    return state_machine;
  }

  CT& Config() const
  {
    return CT::Global();
  }

 private:
  StateMachineType state_machine;
  StateType state;

  std::string current_role;
  size_t current_role_id;
};

}
}


#endif // LIBGDL_PLAYER_GAME_PLAYER_HPP_INCLUDED
