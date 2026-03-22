#include "Request.hpp"

Request::Request():_state(METHOD){
};

Request::Request(const Request& other){
  *this = other; 
};

Request& Request::operator=(const Request& other){
  if (this != &other) {
  }
  return *this;
};

Request::~Request(){};


RequestParserState Request::getParseState() const
{
  return (_state);
}

void Request::setParseState(RequestParserState value)
{
    _state = value;
}