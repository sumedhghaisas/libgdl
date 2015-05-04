#include "game_connection.hpp"

#include <vector>
#include <sstream>
#include <algorithm>
#include <boost/tokenizer.hpp>

#include <libgdl/core/data_types/argument.hpp>

using namespace std;
using namespace libgdl::connection;
using namespace libgdl::core;

int GameConnection::Open(size_t port)
{
  return http_server.Open(port);
}

Request* GameConnection::GetNextRequest()
{
  if(!http_server.IsOpen())
  {
    log.Warn << LOGID << "Server is not open for requests. Use \
                          GameConnection::Open(port) to open the connection."
                      << endl;
    return NULL;
  }
  string str_req = http_server.AcceptRequest();

  cout << str_req << endl;

  string cmd;
  vector<string> args;
  bool succ = Argument::SeparateCommand(str_req, cmd, args, log);

  Request* out = NULL;
  if(cmd == "START")
  {
    size_t meta_time;
    size_t play_time;
    stringstream stream;
    stream << args[3] << " " << args[4];
    stream >> meta_time;
    stream >> play_time;
    std::transform(args[1].begin(), args[1].end(), args[1].begin(), ::tolower);
    out = new StartRequest(args[0], args[1], args[2], meta_time, play_time);
  }
  else if(cmd == "PLAY")
  {
    list<string> str_move;
    cout << "move is " << args[1] << endl;
    string move_cmd;
    vector<string> move_args;
    std::transform(args[1].begin(), args[1].end(), args[1].begin(), ::tolower);
    Argument::SeparateCommand(args[1], move_cmd, move_args);

    str_move.push_back(move_cmd);
    for(auto it : move_args)
    {
      str_move.push_back(it);
    }

    out = new PlayRequest(args[0], str_move);
  }
  else if(cmd == "STOP")
  {
    list<string> str_move;
    cout << "move is " << args[1] << endl;
    string move_cmd;
    vector<string> move_args;
    std::transform(args[1].begin(), args[1].end(), args[1].begin(), ::tolower);
    Argument::SeparateCommand(args[1], move_cmd, move_args);

    str_move.push_back(move_cmd);
    for(auto it : move_args)
    {
      str_move.push_back(it);
    }

    out = new StopRequest(args[0], str_move);
  }

  return out;
}

bool GameConnection::SendResponse(const std::string& data)
{
  return http_server.SendResponse(data);
}
