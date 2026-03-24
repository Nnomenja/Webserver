#include "MethodParser.hpp"


MethodParser::MethodParser():ARequestParserState(METHOD), _i(0){
	_i++;
};
MethodParser::~MethodParser(){};

bool MethodParser::completed() const
{
	return (_finished);
}

void MethodParser::execute() const
{
	
}
