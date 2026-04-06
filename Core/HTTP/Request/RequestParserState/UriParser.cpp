#include "UriParser.hpp"
#include "../../../../Exception/BadRequestException.hpp"

UriParser::UriParser(Request *target, UnitConf_t endpoint):ARequestParserState(URI, target, endpoint), _tmp(-1), _hasQuery(false), _encoded(false), _finished(false){
};
UriParser::~UriParser(){
};

bool  UriParser::checkPathEncode(char c) const
 {
  std::string other = "-._~!$&'()*+,;=:@/";
  return (std::isalnum(c) || other.find(c) != std::string::npos);
}

bool  UriParser::checkQueryEncode(char c) const
{
  std::string other = "-._~!$&'()*+,;=:@/?";
  return (std::isalnum(c) || other.find(c) != std::string::npos);
}


bool  UriParser::checkPathPourcentEncoded(char c) const
 {
  std::string other = "?#[]@=&+;% ";
  return (other.find(c) != std::string::npos);
}

bool  UriParser::checkQueryPourcentEncoded(char c) const
 {
  std::string other = "=&?#+\"<>{}`^~[]` ";
  return (other.find(c) != std::string::npos);
}


bool UriParser::isHexa(char c) const
{
	return (std::isxdigit(static_cast<unsigned char>(c)));
}

int UriParser::HexaToInt(char c) const
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	return (-1);
}

char UriParser::PourcentHexaToChar(char first, char second) const
{
	int		msb;
	int		lsb;

	msb = HexaToInt(first);
	lsb = HexaToInt(second);
	if (msb == -1 || lsb == -1)
		throw BadRequestException();
	return (static_cast<unsigned char>((msb << 4 | lsb)));
}


std::string UriParser::normalizePathname(std::string pathname) const
{
	int									i;
	std::string							tmp;
	std::stringstream					segment;
	std::stringstream					res;
	std::vector<std::string>			el;
	std::vector<std::string>::iterator	it;

	i = 0;
	if (pathname.length() == 1)
		return (pathname);
	while (pathname[i])
	{
		if (pathname[i] != '/')
			segment << pathname[i];
		if ((pathname[i] != '/' && pathname[i + 1] == '/') || !pathname[i + 1])
		{
				tmp = segment.str();
				if (tmp.length())
				{
					if (tmp == "..")
					{
						if (!el.empty())
							el.pop_back();
					}
					else if (tmp != ".")
					{
						el.push_back(tmp);
					}
					segment.clear();
					segment.str("");
				}
		}
		i++;
	}
	res << "/";
	if (el.size())
	{
		it = el.begin();
		while (it != el.end())
		{
			res << (*it);
			it++;
			if (it != el.end())
				res << "/";
		}
	}
	return (res.str());
}

void UriParser::execute()
{
	std::cout << "Uri executing..." << std::endl;
	char c;

	if (_finished)
	{
		skipSeparator();
		return ;
	}

	for (size_t i = _target->getParserIndex(); i < _target->getBufferSize(); i++)
	{
		c = _target->getBuffer()[i];
		if (!_encoded)
		{
			if (c == ' ')
			{
				if (_target->getPathname()[0] != '/')
					throw BadRequestException();
				_target->setPathname(normalizePathname(_target->getPathname()));
				std::cout << "-->pathname: [" << _target->getPathname() << "]" << std::endl;
				std::cout << "-->Query: [" << _target->getQuery() << "]" << std::endl;
				_finished = true;
				skipSeparator();
				return;
			}
			else if (!_hasQuery && c == '?')
				_hasQuery = true;
			else if (c == '%')
				_encoded = true;
			else
			{
				if (_hasQuery)
				{
					if (!checkQueryEncode(c))
						throw BadRequestException();
					_target->addQuery(c);
				}
				else
				{
					if (!checkPathEncode(c))
						throw BadRequestException();
					_target->addPathname(c);
				}
			}
		}
		else
		{
			if (!isHexa(c))
				throw BadRequestException();
			if (_tmp == -1)
				_tmp = c;
			else
			{
				c = PourcentHexaToChar(_tmp, c);
				if (_hasQuery)
				{
					if (!checkQueryPourcentEncoded(c))
						throw BadRequestException();
					_target->addQuery(c);
				}
				else
				{
					if (!checkPathPourcentEncoded(c))
						throw BadRequestException();
					_target->addPathname(c);
				}
				_tmp = -1;
				_encoded = false;
			}
		}
		_target->incrementParserIndex();
	}
	_target->resetParserIndex();
	throw EagainParser();
	std::cout << "...................." << std::endl;
}
