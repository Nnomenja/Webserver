#include "UriParser.hpp"


UriParser::UriParser(Request *target, UnitConf_t endpoint):ARequestParserState(URI, target, endpoint){
};
UriParser::~UriParser(){
};

bool UriParser::completed() const
{
	return (_finished);
}

void UriParser::execute()
{
	std::cout << "Uri executing..." << std::endl;
}
