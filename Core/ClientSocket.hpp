/*
   File: ClientSocket.hpp
   By: Azaria
   Created: 2026/02/20 15:12:01
*/

#ifndef CLIENTSOCKET_HPP
#define CLIENTSOCKET_HPP

# include <exception>
# include "Socket.hpp"
# define MAXREADBYTES 1024
#define	CHARARRAY(name, size) char name[size]

// typedef struct SClientData
// {
// 	SocketInfo clientInfo;
// 	int			fdAcceptor;

//     std::string buffer;
// 	uint64_t	start_time_ms;
// } 		ClientData;



class Client;
class ClientSocket : public Socket
{
	public :
		ClientSocket(const Client& clientData);
		~ClientSocket();

		std::string getBuffer( void ) const;
		int			getFdAcceptor( void ) const;
		void		setBuffer(const std::string& buff);
		void		setStartTime(const uint64_t value);

		std::string recv(unsigned int count);
		ssize_t 	send(const std::string& buff);
		void 		appendBuffer(const std::string& newBuff);
		class Eagain : public std::exception{ };

	private :
		ClientSocket();
		ClientSocket(const ClientSocket& clientSocketType);
		ClientSocket& operator=(const ClientSocket& clientSocketType);

		int			_fdAcceptor;
		std::string _buffer;
		uint64_t	_start_time_ms;
};

#endif /* CLIENTSOCKET_HPP */
