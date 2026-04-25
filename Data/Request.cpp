#include "Request.hpp"
#include "../Core/HTTP/Request/RequestParserState/MethodParser.hpp"
#include "../Core/HTTP/Request/RequestParserState/ARequestParserState.hpp"

#include <algorithm>

Request::Request():_index(0), _parserState(NULL), _method(GET){
	_body._content_length = 0;
};

Request::Request(const Request& other){
  std::cout << "Copy" << std::endl;
  *this = other; 
};

Request& Request::operator=(const Request& other){
  std::cout << "Copy assignment" << std::endl;
  if (this != &other) {
  }
  return *this;
};

Request::~Request(){
	delete _parserState;
};

/**============================================
 *               GETTERS
 *=============================================**/

ARequestParserState *Request::getParserState() const
{
  return (_parserState);
}

size_t Request::getParserIndex() const
{
	return (_index);
}

std::string Request::getBuffer() const
{
  return (_buffer);
}

size_t Request::getBufferSize() const
{
	return (_buffer_size);
}

HttpMethod Request::getMethod() const
{
	return (_method);
}

std::string Request::getMethodString() const
{
	switch (_method)
	{
		case GET:
			return ("GET");
		case POST:
			return ("POST");
		case DELETE:
			return ("DELETE");
		default:
			return ("");
	}
}

const std::string &Request::getPathname() const
{
	return (_pathname);
}


std::string Request::getQuery() const
{
	return (_query);
}

std::map<std::string, std::string> Request::getHeaders() const
{
	return (_headers);
}


const std::string &Request::getHeaderBykey(std::string key)
{
	return (_headers[key]);	
}

t_body_encode Request::getBodyEncode() const
{
	return (_body_encode);
}

long Request::getContentLength() const
{
	return (_body._content_length);
}

t_body &Request::getBody()
{
	return (_body);
}

size_t           Request::getBodySize() const
{
	return (_body._content_length);
}

t_location Request::getLocation() const
{
	return (_location);
}


const std::string &Request::getLocationDefaultIndex() const
{
	return (_location.index);
}

std::string Request::getFullPath() const
{
	return (_fullpath);
}

std::string Request::getRootDir() const
{
	if (_location.root.empty())
		return (_root);
	return (_location.root);
}

std::string Request::getPathExtension() const
{
	return ("." + _fullpath.substr(_fullpath.find_last_of(".") + 1));
}

const std::string &Request::getServerName() const
{
	return (_server_name);
}

/**============================================
 *               SETTERS
 *=============================================**/

void Request::incrementParserIndex()
{
	_index++;
}
void Request::resetParserIndex()
{
	_index = 0;
}

void Request::setParseState(ARequestParserState *state)
{
	if (_parserState)
		delete _parserState;
    _parserState = state;
}
void Request::setBuffer(std::string &value)
{
	_buffer = value;
	_buffer_size = value.size();
}


void Request::setMethod(HttpMethod value)
{
	_method = value;
}

void Request::setPathname(std::string value)
{
	_pathname = value;
}

void Request::setFullPath(std::string value)
{
	_fullpath = value;
}

void Request::setRoot(std::string value)
{
	_root = value;
}

void Request::addPathname(char c)
{
	_pathname.push_back(c);
}

void Request::addQuery(char c)
{
	_query.push_back(c);
}

void Request::setBody(std::string value)
{
	_body._str_buffer = value;
}

std::string &Request::toLowerCase(std::string &src) const
{
	std::transform(src.begin(), src.end(), src.begin(), ::tolower);
	return (src);
}

void Request::setHeader(std::string key, std::string value)
{
	_headers[toLowerCase(key)] = value;
}

bool Request::hasHeader(std::string value) const
{
	return (_headers.find(value) != _headers.end());
}

void Request::setContenLength(long value)
{
	_body._content_length = value;
}

void Request::setBodyEncode(t_body_encode value)
{
	_body_encode = value;
}

void Request::addBody(char c, bool inc)
{
	if (_body._str_buffer.size() < BODY_BUFFER_SIZE_MAX)
		_body._str_buffer.push_back(c);
	else
	{
		if (!_body._file_buffer.is_open())
			_body._file_buffer.open("body_tmp", std::ios::out | std::ios::in | std::ios::trunc);
		_body._file_buffer.put(c);
	}
	if (inc)
		 _body._content_length++;
}

void Request::setLocation(t_location &value)
{
	_location = value;
}
void Request::setLocationType(LocationType value)
{
	_location.type = value;
}
void Request::setRedirectionPath(std::string &value, int code)
{
	_location.ret.target = value;
	_location.ret.code = code;
}

void Request::setServerName(std::string value)
{
	_server_name = value;
}