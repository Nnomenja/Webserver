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

UnitConf_t Client::getEndpoint() const
{
  return (_endpoint);
}

size_t Client::getBufferSize() const
{
	return (_bufferSize);
}

/**============================================
 *               SETTERS
 *=============================================**/

void Client::setEndpoint(UnitConf_t value)
{
  _endpoint = value;
}
void Client::setEndpointType(EndpointType type)
{
  _endpoint.type = type;
}

void Client::setBuffer(std::string &value)
{
	_buffer = value;
}

void Client::setBufferSize(size_t value)
{
	_bufferSize = value;
}

void Client::setFd(int fd)
{
	_fd = fd;
}

/**============================================
 *               UTILS
 *=============================================**/

 void Client::refreshStartTime()
{
  _start_time_ms =  get_time_ms();
}
