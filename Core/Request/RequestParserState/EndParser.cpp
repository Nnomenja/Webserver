#include "./EndParser.hpp"

EndParser::EndParser(Request *target, UnitConf_t endpoint):ARequestParserState(END, target, endpoint){
};
EndParser::~EndParser(){
};

void EndParser::execute()
{
    std::cout << "EndParser executing..." << std::endl;
    skipCRLF();
}