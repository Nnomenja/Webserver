#include "HttpRequestParser.hpp"
#include "./RequestParserState/ARequestParserState.hpp"
#include "./RequestParserState/UriParser.hpp"

HttpRequestParser::HttpRequestParser():_finished(false){


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


void HttpRequestParser::parse(Request *req)
{
	ARequestParserState	*parseState = req->getParserState(); 
	RequestParserStateName name = parseState->getParserStateName(); 
	switch (name)
	{
		case METHOD:
			parseState->execute();
			req->setParseState(new UriParser(req));
			//fallthrough
		case URI :
			req->getParserState()->execute();
			//fallthrough
		default:
			req->setParseState(NULL);
			_finished = true;
			break;
	}
  (void)req;
}

bool HttpRequestParser::finished()
{
  return (_finished);
}