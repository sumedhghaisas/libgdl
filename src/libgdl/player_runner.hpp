#ifndef LIBGDL_PLAYER_RUNNER_HPP_INCLUDED
#define LIBGDL_PLAYER_RUNNER_HPP_INCLUDED

#include <libgdl/core.hpp>
#include <libgdl/connection/game_connection.hpp>

namespace libgdl
{

template<class Player>
class PlayerRunner
{
  typedef typename Player::MoveType MoveType;

 public:
  template<typename... Args>
  PlayerRunner(size_t port, Log log, Args... args);

  void Run();

 private:
  connection::GameConnection game_connection;
  Player player;

  mutable Log log;
};

}

#include "player_runner_impl.hpp"

#endif // LIBGDL_PLAYER_RUNNER_HPP_INCLUDED
