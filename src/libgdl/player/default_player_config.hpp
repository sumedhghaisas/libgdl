#ifndef LIBGDL_PLAYER_DEFAULT_PLAYER_CONFIG_HPP_INCLUDED
#define LIBGDL_PLAYER_DEFAULT_PLAYER_CONFIG_HPP_INCLUDED

namespace libgdl
{
namespace player
{

struct DefaultPlayerConfig
{
  static const size_t select_move_safety_time = 500000;
  static const size_t meta_game_safety_time = 500000;

  static size_t SelectMoveSafetyTime()
  {
    return select_move_safety_time;
  }

  static size_t MetaGameSafetyTime()
  {
    return meta_game_safety_time;
  }
};

}
}


#endif // LIBGDL_PLAYER_DEFAULT_PLAYER_CONFIG_HPP_INCLUDED
