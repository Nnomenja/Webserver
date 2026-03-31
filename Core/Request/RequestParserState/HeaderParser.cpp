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
	return (std::isalnum(c) && other.find(c) == std::string::npos);
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

void HeaderParser::execute()
{
    char    c;

	std::cout << "...HeaderParser executing..." << std::endl;
    for (size_t i = _target->getParserIndex(); i < _target->getBufferSize(); i++)
    {
        c = _target->getBuffer()[i];
		switch (_state)
		{
			case KEY:
				if (checkHeaderEncode(c))
					_key.push_back(c);
				else
				{
					if (c == ':')
					{
						if (!_key.size())
							throw BadRequestException();
						_state = SEPARATOR;
					}
					else if (c == '\r')
						_state = DELIMITER;
					else if (c == '\n')
					{
						if (!_key.size())
						{
							_target->setHeader(_key, _value);
							return;
						}
						std::cout <<  "key->[" << _key << "] value->[" << _value << "]" << std::endl;
						resetStateData();
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
					else if (c == '\r')
						_state = DELIMITER;
					else if (c == '\n')
						_state = KEY;
					else
						throw BadRequestException();
				}
				break;
			case DELIMITER:
				if (c != '\n')
					throw BadRequestException();
				if (!_key.size())
				{
					_target->setHeader(_key, _value);
					return;
				}
				std::cout <<  "key->[" << _key << "] value->[" << _value << "]" << std::endl;
				resetStateData();
				break;
			default:
				if (!checkHeaderEncode(c))
				{
					if (c == '\r')
						_state = DELIMITER;
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
