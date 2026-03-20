/*
   File: ServerSocket.cpp
   By: Azaria
   Created: 2026/02/20 15:13:18
*/

#include "ServerSocket.hpp"
# include "iostream"
# include <sstream>
# include <vector>
# include <unistd.h>
# include <fcntl.h>

# include "ClientSocket.hpp"

/* ************************************************************************** */
/*                            Canonical Form                                  */
/* ************************************************************************** */

ServerSocket::ServerSocket()
	: Socket()
{ }

ServerSocket::ServerSocket(const ServerSocket& serverSocketType)
	: Socket(serverSocketType)
{
	*this = serverSocketType;
}

ServerSocket& ServerSocket::operator=(const ServerSocket& serverSocketType)
{
	if (this != &serverSocketType)
	{
		Socket::operator=(serverSocketType);
	}
	return (*this);
}

ServerSocket::~ServerSocket()
{ }

/* ************************************************************************** */
/*                             Other Methods                                  */
/* ************************************************************************** */

bool ServerSocket::socket(int domain, int type, int protocol)
{
    _addr.sin_family = domain;
    _socketFd = ::socket(domain, type, protocol);

    if (_socketFd < 0)
	{
		std::cerr << "Error: Cannot create socket" << std::endl;
        return (false);
	}
	return (true);
}

bool	ServerSocket::bind(const std::string& host, uint16_t port)
{
	_addr.sin_port = htons(port);

	uint32_t addrTmp;

	if (host.empty())
	    _addr.sin_addr.s_addr = addrTmp = INADDR_ANY;
	else
	{
		if (!inetPToN(_addr.sin_family, host, addrTmp))
			return (false);
	    _addr.sin_addr.s_addr = addrTmp;
	}
    if (::bind(_socketFd, reinterpret_cast<struct sockaddr*>(&_addr), sizeof(_addr)) < 0)
	{
        std::cerr << "Error: bind failed" << std::endl;
        return (false);
    }
	return (true);
}

bool ServerSocket::listen(int backlog)
{
    if (::listen(_socketFd, backlog) < 0)
	{
        std::cerr << "Error: listen failed" << std::endl;
        return (false);
    }
	return (true);
}

SocketInfo 	ServerSocket::accept( void )
{
	SocketInfo info;

	info.fd = -1;
	info.addrlen = 0;

	info.fd = ::accept(_socketFd
		, reinterpret_cast<sockaddr *>(&info.addr)
		, &info.addrlen);

	if (info.fd < 0)
        std::cerr << "Error: accept failed" << std::endl;
	return (info);
}

