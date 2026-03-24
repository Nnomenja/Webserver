#include "./ARequestParserState.hpp"

ARequestParserState::ARequestParserState(RequestParserStateName value):_name(value)
{

}

RequestParserStateName ARequestParserState::getParserStateName() const
{
    return (_name);
}

ARequestParserState::~ARequestParserState()
{

}