/*
   File: Webserv.hpp
   By: Azaria
   Created: 2026/02/25 09:49:33
*/

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

# include <ctime>

# include "../Settings/Config.hpp"
# include "../Network/Epoll.hpp"
# include "../Network/ServerSocket.hpp"
# include "../../Exception/ServerException.hpp"
#include "../../Data/Client.hpp"

#define MAXREADBYTES 1024

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
        bool            readtHttpRequest(Client* client);
        void            sendHttpResponse(Client* client);
        void            removeClientHttp(int fd);

		void		    simulateClient(Client* client);

        bool						_isAlreadyInit;
        std::string					_fileConfigName;
        Config						_config;
        Epoll						_epoll;
        int							_serverSocketsNumber;
        std::vector<ServerSocket*>	_serverSockets;
        std::map<int, Client*>       _clients;
};

#endif /* WEBSERV_HPP */
