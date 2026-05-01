#include "Client.hpp"
#include "../utils/utils.hpp"
#include <algorithm>

Client::Client(std::map<std::string, std::string> &envs):_start_time_ms(get_time_ms()), _req(new Request()), _res(new Response()), _parsed(false),_envs(envs)
{
	this->_start_time_ms = get_time_ms();
	_req->setRoot(_endpoint.root);
	_bufferSize = 0;
	_buffer = "";
	_cgi_pid = -1;
	_cgi_output = -1;
	_cgi_processing = false;
	_cgi_processing_end = false;
	_cgi_output_readed = false;
};

Client::Client(const Client& other):_envs(other._envs){
	_fd = other._fd;
    _buffer = other._buffer;
    _start_time_ms = other._start_time_ms;
};

Client& Client::operator=(const Client& other){
  if (this != &other) {
    _fd = other._fd;
    _buffer = other._buffer;
    _start_time_ms = other._start_time_ms;
  }
  return *this;
};

Client::~Client(){
	
	_logger.setMethod(_req->getMethodString());
	_logger.setIp(_ip);
	_logger.setPath(_req->getPathname());
	_logger.setStatus(_res->getStatusCode());
	_logger.setResponseTime(get_time_ms() - _start_time_ms);
	if (_res->getStatusCode() != 0)
		_logger.log();

	if (_req)
		delete  _req;
	if (_res)
		delete _res;
	::close(_fd);
};

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

int Client::getCGIfd() const
{
	return (_cgi_output);
}

std::string Client::getCGIOutput() const
{
	return (_res->getCgiResponse());
}

pid_t Client::getCGIPid() const
{
	return (_cgi_pid);
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

const std::map<std::string, std::string> &Client::getEnv() const
{
	return (_envs);
}

std::string Client::getCGIbinByExtension(std::string ext) const
{
	if (_req->getLocation().CGI.find(ext) != _req->getLocation().CGI.end())
		return (_req->getLocation().CGI[ext]);
	return ("");
}

void Client::setEndpoint(UnitConf_t value)
{
  _endpoint = value;
}
void Client::setLocationType(LocationType type)
{
	_req->setLocationType(type);
}

void Client::setBuffer(std::string value)
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

void	Client::setCGIInfo(pid_t pid, int output_fd)
{
	_cgi_pid = pid;
	_cgi_output = output_fd;
}

bool Client::isCGI() const
{
	return (_cgi_pid != -1 && _cgi_output != -1);
}
void Client::setEnv(std::string key, std::string value)

{
	_envs[key] = value;
}

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
	std::stringstream statusCode;

	statusCode << _res->getStatusCode();
	_buffer += "HTTP/1.1 ";
	_buffer += statusCode.str();
	_buffer += " ";
	_buffer += _res->getStatusMessage();
	_buffer += "\r\n";

	std::map<std::string, std::string>::const_iterator it =  _res->getHeaders().begin();
	while (it != _res->getHeaders().end())
	{
		_buffer += it->first + ": "+ it->second + "\r\n";
		++it;
	}
	_buffer += "\r\n";
	_buffer += _res->getBody();
	_bufferSize = _buffer.size();
}

bool Client::isProcessingCGI() const
{
	return (_cgi_processing);
}

void Client::endProcessingCGI()
{
	_cgi_processing = false;
}


void Client::setProcessingCGI(bool value)
{
	_cgi_processing = value;
}

void Client::setCGIbin(std::string &value)
{
	_cgi_bin = value;
}

std::string Client::getCGIbin() const
{
	return (_cgi_bin);
}

void Client::setCGIOutputReaded()
{
	_cgi_output_readed = true;
}

bool Client::isCGIOutputReaded() const
{
	return (_cgi_output_readed);
}

bool Client::isCGIProcessEnd()
{
	return (_cgi_processing_end);
}

void Client::setCGICGIProcessEnd()
{
	_cgi_processing_end = true;
}

// NEW ->
std::string Client::my_inet_ntoa(sockaddrIn addr) {
    unsigned long ip = ntohl(addr.sin_addr.s_addr);

    std::ostringstream oss;
    oss << ((ip >> 24) & 0xFF) << "."
        << ((ip >> 16) & 0xFF) << "."
        << ((ip >> 8) & 0xFF) << "."
        << (ip & 0xFF);

    return oss.str();
}

void Client::setIp(sockaddrIn addr) {
    _ip = my_inet_ntoa(addr);
}

std::string         Client::getIp() const
{
	return _ip;
}

RequestLogger &Client::getLogger()
{
	return _logger;
}
