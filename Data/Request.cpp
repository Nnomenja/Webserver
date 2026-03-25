#include "Request.hpp"
#include "../Core/Request/RequestParserState/MethodParser.hpp"
#include "../Core/Request/RequestParserState/ARequestParserState.hpp"

#include <iostream>

Request::Request():_index(0){
	_parserState = new MethodParser(this);
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

ARequestParserState *Request::getParserState() const
{
  return (_parserState);
}

size_t Request::getParserIndex() const
{
	return (_index);
}

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
}