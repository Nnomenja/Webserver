#include "Request.hpp"
#include "../Core/Request/RequestParserState/MethodParser.hpp"
#include "../Core/Request/RequestParserState/ARequestParserState.hpp"

#include <iostream>

Request::Request():_index(0), _parserState(NULL), _method(GET){

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
	_buffer_size = value.length();
}


void Request::setMethod(HttpMethod value)
{
	_method = value;
}