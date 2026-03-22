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

class Client;
class ClientSocket : public Socket
{
	public :
		ClientSocket(const Client& clientData);
		~ClientSocket();

		std::string getBuffer( void ) const;
		int			getBufferSize ( void ) const;
		int			getFdAcceptor( void ) const;
		void		setBuffer(const std::string& buff);
		void		setStartTime(const uint64_t value);

		std::string recv(unsigned int count, bool *end);
		ssize_t 	send(const std::string& buff);
		void 		appendBuffer(const std::string& newBuff);
		void		setBufferSize(size_t value);
		class Eagain : public std::exception{ };

	private :
		ClientSocket();
		ClientSocket(const ClientSocket& clientSocketType);
		ClientSocket& operator=(const ClientSocket& clientSocketType);

		int				_fdAcceptor;
		std::string 	_buffer;
		size_t			_size;
		uint64_t		_start_time_ms;
};

#endif /* CLIENTSOCKET_HPP */
