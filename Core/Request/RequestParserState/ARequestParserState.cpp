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

void ARequestParserState::skipSeparator()
{
	std::cout << "...Skip parse executing..." << std::endl;
  for (size_t i = _target->getParserIndex(); i < _target->getBufferSize(); i++)
  {
    if (_target->getBuffer()[i] != ' ')
        return;
    _target->incrementParserIndex();
  }
  _target->resetParserIndex();
  throw EagainParser();
}