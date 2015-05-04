#ifndef LIBGDL_CONNECTION_GAME_CONNECTION_HPP_INCLUDED
#define LIBGDL_CONNECTION_GAME_CONNECTION_HPP_INCLUDED

#include <libgdl/core.hpp>

#include "http_server.hpp"
#include "request.hpp"

namespace libgdl
{
namespace connection
{

class GameConnection
{
 public:
  GameConnection(Log log = GLOBAL_LOG) {}

  int Open(size_t port);

  Request* GetNextRequest();

  bool SendResponse(const std::string& data);

  size_t GetPortNumber() const
  {
    return http_server.GetPortNumber();
  }

  bool IsOpen() const
  {
    return http_server.IsOpen();
  }

 private:
  HttpServer http_server;
  Log log;
};

}
}

#endif // LIBGDL_CONNECTION_GAME_CONNECTION_HPP_INCLUDED
