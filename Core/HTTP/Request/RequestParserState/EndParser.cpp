#include "./EndParser.hpp"

EndParser::EndParser(Request *target, UnitConf_t endpoint):ARequestParserState(END, target, endpoint){
};
EndParser::~EndParser(){
};

void EndParser::execute()
{
    if (_target->getBodyEncode() == BODY_CHUNKED)
        return;
    skipCRLF();
}