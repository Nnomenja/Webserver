/*
   File: Socket.hpp
   By: Azaria
   Created: 2026/02/13 16:01:56
*/

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <arpa/inet.h>
# include <string>

typedef  struct sockaddr_in sockaddrIn;
typedef struct sockaddr sockaddr;

typedef struct SSocketInfo
{
	int				fd;
	sockaddrIn		addr;
	socklen_t		addrlen;
} SocketInfo;

class Socket
{
	public :
		Socket();
		virtual ~Socket();

		int		getSocketFd( void ) const;
		sockaddrIn	getAddr( void ) const;
		socklen_t	AddrLen( void ) const;

		void	initSocketFd( int fd);
		void	initAddr( sockaddrIn& addr );
		void	initAddrLen( socklen_t addrLen );

	    bool	setsockopt(int level, int optname, unsigned int optlen);
		bool	inetPToN(int domain, const std::string& ipString
		, uint32_t& ipNum) const;
		bool	setBlocking(bool set);
		void	close( void );

	protected :
		Socket(const Socket& socketType);
		Socket& operator=(const Socket& socketType);

		int			_socketFd;
	    sockaddrIn	_addr;
		socklen_t 	_addrLen;
};

bool stringToInt(const std::string& str, int& outValue);

#endif /* SOCKET_HPP */

