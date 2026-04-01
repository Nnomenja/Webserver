#include "HttpRequestParser.hpp"
#include "./RequestParserState/ARequestParserState.hpp"
#include "./RequestParserState/UriParser.hpp"
#include "./RequestParserState/MethodParser.hpp"
#include "./RequestParserState/HttpVersionParser.hpp"
#include "./RequestParserState/HeaderParser.hpp"
#include "./RequestParserState/BodyParser.hpp"

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


void HttpRequestParser::parse(Request *req, UnitConf_t endpoint)
{
	ARequestParserState	*parseState = req->getParserState();
	RequestParserStateName name;
	if (parseState == NULL)
	{
		req->setParseState(new MethodParser(req, endpoint));
		name  = METHOD;
		parseState = req->getParserState();
	}
	else
		name = parseState->getParserStateName();
	try
	{
		switch (name)
		{
			case METHOD:
				parseState->execute();
				req->setParseState(new UriParser(req, endpoint));
				//fallthrough
			case URI :
				req->getParserState()->execute();
				req->setParseState(new HttpVersionParser(req, endpoint));
				//fallthrough
			case VERSION :
				req->getParserState()->execute();
				req->setParseState(new HeaderParser(req, endpoint));
				//fallthrough
			case  HEADER:
				req->getParserState()->execute();
				req->setParseState(new BodyParser(req, endpoint));
				//fallthrough
			case BODY:
				req->getParserState()->execute();
				//fallthrough
			default:
				req->setParseState(NULL);
				_finished = true;
				break;
		}
	}
	catch(const ARequestParserState::EagainParser& e)
	{
		std::cout << e.what() << std::endl;
		return;
	}
}

bool HttpRequestParser::finished()
{
  return (_finished);
}