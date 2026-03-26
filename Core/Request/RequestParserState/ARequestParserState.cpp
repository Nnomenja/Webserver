#include "./ARequestParserState.hpp"

ARequestParserState::ARequestParserState(RequestParserStateName value, Request *target):_name(value), _target(target)
{
}

RequestParserStateName ARequestParserState::getParserStateName() const
{
    return (_name);
}

ARequestParserState::~ARequestParserState()
{

}