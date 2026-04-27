#include "./Request.hpp"


Request::Request(std::string &value, int receivedBytes)
	:_buffer(value), _receivedByte(receivedBytes),_i(0),
	_hasPourcentEncode(false), _hasQuery(false), _hasContentLength(false),
	_hasContentType(false), _hasBody(false)
{
	/*------- Request Line -------*/
	setMethod();
	setPathname();
	readQuery();
	readProtocol();
	setHttpVersion();
	normalizePathname();
	DecodePourcentEncode();
	/*------- Request Header -------*/
	readHeader();
	setHost();
	setBody();
}

Request::~Request(){};

/**========================================================================
 *                           METHODS
 *========================================================================**/

bool Request::checkHeaderLineEncode(char c) const
{
  return (c && std::isprint(c));
}

char Request::readByte()
{
  return (_buffer[this->_i++]);  
}
 
/**========================================================================
 *                           UTILS
 *========================================================================**/

int Request::HexaToInt(char c) const
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	return (-1);
}

bool Request::isHexa(char c) const
{
	return (std::isxdigit(static_cast<unsigned char>(c)));
}

bool  Request::checkPathEncode(char c) const
 {
  std::string other = "-._~!$&'()*+,;=:@/";
  return (std::isalnum(c) || other.find(c) != std::string::npos);
}

bool  Request::checkQueryEncode(char c) const
 {
  std::string other = "-._~!$&'()*+,;=:@/?";
  return (std::isalnum(c) || other.find(c) != std::string::npos);
}

bool  Request::checkPathPourcentEncoded(char c) const
 {
  std::string other = "?#[]@=&+;% ";
  return (other.find(c) != std::string::npos);
}

bool  Request::checkQueryPourcentEncoded(char c) const
 {
  std::string other = "=&?#+\"<>{}`^~[]` ";
  return (other.find(c) != std::string::npos);
}

bool Request::checkHeaderEncode(char c) const
{
	std::string other = "!#$%&'*+-.^_`|~:";
	return (std::isalnum(c) || other.find(c) != std::string::npos);
}

std::string &Request::toLowerCase(std::string &src) const
{
	std::transform(src.begin(), src.end(), src.begin(), ::tolower);
	return (src);
}

/**========================================================================
 *                           SETTERS
 *========================================================================**/



void Request::setMethod()
{
  char                    	tmp;
  int                     	i;
  std::vector<std::string>	methods;

  i = 0;
  methods.push_back("GET");
  methods.push_back("POST");
  methods.push_back("DELETE");
  methods.push_back("OPTIONS");
  methods.push_back("HEAD");
  methods.push_back("PUT");
  methods.push_back("TRACE");

  while (1)
  {
    tmp = readByte();
    if (!checkHeaderLineEncode(tmp))
      throw BadRequest();
    for (std::vector<std::string>::iterator it = methods.begin(); it != methods.end(); )
    {
        if ((*it)[i] != tmp)
          it = methods.erase(it);
        else
        {
          if ((*it)[i + 1] == '\0')
          {
            tmp = readByte();
            if (tmp != ' ')
              it = methods.erase(it);
            else
            {
              this->_method = *it;
              if (this->_method != "GET" && this->_method != "POST" && this->_method != "DELETE" )
                throw NotImplementedMethod();
              return;
            }
          }
          else
            it++;
        }
    }
    if (!methods.size())
      throw BadRequest();
    i++;
  }
}

void Request::setPathname()
{
	char tmp;
	std::ostringstream path;
	int i;

	tmp = readByte();
	if (tmp != '/')
		throw BadRequest();
	path << tmp;
	while (1)
	{
		tmp = readByte();
		if (tmp == ' ' || tmp == '?' )
		{
			if (tmp == '?')
			_hasQuery = true;
			break;
		}
		else if (!checkPathEncode(tmp))
		{
			if (tmp == '%')
			{
				i = 2;
				path << tmp;
				while (i--)
				{
					tmp = readByte();
					isHexa(tmp) ? (path << tmp) : throw BadRequest();
				}
				this->_hasPourcentEncode = true;
			}
			else
				throw BadRequest();
		}
		else
			path << tmp;
	}
	this->_pathname = path.str();
}

char Request::readPourcentHexa()
{
	int		msb;
	int		lsb;
	int		value;

	value = readByte();
	msb = HexaToInt(value);
	value = readByte();
	lsb = HexaToInt(value);
	if (msb == -1 || lsb == -1)
		throw BadRequest();
	return (static_cast<unsigned char>((msb << 4 | lsb)));
}


