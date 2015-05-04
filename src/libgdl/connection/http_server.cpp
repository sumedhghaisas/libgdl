#include "http_server.hpp"

using namespace std;
using namespace libgdl::connection;

#include <iostream>
#include <cstdlib>
#include <sstream>
#include <cstring>

int HttpServer::Open(size_t port)
{
  port = port;

  // init socket
	if((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
		log.Warn << LOGID << "Could not set up socket." << endl;
    return -1;
	}

	// set socket options
	int byte=1;
	if (setsockopt(socket_fd,SOL_SOCKET,SO_REUSEADDR,&byte,sizeof(int)) == -1)
  {
		log.Warn << LOGID << "Could not setup socket options." << endl;
		return -2;
	}

	struct sockaddr_in server_address_info;
	memset(&server_address_info, '\0', sizeof(sockaddr_in)); // zero the struct
	server_address_info.sin_family = AF_INET;         // host byte order
	server_address_info.sin_port = htons(port);     // short, network byte order
	server_address_info.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP

	// bind socket
	if(bind(socket_fd, (struct sockaddr *)&server_address_info, sizeof(struct sockaddr)) == -1)
	{
		log.Warn << LOGID << "Could not bind socket with addressinfo." << endl;
		return -3;
	}

	// listen to socken
	if (listen(socket_fd, 1) == -1)
	{
		log.Warn << LOGID << "Error in listening to socket." << endl;
		return -4;
	}

	isOpen = true;
	return 1;
}

void HttpServer::Close()
{
  if(isOpen)
  {
    if(connection_fd != -1) close(connection_fd);
    if(socket_fd != -1) close(socket_fd);
    isOpen = false;
  }
}

HttpServer::~HttpServer()
{
	if(connection_fd != -1) close(connection_fd);
	if(socket_fd != -1) close(socket_fd);
}

const std::string HttpServer::AcceptRequest(void)
{
  if(isOpen)
  {
    // wait for connection, which has to be established before receiving anything
    waitForConnection();

    // get HTTP message, extract content
    std::string message="";
    int numbytes;  // length of the received data
    char buffer[8193];

    // receive until we have the complete http-header
    std::string::size_type headerLength;
    do
    {
      numbytes=receive(buffer, 8192);
      buffer[numbytes]='\0';
      message+=buffer;
    } while((headerLength = message.find("\r\n\r\n")) == std::string::npos);

    headerLength+=4;

    // extract the content length from the header
    std::string::size_type contentLengthStart;
    if((contentLengthStart=message.find("\r\nContent-length:")) == std::string::npos)
    {
      contentLengthStart=message.find("\r\nContent-Length:");
    }

    if(contentLengthStart == std::string::npos)
    {
      std::cout << "message has no content length field:" << std::endl << message << std::endl;
      close(connection_fd); // close child socket
      connection_fd =- 1;
      return "invalid message";
    }
    else
    {
      contentLengthStart+=18;
      std::string::size_type contentLengthEnd = message.find("\r\n", contentLengthStart + 1);
      std::string contentLengthStr = message.substr(contentLengthStart, contentLengthEnd - contentLengthStart);
      unsigned int messageLength = atoi(contentLengthStr.c_str()) + headerLength;

      // receive remaining message
      while(message.length()<messageLength)
      {
        numbytes=receive(buffer, 8192);
        buffer[numbytes]='\0';
        message+=buffer;
      }

      // return the content of the message without http header
      message=message.substr(headerLength);
      return message;
    }
  }
  else
  {
    log.Warn << LOGID << "Server connection is not open for request." << endl;
  }

  return "";
}

int HttpServer::receive(char *buffer, int length)
{
	int numbytes;
	if((numbytes = recv(connection_fd, (void*)buffer, length, 0)) == -1)
  {
		log.Warn << LOGID << "Error in receiving. Closing the connection on port "
             << port << "." << endl;
		Close();
		return -1;
	}
	return numbytes;
}

bool HttpServer::SendResponse(const std::string& message)
{
	std::stringstream response;
	response << "HTTP/1.0 200 OK\r\n";
	response << "Content-type: text/acl\r\n";
	response << "Content-length: " << message.length();
	response << "\r\n\r\n";
	response << message;
	std::string responseStr=response.str();
	if ( send(connection_fd, responseStr.c_str(), responseStr.length(), 0) == -1)
  {
    log.Warn << LOGID << "Error sending message: " << message << std::endl;
    return false;
	}
	close(connection_fd); // close child socket
	connection_fd=-1;
	return true;
}

void HttpServer::waitForConnection()
{
	int sin_size = sizeof(struct sockaddr_in);
	if((connection_fd = accept(socket_fd, (struct sockaddr *)&clientAddressInfo, (socklen_t*)&sin_size)) == -1)
  {
		perror("accepting client");
		exit(1);
	}
}
