#include "MethodParser.hpp"


MethodParser::MethodParser(Request *target):ARequestParserState(METHOD, target){
};
MethodParser::~MethodParser(){
};

bool MethodParser::completed() const
{
	return (_finished);
}

void MethodParser::execute() const
{
	std::cout << "MethodParser executing..." << std::endl;
}
