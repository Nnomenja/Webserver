#include "Webserv.hpp"
#include "../Request/HttpRequestParser.hpp"
#include "../../Data/Request.hpp"
#include "../../Data/Client.hpp"
#include "../Request/RequestProcessor.hpp"
#include <signal.h>

extern volatile sig_atomic_t stop;

/* ************************************************************************** */
/*                            Canonical Form                                  */
/* ************************************************************************** */

Webserv::Webserv()
    : _isAlreadyInit(false)
{ }

Webserv::Webserv(const Webserv& webservType)
{
    *this = webservType;
}

Webserv& Webserv::operator=(const Webserv& webservType)
{
    if (this != &webservType)
    {
        return (*this);
    }
    return *this;
}

Webserv::~Webserv()
{
    this->clear();
}

/* ************************************************************************** */
/*                         Specific Constructors                              */
/* ************************************************************************** */

Webserv::Webserv(const std::string& fileConfigName)
    :   _isAlreadyInit(false)
        ,_fileConfigName(fileConfigName)
{ }

/* ************************************************************************** */
/*                                Getters                                     */
/* ************************************************************************** */


/* ************************************************************************** */
/*                                Setters                                     */
/* ************************************************************************** */


/* ************************************************************************** */
/*                             Other Methods                                  */
/* ************************************************************************** */


void Webserv::removeSocket(int fd)
{
	::close(fd);
	_clients.erase(fd);
	_epoll.unregister(fd);
}

// SIMULATION ->
typedef struct UnitConfSimul
{
    std::string 	host;
    int 			port;
    int 			methods;
	EndpointType	type;
} UnitConfSimul_t;

void initSimulData(std::vector<UnitConfSimul_t> &configsSimul)
{
    UnitConfSimul_t u;
    
	// n = 1
    u.host = "0.0.0.0";
    u.port = 2000;
    u.methods = GET + POST;
	u.type = STATIC;
    configsSimul.push_back(u);
    // n = 2
	u.host = "0.0.0.0";
    u.port = 2001;
	u.type = STATIC;
    u.methods = GET + POST + DELETE;
    configsSimul.push_back(u);
}
// <- SIMULATION

bool Webserv::init( void )
{
    if (_isAlreadyInit)
        return (true);
	// REAL ->
    // try
    // {
    //     Config configTmp(_fileConfigName);
    //     _config = configTmp;
    // }
    // catch(const Config::ConfigException& e)
    // {
    //     std::cerr << "Error: " << e.what() << std::endl;
    //     return (false);
    // }
	// <- REAL

    if (!createServerSockets())
	{
        return (false);
	}

    /*
        donne tolotra---------------------------------------------
    */
        // -- epoll
        int maxEvent = MAX_EVENTS;

        if (!_epoll.epollCreate())
            return (false);        
        _epoll.setMaxEvent(maxEvent);

        for (size_t i = 0; i < _serverSockets.size(); i++)
        {
            if (!_epoll.registerFd(_serverSockets[i]->getSocketFd(), EPOLLIN))
                return (false);
        }
    //=============================================================
    _isAlreadyInit = true;
    return (true);
}

bool Webserv::createServerSockets( void )
{
	// REAL ->
    // _serverSocketsNumber = _config.getN();

	// std::vector<UnitConf_t> configs = _config.getConfigs();
	// <- REAL

	// SIMULATION ->
	_serverSocketsNumber = 2;
	std::vector<UnitConfSimul_t> configs;
	initSimulData(configs);
	// <- SIMULATION

    for (int i = 0; i < _serverSocketsNumber; i++)
    {
        ServerSocket *tmp = new ServerSocket;
        if (!tmp)
            return (false);
        _serverSockets.push_back(tmp);

        /*
            donne tolotra---------------------------------------------
        */

            // socket -----------------------
        int domain = AF_INET;
        int type = SOCK_STREAM;
        int protocol = IPPROTO_TCP;

        if (!tmp->socket(domain, type, protocol))
            return (false);

            // sockopt -----------------------
        int level = SOL_SOCKET;
        int optname = SO_REUSEADDR;
        int optlen = 1;

        if (!tmp->setsockopt(level, optname, optlen))
            return (false);	

            // blocking ----------------------
        bool blocking = false;

        if (!tmp->setBlocking(blocking))
            return (false);
            // bind --------------------------

        std::string& host = configs[i].host;
        uint16_t port = configs[i].port;

        if (!tmp->bind(host, port))
            return (false);

        /*
            -------------------------------------------------------------
        */

        int backlog = SOMAXCONN;
        if (!tmp->listen(backlog))
            return (false);

        std::cout << "Server "<< i << ":"
                << std::endl
                << "    Host = " << host << std::endl
                << "    port = " << port << std::endl
                << "    fd = " << tmp->getSocketFd() << std::endl
                << "    No blocking" << std::endl
                << std::endl;
    }
    return (true);
}

