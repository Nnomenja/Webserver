#include "HttpRequestParser.hpp"
#include "./RequestParserState/ARequestParserState.hpp"

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
//   switch (req.getParserState().)
//   {
	
//   }
	std::cout << "I: " << req->getI() << std::endl;
	if (req->getI() == 0)
		_finished = true;
	else
	{
		std::cout << "Decrementing.."<< std::endl;
		// req.decrement();
	}
  // if parsing is compled
  (void)req;
}

bool HttpRequestParser::finished()
{
  return (_finished);
}