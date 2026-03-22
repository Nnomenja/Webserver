/*
   File: ServerSocket.hpp
   By: Azaria
   Created: 2026/02/20 15:13:34
*/

#ifndef SERVERSOCKET_HPP
#define SERVERSOCKET_HPP

# include "Socket.hpp"

class ClientSocket;

class ServerSocket : public Socket
{
	public :
		ServerSocket();
		~ServerSocket();

		bool		socket(int domain, int type, int protocol=0);
		bool		bind(const std::string& host, uint16_t port);
		bool		listen(int backlog=SOMAXCONN);
		SocketInfo	accept( void );

	private :
		ServerSocket(const ServerSocket& serverSocketType);
		ServerSocket& operator=(const ServerSocket& serverSocketType);
};

#endif /* SERVERSOCKET_HPP */
