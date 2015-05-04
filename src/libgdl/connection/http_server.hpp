#ifndef LIBGDL_CONNECTION_HTTP_SERVER_HPP_INCLUDED
#define LIBGDL_CONNECTION_HTTP_SERVER_HPP_INCLUDED

#include <libgdl/core.hpp>

// c++ header
#include <cstdio>
#include <cstdlib>
#include <string>

// c header
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>

namespace libgdl
{
namespace connection
{

class HttpServer
{
 public:
  // constructor and destructor
  HttpServer(Log log = GLOBAL_LOG)
    : log(log) {}

  ~HttpServer();

  int Open(size_t port);

  void Close();

  bool IsOpen() const
  {
    return isOpen;
  }

  /**
   * /brief Waits (is blocking) for an incoming http-request and set up the connection,
   * should not be called again until sendResponse is called
   * /return Returns the content of the accepted request without the http-header
   */
  const std::string AcceptRequest(void);

  /**
   * /brief Send a response to the via acceptRequest connected client,
   * should be called only after a acceptRequest call
   * /param response The content of the response with http-header
   */
  bool SendResponse(const std::string& message);

  size_t GetPortNumber() const
  {
    return port;
  }

private:
  size_t port;
  // socket for new connections
  int socket_fd = -1;

  // socket for current connection
  int connection_fd = -1;

  // clients address information
  struct sockaddr_in clientAddressInfo;

  // waits for a client to connect
  void waitForConnection();

  // receive (part of) the message from current connection
  int receive(char *buffer, int length);

  bool isOpen = false;

  mutable Log log;
};

}
}


#endif // LIBGDL_CONNECTION_HTTP_SERVER_HPP_INCLUDED
