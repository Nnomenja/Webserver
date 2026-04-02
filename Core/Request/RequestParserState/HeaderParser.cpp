#include "HeaderParser.hpp"
#include <algorithm>
#include "../../../Exception/BadRequestException.hpp"

HeaderParser::HeaderParser(Request *target, UnitConf_t endpoint):ARequestParserState(HEADER, target, endpoint), _state(KEY){
};
HeaderParser::~HeaderParser(){
};

bool HeaderParser::checkHeaderEncode(char c) const
{
	std::string other = "!#$%&'*+-.^_`|~:";
	(void)c;
	return (isalnum(c) || other.find(c) != std::string::npos);
}

std::string &HeaderParser::toLowerCase(std::string &src) const
{
	std::transform(src.begin(), src.end(), src.begin(), ::tolower);
	return (src);
}

void HeaderParser::resetStateData()
{
	_state = KEY;
	_key.clear();
	_value.clear();
}

void HeaderParser::addHeaderAndReset()
{
	if (_target->hasHeader(_key))
		throw BadRequestException();
	_target->setHeader(_key, _value);
	resetStateData();
}


bool HeaderParser::endOfHeadersReached()
{
	if (_key.size())
		return (false);
	return (true);
}

void HeaderParser::receivingHeaders()
{
	char    c;

	std::cout << "...HeaderParser executing..." << std::endl;
    for (size_t i = _target->getParserIndex(); i < _target->getBufferSize(); i++)
    {
        c = _target->getBuffer()[i];
		switch (_state)
		{
			case KEY:
				if (c == ':')
				{
						if (!_key.size())
							throw BadRequestException();
						_state = SEPARATOR;
				}
				else if (checkHeaderEncode(c))
					_key.push_back(c);
				else
				{
					if (c == '\r')
						_state = DELIMITER;
					else if (c == '\n')
					{
						if (endOfHeadersReached())
							return;
						addHeaderAndReset();
					}
					else
						throw BadRequestException();
				}
				break;

			case SEPARATOR:
				if (c != ' ')
				{
					if (checkHeaderEncode(c))
					{
						_state = VALUE;
						_value.push_back(c);
					}
					else
					{
						throw BadRequestException();
					}
				}
				break;
			case DELIMITER:

				if (c != '\n')
					throw BadRequestException();
				if (endOfHeadersReached())
					return;
				addHeaderAndReset();
				_state = KEY;
				break;

			default:
				if (!checkHeaderEncode(c) && c != ' ')
				{
					if (c == '\r')
						_state = DELIMITER;
					else if (c == '\n')
						addHeaderAndReset();
					else
						throw BadRequestException();
				}
				else
					_value.push_back(c);
				break;
		}
		_target->incrementParserIndex();
    }
	_target->resetParserIndex();
	throw EagainParser();
}

void HeaderParser::parseHeaders()
{
	if (_target->hasHeader("Host"))
		throw BadRequestException();
}

void HeaderParser::execute()
{
	receivingHeaders();
}
