#include "Client.hpp"
#include "../utils/utils.hpp"

Client::Client(){
  this->_start_time_ms = get_time_ms();
};

Client::Client(const Client& other){
  *this = other;
};

Client& Client::operator=(const Client& other){
  if (this != &other) {
    _clientInfo = other._clientInfo;
    _fd = other._fd;
    _buffer = other._buffer;
    _start_time_ms = other._start_time_ms;
  }
  return *this;
};

Client::~Client(){};

/**============================================
 *               GETTERS
 *=============================================**/

SocketInfo Client::getCLientInfo() const
{
  return (_clientInfo);
}

int Client::getFd() const
{
  return (_fd);
}
std::string Client::getBuffer() const
{
  return (_buffer);
}

uint64_t Client::getStartTime() const
{
  return (_start_time_ms);
}

EndpointType Client::getEndpointType() const
{
  return (_type);
}

/**============================================
 *               SETTERS
 *=============================================**/

void Client::setClientInfo(SocketInfo value)
{
  _clientInfo = value;
}

void Client::setEndpointType(EndpointType type)
{
  _type = type;
}
/**============================================
 *               UTILS
 *=============================================**/

 void Client::refreshStartTime()
{
  _start_time_ms =  get_time_ms();
}
