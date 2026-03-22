#include "HttpRequestParser.hpp"

HttpRequestParser::HttpRequestParser(){


};

HttpRequestParser::HttpRequestParser(const HttpRequestParser& other){
  *this = other;
};

HttpRequestParser& HttpRequestParser::operator=(const HttpRequestParser& other){
  if (this == &other)
    return (*this);
  return *this;
};

HttpRequestParser::~HttpRequestParser(){
};


void HttpRequestParser::parse(Request &req)
{
	(void)req;
  req.setParseState(COMPLETE);
}