char Request::PourcentHexaToChar(char first, char second) const
{
	int		msb;
	int		lsb;

	msb = HexaToInt(first);
	lsb = HexaToInt(second);
	if (msb == -1 || lsb == -1)
		throw BadRequest();
	return (static_cast<unsigned char>((msb << 4 | lsb)));
}

void Request::readQuery()
{
	char	tmp;

	if (!_hasQuery)
		return;
	while (1)
	{
		tmp = readByte();
		if (tmp == ' ')
			break;
		if (tmp == '%')
		{
			tmp = readPourcentHexa();
			if (!checkPathPourcentEncoded(tmp))
				throw BadRequest();
		}
		else if (!checkQueryEncode(tmp))
			throw BadRequest();
	}
}

void Request::readProtocol()
{
	char				tmp;
	int					count;
	std::stringstream 	value;

	count = 0;
	while (count < 5)
	{
		tmp = readByte();
		if (!tmp)
			throw BadRequest();
		value << tmp;
		count++;
	}
	if (value.str() != "HTTP/")
	{
		throw BadRequest();
	}
}

void Request::setHttpVersion()
{
	char						tmp;
	int							i;
	std::vector<std::string>	versions;

	i = 0;

	versions.push_back("1.0");
	versions.push_back("1.1");
	versions.push_back("2");
	versions.push_back("3");

	while (1)
	{
		tmp = readByte();
		for (std::vector<std::string>::iterator it = versions.begin(); it != versions.end(); )
		{
			if ((*it)[i] != tmp)
				it = versions.erase(it);
			else if ((*it)[i + 1] == '\0')
			{
				tmp = readByte();
				if (tmp != '\r')
					it = versions.erase(it);
				else
				{
					tmp = readByte();
					if (tmp != '\n')
						throw BadRequest();
					this->_httpVersion = *it;
					return;
				}
			}
			else
			  it++;
		}
		if (!versions.size())
			throw BadRequest();
		i++;
	}
}