void  Webserv::clear( void )
{
    _epoll.close();
    _serverSocketsNumber = 0;
    for (size_t i = 0; i < _serverSockets.size(); i++)
    {
        _serverSockets[i]->close();
        delete _serverSockets[i];
    }
    _serverSockets.clear();
    _clients.clear();
    _isAlreadyInit = false;
}

ServerSocket* Webserv::getServerSocket(int fd) const
{
    for (size_t i = 0; i < _serverSockets.size(); i++)
    {
        if (_serverSockets[i]->getSocketFd() == fd)
            return (_serverSockets[i]);
    }
    return (0);
}

void Webserv::run(void)
{
	RequestProcessor	process;
	ServerSocket* 		serverSocket;
	SocketInfo			tmp;

	int epollTimeOut 	= 0;

	while (!stop)
	{
		int nFds = _epoll.wait(epollTimeOut);

		if (nFds < 0)
		{
            this->clear();
			return ;
		}
		for (int i = 0; i < nFds; i++)
		{
			int currentFd = _epoll.getEvents()[i].data.fd;

            serverSocket = getServerSocket(currentFd);
			// 1. Socket's type is server
			if (serverSocket)
			{
				tmp = serverSocket->accept();
				if (tmp.fd < 0)
					return ;
				_epoll.registerFd(tmp.fd, EPOLLIN | EPOLLET);
				_clients[tmp.fd];
				tmp.serverFd = serverSocket->getSocketFd();
				_clients[tmp.fd].setClientInfo(tmp);
				std::cout << "[" << tmp.fd << "]: new client from server fd: " << tmp.serverFd << std::endl;
			}

			// 2. Socket's type is client
			else
			{
				// 3. An error occurs in socket event
				if ((_epoll.getEvents()[i].events & (EPOLLERR | EPOLLHUP)))
				{
					removeSocket(currentFd);
					std::cout << "[" << currentFd << "]: disconnected" << std::endl;
					continue;
				}

				//4. Ready to read
				if (_epoll.getEvents()[i].events & EPOLLIN)
				{
					ClientSocket client_socket(_clients[currentFd]);
					
					if (!readClientBuffer(client_socket))
						continue;
					std::cout << "POLLIN" << std::endl;
					process.processRequest(_clients[currentFd]);
					_epoll.modify(client_socket.getSocketFd(), EPOLLOUT);
					std::cout << "[" << client_socket.getSocketFd() << "]: read successfull" << std::endl;
				}
	
				//5. Ready to write
				else if (_epoll.getEvents()[i].events & EPOLLOUT)
				{
					 // because there are not request process strategy implemented 
					ClientSocket client_socket(_clients[currentFd]);
					
					// Client		_clients[currentFd];

					/**========================================================================
					 * todo                             TODO
					 *   - set client_socket buffer by client.getResponse();
					 *   - set client_socket buffer size by client.getResponse size
					 *========================================================================**/
					std::cout << "POLLOUT" << std::endl;
					http(client_socket); // this is an example of response because there are not strategy request implemented

					writeClientResponse(client_socket);
					std::cout << "[" << currentFd << "]: write successfull" << std::endl;
				}
			}
		}
	
		/**
		 * To enforce request header and body timeouts, the server iterates over each client, 
		 * comparing the client's start time with the current time and 
		 * checking against the configuration parameters (header_timeout and body_timeout).
		 * Safe client cleanup procedure:
		*   1. Copy the client's file descriptor to a temporary variable.
		*      This allows erasing the client without breaking the iteration.
		*   2. Verify whether the client's deadline (header or body timeout) has been reached.
		*   3. Erase the client from the container only after the iterator has been
		*      incremented, preventing undefined behavior.
		*/
		std::map<int, Client>::iterator it = _clients.begin();
		while (!_clients.empty() && it != _clients.end())
		{
			int tmp;

			bool check = verify_deadline_ms(it->second.getStartTime(), 50000); 
			if (it->second.req.getParseState() != COMPLETE && check)
			{
				std::cout << "nbr: " << _clients.size() << std::endl;
				tmp = it->first;
				it++;
				removeSocket(tmp);
				std::cout << "[" << tmp << "]: timeout" << std::endl;
			}
			else
				++it;
		}
	}
}


