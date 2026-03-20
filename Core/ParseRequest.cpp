#include "ParseRequest.hpp"
#include "../Data/Request.hpp"

ParseRequest::ParseRequest(){
};

ParseRequest::ParseRequest(int header_timeout, int body_timeout, int header_field_limit, int uri_limit)
:_header_timeout(header_timeout), _body_timeout(body_timeout), _header_field_limit(header_field_limit),
_uri_limit(uri_limit)
{

}

ParseRequest::ParseRequest(const ParseRequest& other){
  *this = other;
};

ParseRequest& ParseRequest::operator=(const ParseRequest& other){
  if (this != &other) {
  _header_timeout = other._header_timeout;
  _body_timeout = other._body_timeout;
  _header_field_limit = other._header_field_limit;
  _uri_limit = other._uri_limit;
  }
  return *this;
};

ParseRequest::~ParseRequest(){};

void ParseRequest::parse(Request &req) const
{

}