void Request::normalizePathname()
{
	int									i;
	std::string							tmp;
	std::stringstream					segment;
	std::stringstream					res;
	std::vector<std::string>			el;
	std::vector<std::string>::iterator	it;

	i = 0;
	if (_pathname.length() == 1)
		return;
	while (_pathname[i])
	{
		if (_pathname[i] != '/')
			segment << _pathname[i];
		if ((_pathname[i] != '/' && _pathname[i + 1] == '/') || !_pathname[i + 1])
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
	_pathname = res.str();
}


void Request::DecodePourcentEncode()
{
	std::stringstream	pathDecoded;
	ssize_t				i;
	char				tmp;

	if (!_hasPourcentEncode)
		return;
	i = 0;
	while (_pathname[i])
	{
		if (_pathname[i] == '%')
		{
			tmp = PourcentHexaToChar(_pathname[i + 1], _pathname[i + 2]);
			if (!checkPathPourcentEncoded(tmp))
				throw BadRequest();
			pathDecoded << tmp;
			i += 2;
		}
		else
			pathDecoded << _pathname[i];
		i++;
	}
	_pathname = pathDecoded.str();
}

void Request::readHeader()
{
	char				c;
	std::stringstream	tmp;
	bool				side;
	std::string			key;
	std::string			value;

	side = false;
	while (1)
	{
		c = readByte();
		if (!c && !side)
			return;
		if (c == ':' && !side)
		{
			c = readByte();
			if (c != ' ')
				throw BadRequest();
			side = true;
			key = tmp.str();
			tmp.clear();
			tmp.str("");
		}
		else if (c == '\r')
		{
			c =  readByte();
			if (c == '\n')
			{
				value = tmp.str();
				if (value.empty())
					return;
				key = toLowerCase(key);
				if (_headers.find(value) != _headers.end() && (_headers[key] != value))
					throw BadRequest();
				if (key == "host")
				{
					_headers[key] = value;
					setHost();
				}
				else if (key == "content-length")
				{
					_headers[key] = value;
					_hasContentLength = true;
					setContentLength();
				}
				else if (key == "content-type")
				{
					_hasContentType = true;
					_headers[key] = value;
					setContentType();
				}
				tmp.clear();
				tmp.str("");
				key = "";
				side = false;
			}
			else
			{
				throw BadRequest();
			}
		}
		else
			tmp << c;
	}
	
}


void Request::setHost()
{
	std::string 		host;
	std::string			hostName;
	std::stringstream	tmp;
	int					i;
	int					port;

	hostName = "localhost:";
	if (_headers.find("host") == _headers.end())
		throw BadRequest();
	host = _headers["host"];
	for (size_t i = 0; i < hostName.length(); i++)
	{
		if (!host[i] || host[i] != hostName[i])
			throw BadRequest();
	}
	i = hostName.length();
	while (1)
	{
		if (host[i] == '\0')
			break;
		if (!isdigit(host[i]))
			throw BadRequest();
		else
		{
			if (i == 5)
				throw BadRequest();
			tmp << host[i];
			i++;
		}
	}
	tmp >> port;
	if (port < 1024 || port > 49151)
		throw BadRequest();
	this->_port = port;
}

void Request::setContentLength()
{
	int i;
	std::string value = _headers["content-length"];
	ssize_t	res = 0;

	i = -1;
	while (value[++i])
	{
		if (!isdigit(value[i]))
			throw BadRequest();
		res *= 10;
		res += value[i] - '0';
		if (res > CONTENT_LENGTH_MAX)
			throw PayloadTooLarge();
	}
	_contentLength = res;
}

void Request::setContentType()
{
	char c;
	std::stringstream tmp;
	std::string value;
	std::string str = _headers["content-type"];
	int i = 0;

	while (1)
	{
		c = str[i];
		if (!c || c == ';')
		{
			value = tmp.str();
			if(value == "")
				throw BadRequest();
			_contentType.push_back(value);
			if (!c)
				break;
			i++;
			c = str[i];
			if (c != ' ')
				throw BadRequest();
			i++;
			c = str[i];
			if (!c || c == ';')
				throw BadRequest();
		}
		tmp << c;
		i++;
	}
}


/**========================================================================
 *                           GETTERS
 *========================================================================**/

std::string Request::getMethod() const
{
	return (_method);
}

std::string Request::getPathName() const
{
	return (_pathname);
}

std::string Request::getHTTPversion() const
{
	return (_httpVersion);
}

bool Request::getHasQuery() const
{
	return (_hasQuery);
}

int Request::getPort() const
{
	return (_port);
}

bool Request::getHasContentLength() const
{
	return (_hasContentLength);
}

int Request::getContentLength() const
{
	return (_contentLength);
}

std::string Request::getContentType() const
{
	std::map<std::string, std::string>::const_iterator it = _headers.find("content-type");
	if (it != _headers.end())
		return (it->second);
	return ("");
}

/**========================================================================
 *                           EXCEPTIONS
 *========================================================================**/

const char *Request::InvalidMethod::what() const throw ()
{
  return ("Invalid method");
}

const char *Request::NotImplementedMethod::what() const throw ()
{
  return ("501 Method not implemented");
}

const char *Request::BadRequest::what() const throw ()
{
  return ("400 Bad request");
}

const char *Request::URITooLong::what() const throw ()
{
  return ("414 Request-URI Too Long");
}

const char *Request::NotFound::what() const throw ()
{
  return ("404 Not found");
}

const char *Request::LengthRequired::what() const throw ()
{
  return ("411 Length Required");
}

//?????????
const char *Request::PayloadTooLarge::what() const throw ()
{
  return ("413 Payload Too Large");
}
//?????????

const char *Request::UnsupportedMediaType::what() const throw ()
{
  return ("415 Unsupported Media Type");
}

/**========================================================================
 *                           Body
 *========================================================================**/

std::string Request::getBody( void ) const
{
	return (_body);
}

static bool methodAcceptsBody (const std::string& method)
{
	return ((method == "POST") || (method == "PUT")
		|| (method == "PATCH"));
}

static bool bodyPresent(const std::string& body)
{
	return ((body.size()) ? true : false);
}


bool Request::requiredHeaderPresent (void) const
{
	//delimited
	if (!_hasContentLength)
		throw Request::LengthRequired();
	//----------------------------------------
	
	return (_hasContentType);
}

void Request::setBody( void )
{
	// chage on read_from_socket
	std::string body = _buffer.substr(_i);
	// ---------------------------------------

	if (bodyPresent(body))
	{
		if (methodAcceptsBody(_method))
		{
			if (!requiredHeaderPresent())
				throw Request::BadRequest();

			if (body.size() != _contentLength)
				throw Request::BadRequest();
			
			_body = _buffer.substr(_i, _i + _contentLength);
		}
		else
			throw Request::BadRequest();
	}
}
