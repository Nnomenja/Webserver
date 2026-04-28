#include "HeaderParser.hpp"
#include <algorithm>
#include "../../../../Exception/BadRequestException.hpp"

HeaderParser::HeaderParser(Request *target, UnitConf_t endpoint):ARequestParserState(HEADER, target, endpoint), _state(KEY){
};
HeaderParser::~HeaderParser(){
};

bool HeaderParser::checkHeaderEncode(char c)
{
	std::string other = "!#$%&'*+-.^_|~`";
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
	_target->setHeader(toLowerCase(_key), _value);
	resetStateData();
}


bool HeaderParser::endOfHeadersReached()
{
	if (_key.size())
		return (false);
	if (_target->getBuffer()[_target->getParserIndex()] == '\r')
	{
		_target->incrementParserIndex();
		if (_target->getBuffer()[_target->getParserIndex()] != '\n')
			throw BadRequestException();
		_target->incrementParserIndex();
	}
	else if (_target->getBuffer()[_target->getParserIndex()] == '\n')
		_target->incrementParserIndex();
	else
		throw BadRequestException();
	return (true);
}

void HeaderParser::receivingHeaders()
{
	char    c;

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
					if ((c == '\n' || c == '\r'))
					{
						if (endOfHeadersReached())
							return;
						if (c == '\n')
							addHeaderAndReset();
						else
							_state = DELIMITER;
					}
					else
						throw BadRequestException();
				}
				break;

			case SEPARATOR:
				if (c != ' ')
				{
					_state = VALUE;
					_value.push_back(c);
				}
				break;
			case DELIMITER:

				if (c != '\n')
					throw BadRequestException();
				addHeaderAndReset();
				break;

			default:
				if (!((c >= 33 && c <= 126 )|| c == ' '))
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


void HeaderParser::execute()
{
	receivingHeaders();
}
