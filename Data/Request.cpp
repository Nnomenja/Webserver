#include "Request.hpp"
#include "../Core/Request/RequestParserState/MethodParser.hpp"
#include "../Core/Request/RequestParserState/ARequestParserState.hpp"

#include <iostream>

Request::Request():_parserState(new MethodParser()){
  _i++;
  std::cout << "Request: " << _i << std::endl;
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

void Request::setParseState(ARequestParserState *state)
{
    _parserState = state;
}

int Request::_i = 0;