bool Webserv::readClientBuffer(ClientSocket& client)
{
	int				maxReadBytes = MAXREADBYTES;
	Client			clientHttp = _clients[client.getSocketFd()];
	bool			end = false;

	client.setBlocking(false);
	while (true)
	{
		std::string data;

		data = client.recv(maxReadBytes, &end);
		if (end)
			return (false);
		if (data.empty())
		{
			std::cout << "Client disconnected: "<< client.getSocketFd()  << std::endl;
			removeSocket(client.getSocketFd());
			std::cout << "Numbers of client: " << _clients.size() << std::endl;
			return (false);
		}
		client.appendBuffer(data);
		try
		{
			HttpRequestParser	parse;
			parse.parse(clientHttp.req);
			// return (true); //temporary  prevent infinity loop
		}
		catch(const ServerException& e)
		{
			clientHttp.res.setStatusCode(e.getCode());
			clientHttp.setEndpointType(ERROR);
			return (true);
		}
	}
	return (true);
}

void	Webserv::writeClientResponse(ClientSocket& client)
{
	size_t bytesSend = 0;
	std::string	response = client.getBuffer();
	size_t responseSize = response.size();

	while (true)
	{
		ssize_t len = 0;

		try
		{
			len = client.send(response);
			if (len <= 0)
			{
				break;
			}
			bytesSend += len;
			if (bytesSend == responseSize)
			{
				break ;
			}
			response = response.c_str() + bytesSend;
		}
		catch(const ClientSocket::Eagain& e)
		{
			std::cout << "EAGAIN" << std::endl;
			_epoll.modify(client.getSocketFd(), EPOLLOUT);
			_clients[client.getSocketFd()].getBuffer() = response;
			return ;
		}
	}
	_clients.erase(client.getSocketFd());
	client.close();
}


void Webserv::http(ClientSocket& client)
{
	// try
	// {
		// Request request(client);
		std::ostringstream oss;

		std::string body = 
			"<!DOCTYPE html>"
			"<html lang=\"fr\">"
			"<head>"
			"<meta charset=\"UTF-8\">"
			"<title>Webserv - 42 Antananarivo</title>"
			"<style>"
			"body{margin:0;padding:0;background:linear-gradient(135deg,#0f2027,#203a43,#2c5364);"
			"font-family:Arial,sans-serif;color:white;display:flex;justify-content:center;"
			"align-items:center;height:100vh;}"
			".card{background:rgba(255,255,255,0.1);backdrop-filter:blur(10px);"
			"padding:40px;border-radius:15px;text-align:center;"
			"box-shadow:0 0 30px rgba(0,0,0,0.5);width:500px;}"
			"h1{margin-bottom:10px;font-size:32px;}"
			".status{color:#22c55e;font-weight:bold;font-size:20px;}"
			".info{margin-top:20px;font-size:14px;opacity:0.85;}"
			".footer{margin-top:30px;font-size:12px;opacity:0.6;}"
			"</style>"
			"</head>"
			"<body>"
			"<div class=\"card\">"
			"<h1>Webserv Opérationnel</h1>"
			"<div class=\"status\">HTTP/1.1 200 OK</div>"
			"<div class=\"info\">"
			"<p>Projet: Webserv</p>"
			"<p>Langage: C++98</p>"
			"<p>Architecture: epoll event-driven</p>"
			"<p>Mode: Non-Blocking I/O</p>"
			"</div>"
			"<div class=\"footer\">42 Antananarivo - 2026</div>"
			"</div>"
			"</body>"
			"</html>";

		oss << "HTTP/1.1 200 OK\r\n"
			<< "Content-Type: text/html; charset=UTF-8\r\n"
			<< "Connection: close\r\n"
			<< "Content-Length: " << body.size() << "\r\n"
			<< "\r\n"
			<< body;

		std::string response = oss.str();
		client.setBuffer(response);
	// }
	// catch(const Request::InvalidMethod& e)
	// {
	// 	client.setBuffer("Invalid method");
	// 	return ;
	// }
	// catch(const Request::NotImplementedMethod& e)
	// {
	// 	client.setBuffer("501 Method not implemented");
	// 	return ;
	// }
	// catch(const Request::BadRequest& e)
	// {
	// 	client.setBuffer("400 Bad request");
	// 	return ;
	// }
	// catch(const Request::URITooLong& e)
	// {
	// 	client.setBuffer("414 Request-URI Too Long");
	// 	return ;
	// }
	// catch(const Request::NotFound& e)
	// {
	// 	client.setBuffer("404 Not found");
	// 	return ;
	// }
	// catch(const Request::LengthRequired& e)
	// {
	// 	client.setBuffer("411 Length Required");
	// 	return ;
	// }
	// catch(const Request::PayloadTooLarge& e)
	// {
	// 	client.setBuffer("413 Payload Too Large");
	// 	return ;
	// }
	// catch(const Request::UnsupportedMediaType& e)
	// {
	// 	client.setBuffer("415 Unsupported Media Type");
	// 	return ;
	// }
}
