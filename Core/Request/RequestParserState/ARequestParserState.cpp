#include "./ARequestParserState.hpp"
#include "../../Settings/Config.hpp"
#include "../../../Exception/BadRequestException.hpp"

ARequestParserState::ARequestParserState(RequestParserStateName value, Request *target, UnitConf_t endpoint):_crlf(false), _name(value), _target(target), _endpoint(endpoint)
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
	std::cout << "...Skip separator  executing..." << std::endl;
  for (size_t i = _target->getParserIndex(); i < _target->getBufferSize(); i++)
  {
    if (_target->getBuffer()[i] != ' ')
        return;
    _target->incrementParserIndex();
  }
  _target->resetParserIndex();
  throw EagainParser();
}

void ARequestParserState::skipCRLF()
{
	char c;
	for (size_t i = _target->getParserIndex(); i < _target->getBufferSize(); i++)
	{
		c = _target->getBuffer()[i];
		if (!_crlf)
		{
			if (c == '\r')
				_crlf = true;
			else if (c == '\n')
			{
				_target->incrementParserIndex();
				return;
			}
		}
		else
		{
			if (c == '\n')
			{
				_crlf = false;
				_target->incrementParserIndex();
				return;
			}
			else
				throw BadRequestException();
		}
		_target->incrementParserIndex();
	}
	_target->resetParserIndex();
	throw EagainParser();
}
