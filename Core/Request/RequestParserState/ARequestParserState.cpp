#include "./ARequestParserState.hpp"
#include "../../Settings/Config.hpp"

ARequestParserState::ARequestParserState(RequestParserStateName value, Request *target, UnitConf_t endpoint):_name(value), _target(target), _endpoint(endpoint)
{
}

RequestParserStateName ARequestParserState::getParserStateName() const
{
    return (_name);
}

ARequestParserState::~ARequestParserState()
{

}