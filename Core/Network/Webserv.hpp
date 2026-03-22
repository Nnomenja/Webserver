/*
   File: Webserv.hpp
   By: Azaria
   Created: 2026/02/25 09:49:33
*/

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

# include <vector>
# include <ctime>

# include "../Settings/Config.hpp"
# include "./Epoll.hpp"
# include "./ServerSocket.hpp"
# include "./ClientSocket.hpp"
# include "../../Exception/ServerException.hpp"

class Webserv
{
    public :
        ~Webserv();

        Webserv(const std::string& fileConfigName);
        bool    init( void );
        void    run( void );
        void    clear( void );

    private :
        Webserv();
        Webserv(const Webserv& webservType);
        Webserv& operator=(const Webserv& webservType);
        
        bool            createServerSockets( void );
        ServerSocket*   getServerSocket(int fd) const;

        /**
         * @brief Reads stream data from the client and passes it to `HttpRequestParser`.
         * @return `true` if the request is complete and no errors occur (e.g., parsing or blocking); otherwise `false`.
        */
        bool            readClientBuffer(ClientSocket& client);
        void            writeClientResponse(ClientSocket& client);
		void		    http(ClientSocket& client);
        void            removeSocket(int fd);

        bool						_isAlreadyInit;
        std::string					_fileConfigName;
        Config						_config;
        Epoll						_epoll;
        int							_serverSocketsNumber;
        std::vector<ServerSocket*>	_serverSockets;
        std::map<int, Client>       _clients;
};

#endif /* WEBSERV_HPP */
