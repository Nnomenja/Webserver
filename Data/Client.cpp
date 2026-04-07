#include "Client.hpp"
#include "../utils/utils.hpp"
#include <algorithm>

Client::Client():_start_time_ms(get_time_ms()), _req(new Request()), _res(new Response()), _parsed(false){
  std::cout << "Create client" << std::endl;
//   this->_start_time_ms = get_time_ms();
};

Client::Client(const Client& other){
	std::cout << "Copy client" << std::endl;
	_fd = other._fd;
    _buffer = other._buffer;
    _start_time_ms = other._start_time_ms;
	// *this = other;
};

Client& Client::operator=(const Client& other){
  if (this != &other) {
    _fd = other._fd;
    _buffer = other._buffer;
    _start_time_ms = other._start_time_ms;
    // req = other.req;
    // res = other.res;
  }
	std::cout << "Copy Assignmnt" << std::endl;
  return *this;
};

Client::~Client(){
	if (_req)
		delete  _req;
	if (_res)
		delete _res;
};

/**============================================
 *               GETTERS
 *=============================================**/

int Client::getFd() const
{
  return (_fd);
}
std::string Client::getResponseHttp() const
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

size_t Client::getResponseHttpSize() const
{
	return (_bufferSize);
}

Request *Client::getRequest()
{
	return (_req);
}

Response *Client::getResponse()
{
	return (_res);
}


std::string findDefaultErrorPagePathBySource(int code, const std::vector<t_error_page> &error_pages)
{
	std::vector<t_error_page>::const_iterator it = error_pages.begin();
	while (it != error_pages.end())
	{
		if (std::find(it->codes.begin(), it->codes.end(), code) != it->codes.end())
			return (it->path);
		++it;
	}
	return ("");
}

std::string Client::getDefaultErrorPagePath(int code) const
{
	std::string path;

	path = findDefaultErrorPagePathBySource(code, _req->getLocation().error_pages);
	if (path.size())
		return (_req->getLocation().root + path);
	path = findDefaultErrorPagePathBySource(code, _endpoint.error_pages);
	if (path.size())
		return (_endpoint.root + path);
	return ("");
}

/**============================================
 *               SETTERS
 *=============================================**/

void Client::setEndpoint(UnitConf_t value)
{
  _endpoint = value;
}
void Client::setLocationType(LocationType type)
{
	_req->setLocationType(type);
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


bool Client::isParsed() const
{
	return (_parsed);
}

void Client::parsed()
{
	_parsed = true;
}


void	Client::generateResponse()
{
	/*==== RESPONSE LINE ====*/
	std::stringstream statusCode;

	statusCode << _res->getStatusCode();
	_buffer += "HTTP/1.1 ";
	_buffer += statusCode.str();
	_buffer += " ";
	_buffer += _res->getStatusMessage();
	_buffer += "\r\n";

	/*==== RESPONSE HEADERS ====*/
	std::map<std::string, std::string>::const_iterator it =  _res->getHeaders().begin();
	while (it != _res->getHeaders().end())
	{
		_buffer += it->second + "\r\n";
		++it;
	}
	_buffer += "\r\n";
	/*==== RESPONSE BODY ====*/
	_buffer += _res->getBody();
	_bufferSize = _buffer.size();
	// delete _req;
	// delete _res;
}