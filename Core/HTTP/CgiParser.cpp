#include "CgiParser.hpp"
#include "./Request/RequestParserState/HeaderParser.hpp"

CgiParser::CgiParser(const std::string &s, Response *res):_s(s), _state(HEADER), _i(0),_res(res)
{

}

void CgiParser::parseStatusCode(std::string &s)
{
	size_t i = 0;
	std::stringstream tmp;
	std::string message;
	int code;

	while (s[i])
	{
		if (i == ' ')
			break;
		if (!isdigit(s[i]))
			throw std::exception();
		tmp << s[i++];
	}
	tmp >> code;
	while (s[i])
	{
		message.push_back(s[i++]);
	}
	
	if (Response::getResponseStatus(code).message != message)
		throw std::exception();
	_res->setStatusCode(code);
	_res->setStatusName(message);
}

void CgiParser::addHeader(std::string &key, std::string &value)
{
	if (key == "Status")
		parseStatusCode(value);
	else
	{
		if (!_res->hasHeader(key))
			_res->addHeader(key, value);
	}
	key = "";
	value = "";
}

void CgiParser::parseHeader()
{
	char c;
	std::string key;
	std::string value;
	t_header_state state = KEY;

	while (_s[_i])
	{
        c = _s[_i];
		switch (state)
		{
			case KEY:
				if (c == ':')
				{
					if (!key.size())
						throw std::exception();
					state = SEPARATOR;
				}
				else if (HeaderParser::checkHeaderEncode(c))
					key.push_back(c);
				else
				{
					if (('\r') && (!key.size() && _s[_i + 1] == '\n'))
						return;
					else
						throw std::exception();
				}
				break;

			case SEPARATOR:
				if (c != ' ')
				{
					state = VALUE;
					value.push_back(c);
				}
				break;
			case DELIMITER:

				if (c != '\n')
					throw std::exception();
				addHeader(key, value);
				break;

			default:
				if (!((c >= 33 && c <= 126 )|| c == ' '))
				{
					if (c == '\r')
						state = DELIMITER;
					else if (c == '\n')
						addHeader(key, value);
					else
						throw std::exception();
				}
				else
					value.push_back(c);
				break;
		}
		_i++;
    }
	throw std::exception();
}

void CgiParser::parseBody()
{
	_res->setBody(&_s[_i]);
	_res->setContentLength(_res->getBody().size());
}

void CgiParser::parse()
{
	parseHeader();
	parseBody();
}

CgiParser::~CgiParser(){

};
