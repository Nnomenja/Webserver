/*
   File: Webserv.hpp
   By: Azaria
   Created: 2026/02/25 09:49:33
*/

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <ctime>
#include <algorithm>
#include <sys/wait.h>

# include "../Settings/Config.hpp"
# include "../Network/Epoll.hpp"
# include "../Network/ServerSocket.hpp"
# include "../../Exception/ServerException.hpp"
#include "../../Data/Client.hpp"
#include "../../Data/Process.hpp"

#define MAXREADBYTES 1024
#define CGI_MAX_OUTPUT_BYTES 409600
#define CGI_TIMEOUT 50000 // ms

#define GREEN "\033[32m"
#define RESET "\033[0m"
#define RED   "\033[31m"
#define YELLOW "\033[33m"
typedef struct  s_child_process
{
    int     fd_client;
    int     fd_output;
    pid_t   pid;
}   t_child_process;

class Webserv
{
    public :
        ~Webserv();

        Webserv(const std::string& fileConfigName);
        bool    init();
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


        bool                                _isAlreadyInit;
        std::string                         _fileConfigName;
        Config                              _config;
        Epoll                               _epoll;
        int                                 _serverSocketsNumber;
        std::vector<ServerSocket*>          _serverSockets;
        std::map<int, Client*>              _clients;
        Process                             _process;
         std::map<std::string, std::string> _envs;
        
};

#endif /* WEBSERV_HPP */
