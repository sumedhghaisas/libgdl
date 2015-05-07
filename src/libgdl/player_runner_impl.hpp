#include "player_runner.hpp"

#include <libgdl/connection/request.hpp>

using namespace std;
using namespace libgdl;
using namespace libgdl::connection;

template<class Player>
template<typename... Args>
PlayerRunner<Player>::PlayerRunner(size_t port, Log log, Args... args)
  : player(args...)
{
  log.Info << "Player initialized." << endl;
  size_t current_port = 9147;
  while(true)
  {
    log.Info << "Setting up player on port " << current_port << "." << endl;
    if(game_connection.Open(current_port) > 0)
      break;
    current_port++;
  }
  log.Info << "Connection setup successful." << endl;
}

template<class Player>
void PlayerRunner<Player>::Run()
{
  if(!game_connection.IsOpen())
  {
    log.Warn << LOGID << "Game connection is not setup properly." << endl;
    return;
  }

  Request* current_request = NULL;
  while(!(current_request = game_connection.GetNextRequest())->IsStartRequest());
  StartRequest* start_request = static_cast<StartRequest*>(current_request);
  bool isGameOn = true;
  string match_id = start_request->MatchID();
  player.SetRole(start_request->Role());
  player.MetaGame(start_request->MetaTime());
  current_request = game_connection.GetNextRequest();
  while(isGameOn)
  {
    std::string res = player.SelectMove(start_request->PlayTime());
    std::cout << "Sending res.." << std::endl;
    game_connection.SendResponse(res);
    current_request = game_connection.GetNextRequest();
    if(current_request->IsPlayRequest() &&
       static_cast<PlayRequest*>(current_request)->MatchID() != match_id)
    {
      log.Info << "Ignoring request. Waiting for play/stop request with match id: "
               << match_id << std::endl;
      continue;
    }

    if(current_request->IsStopRequest() &&
       static_cast<StopRequest*>(current_request)->MatchID() != match_id)
    {
      log.Info << "Ignoring request. Waiting for play/stop request with match id: "
               << match_id << std::endl;
      continue;
    }

    if(!current_request->IsPlayRequest() && !current_request->IsStopRequest())
    {
      log.Info << "Ignoring request. Waiting for play/stop request with match id: "
               << match_id << std::endl;
      continue;
    }

    if(current_request->IsStopRequest())
    {
      StopRequest* stop_request = static_cast<StopRequest*>(current_request);
      MoveType move = MoveType::Create(stop_request->Move());
      player.StopGame(move);
      break;
    }

    PlayRequest* play_request = static_cast<PlayRequest*>(current_request);
    MoveType move = MoveType::Create(play_request->Move());
    player.ApplyMove(move);
  }
}
