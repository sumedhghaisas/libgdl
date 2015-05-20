#ifndef LIBGDL_CONNECTION_REQUEST_HPP_INCLUDED
#define LIBGDL_CONNECTION_REQUEST_HPP_INCLUDED

#include <string>
#include <list>

namespace libgdl
{
namespace connection
{

struct Request
{
  enum class Type{START, PLAY, STOP};

  Request(const Type& t, const std::string& match_id)
    : t(t) {}

  bool IsStartRequest() const
  {
    if(t == Type::START)
      return true;
    return false;
  }

  bool IsPlayRequest() const
  {
    if(t == Type::PLAY)
      return true;
    return false;
  }

  bool IsStopRequest() const
  {
    if(t == Type::STOP)
      return true;
    return false;
  }

  const std::string& MatchID() const
  {
    return match_id;
  }

  Type t;
  std::string match_id;
};

struct StartRequest : public Request
{
  StartRequest(const std::string& match_id,
               const std::string& role,
               const std::string& game_desc,
               size_t meta_time,
               size_t play_time)
    : Request(Type::START, match_id), role(role),
    game_desc(game_desc), meta_time(meta_time), play_time(play_time)
  {
  }

  std::string GameDesc() const
  {
    return game_desc;
  }

  size_t MetaTime() const
  {
    return meta_time;
  }

  size_t PlayTime() const
  {
    return play_time;
  }

  const std::string& Role() const
  {
    return role;
  }

  std::string role;
  std::string game_desc;
  size_t meta_time;
  size_t play_time;
};

struct PlayRequest : public Request
{
  PlayRequest(const std::string& match_id, std::list<std::string>& str_move)
    : Request(Type::PLAY, match_id), str_move(str_move) {}

  const std::list<std::string>& Move() const
  {
    return str_move;
  }

  std::list<std::string> str_move;
};

struct StopRequest : public Request
{
  StopRequest(const std::string& match_id, std::list<std::string>& str_moves)
    : Request(Type::STOP, match_id), str_moves(str_moves) {}

  const std::list<std::string>& Move() const
  {
    return str_moves;
  }

  std::list<std::string> str_moves;
};

}
}

#endif // LIBGDL_CONNECTION_REQUEST_HPP_INCLUDED
