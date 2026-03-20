/*
   File: Webserv.hpp
   By: Azaria
   Created: 2026/02/25 09:49:33
*/

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

# include <vector>
# include <ctime>

# include "./Config.hpp"
# include "./Epoll.hpp"
# include "./ServerSocket.hpp"
# include "./ClientSocket.hpp"
# include "./ParseRequest.hpp"

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
        ParseRequest				*_parse;

};

#endif /* WEBSERV_HPP */
