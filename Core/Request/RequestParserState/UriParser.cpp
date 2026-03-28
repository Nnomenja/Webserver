#include "UriParser.hpp"


UriParser::UriParser(Request *target, UnitConf_t endpoint):ARequestParserState(URI, target, endpoint){
};
UriParser::~UriParser(){
};

void UriParser::execute()
{
	std::cout << "Uri executing..." << std::endl;
}
