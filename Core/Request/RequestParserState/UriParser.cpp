#include "UriParser.hpp"


UriParser::UriParser(Request *target):ARequestParserState(URI, target){
};
UriParser::~UriParser(){
};

bool UriParser::completed() const
{
	return (_finished);
}

void UriParser::execute() const
{
	std::cout << "Uri executing..." << std::endl;
}
