/*
   File: Socket.cpp
   By: Azaria
   Created: 2026/02/13 16:01:41
*/

# include "Socket.hpp"
# include "iostream"
# include <sstream>
# include <vector>
# include <unistd.h>

Socket::Socket()
	: _socketFd(-1)
{
	this->_addrLen = sizeof(this->_addr);
}

Socket::Socket(const Socket& socketType)
{
	*this = socketType;
}

Socket& Socket::operator=(const Socket& socketType)
{
	if (this != &socketType)
	{
		_socketFd = socketType._socketFd;
		_addr = socketType._addr;
		_addrLen = socketType._addrLen;
	}
	return (*this);
}

Socket::~Socket()
{ }

int Socket::getSocketFd( void ) const
{
	return (_socketFd);
}

sockaddrIn	Socket::getAddr( void ) const
{
	return (_addr);
}

socklen_t	Socket::AddrLen( void ) const
{
	return (_addrLen);
}

bool Socket::setsockopt(int level, int optname, unsigned int optlen)
{
    int opt = optlen;

    if (::setsockopt(_socketFd, level, optname, &opt, sizeof(opt)) < 0)
	{
        std::cout << "Error: setsockopt failed" << std::endl;
        return (false);
    }
	return (true);
}

bool Socket::inetPToN(int domain, const std::string& ipString, uint32_t& ipNum) const
{
	if (domain != AF_INET)
		return (false);

    std::stringstream ss(ipString);
    std::string token;
    std::vector<int> parts;
	int num = 0;
	int i = 0;

	while (std::getline(ss, token, '.'))
	{
		if (i > 3
			|| !stringToInt(token, num)
			|| num < 0
			|| num > 255)
		{
			std::cout << "Error: bad ip addr " << ipString << std::endl;
			return (false);
		}
        parts.push_back(num);
		i++;
	}
	ipNum = 0;
    ipNum = (parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8) | parts[3];
	ipNum = htonl(ipNum);
	return (true);
}

void	Socket::close( void )
{
	if (_socketFd >= 0)
	{
		::close(_socketFd);
		_socketFd = -1;
	}
}

bool stringToInt(const std::string& str, int& outValue)
{
    std::stringstream ss(str);
    ss >> outValue;

	if (ss.fail() || !ss.eof())
        return (false);
    return (true);
}

void	Socket::initSocketFd(int fd)
{
	if (_socketFd < 0)
		_socketFd = fd;
}

void	Socket::initAddr(sockaddrIn& addr)
{
	_addr = addr;
}

void	Socket::initAddrLen(socklen_t addrLen)
{
	_addrLen = addrLen;
}

