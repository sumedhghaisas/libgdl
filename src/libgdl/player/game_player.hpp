#ifndef LIBGDL_PLAYER_GAME_PLAYER_HPP_INCLUDED
#define LIBGDL_PLAYER_GAME_PLAYER_HPP_INCLUDED

#include "default_player_config.hpp"

namespace libgdl
{
namespace player
{

template<class SMT, class CT = DefaultPlayerConfig>
class GamePlayer
{
 public:
  typedef SMT StateMachineType;
  typedef typename StateMachineType::StateType StateType;
  typedef typename StateMachineType::MoveType MoveType;

  template<typename... Args>
  GamePlayer(Args... args)
    : state_machine(args...) {}

  void SetRole(const std::string& r)
  {
    role = r;
    role_id = state_machine.GetRoleID(role);
  }

  const size_t RoleID() const
  {
    return role_id;
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

  std::string role;
  size_t role_id;
};

}
}


#endif // LIBGDL_PLAYER_GAME_PLAYER_HPP_INCLUDED
