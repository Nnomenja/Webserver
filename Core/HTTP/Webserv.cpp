#include "Webserv.hpp"
#include "./Request/HttpRequestParser.hpp"
#include "../../Data/Request.hpp"
#include "../../Data/Client.hpp"
#include "./Request/RequestProcessor.hpp"
#include <signal.h>
#include "ErrorProcess.hpp"
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
	if (_clients.size())
	{
		std::cout << "closing" << std::endl;
		for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		{
				std::cout << "closing: " << it->second->getFd()<< std::endl;
				delete it->second;
			// if (it->second)
				// delete [] it->second;
		}
	}
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


void Webserv::removeClientHttp(int fd)
{
	::close(fd);
	delete  _clients[fd];
	_clients.erase(fd);
	_epoll.unregister(fd);
}

// SIMULATION ->

void initSimulData(std::vector<UnitConf_t> &configsSimul)
{

	/**========================================================================
	 *                           STATIC WEBSITE
	 * - GET : render index.hml
	 * - POST : 405 Method Not Allowed
	 * - DELETE : 405 Method Not Allowed
	 *========================================================================**/

    UnitConf_t u;
    t_location l;

	u.host = "0.0.0.0";
    u.port = 2000;
	u.enable_virtual_hosting = false;
    u.methods = GET + POST + DELETE;
	u.method_arr.push_back("GET");
	u.method_arr.push_back("POST");
	u.method_arr.push_back("DELETE");
	// u.type = STATIC;
	u.max_body_size = 10000000;

	l.type = STATIC;

	l.path = "/site";
	u.root = "/home/nnomenja/Desktop/42/webserver/www";
	l.auto_index = true;
	// 400
	t_error_page error1;
	error1.codes.push_back(400);
	error1.path = "/error/400.html";
	u.error_pages.push_back(error1);
	// 404
	t_error_page error2;
	error2.codes.push_back(404);
	error2.path = "/error/404.html";
	u.error_pages.push_back(error2);
	l.index = "index.html";
	u.locations.push_back(l);
	
    configsSimul.push_back(u);

	/**========================================================================
	 *                           REDIRECTION
	 * - GET : redirect 3xx
	 * - POST : 405 Method Not Allowed
	 * - DELETE : 405 Method Not Allowed
	 *========================================================================**/

	u.host = "0.0.0.0";
    u.port = 2001;
	// u.type = REDIRECTION;
	u.method_arr.push_back("GET");
	u.method_arr.push_back("POST");
	u.method_arr.push_back("DELETE");
    u.methods = GET + POST + DELETE;
	u.max_body_size = 100000;

	l.path ="/home";
	l.type = REDIRECTION;
	l.return_path = "https:google.com";
	l.root = "/home/nnomenja/Desktop/42/webserver/www";
	l.auto_index = true;
	l.index = "";
	u.locations.push_back(l);
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
	std::vector<UnitConf_t> configs;
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
		_config.setEndpointByFd(configs[i], tmp->getSocketFd());
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
        delete  _serverSockets[i];
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

			/**============================================
			 *               SOCKET ACCEPT
			 *=============================================**/
			if (serverSocket)
			{
				tmp = serverSocket->accept();
				if (tmp.fd < 0)
					return ;
				_epoll.registerFd(tmp.fd, EPOLLIN | EPOLLET);
				// {
				// 	std::cout << "---------x--------" <<std::endl;
				// 	_clients[tmp.fd];
 				// 	std::cout << "-----------x------" <<std::endl;
				// }
				
				std::cout << "--------" << tmp.fd << "---------" << _clients.size() <<std::endl;
				_clients[tmp.fd] = new Client();
				// sleep(90);
				std::cout << "-----------------" << _clients.size() << std::endl;
				_clients[tmp.fd]->setFd(tmp.fd);
				std::cout << "---------X--------" <<std::endl;
				_clients[tmp.fd]->setEndpoint(_config.findEndpointByFd(tmp.serverFd));
				std::cout << "---------X--------" << _clients.size()<<std::endl;
				std::cout << "client:" << tmp.fd << " Server: " << serverSocket->getSocketFd() << std::endl;
				std::cout << "[" << tmp.fd << "]: new client from server fd: " << tmp.serverFd << std::endl;
			}

			// 2. Socket's type is client
			else
			{

				Client *client = _clients[currentFd];

				/**============================================
				 *               SOCKET ERROR
				 *=============================================**/

				if ((_epoll.getEvents()[i].events & (EPOLLERR | EPOLLHUP)))
				{
					removeClientHttp(currentFd);
					std::cout << "[" << currentFd << "]: disconnected" << std::endl;
					continue;
				}

				/**============================================
				 *               SOCKET READ
				 *=============================================**/

				if (_epoll.getEvents()[i].events & EPOLLIN)
				{
					try
					{
						if (!readtHttpRequest(client))
							continue;
						RequestProcessor	process;
						process.processRequest(_clients[currentFd]);
						// simulateClient(client); // this is an example of response because there are not strategy request implemented
					}
					catch(const ServerException& e)
					{
						std::cout << "#####ERROR: " << e.getCode() << " " << e.getName() << std::endl;
						ErrorProcess::processError(e, _clients[currentFd]);
					}
					client->generateResponse();
					_epoll.modify(client->getFd(), EPOLLOUT);
					std::cout << "[" << client->getFd() << "]: read successfull" << std::endl;
				}
	
				/**============================================
				 *               SOCKET WRITE
				 *=============================================**/

				else if (_epoll.getEvents()[i].events & EPOLLOUT)
				{
					 // because there are not request process strategy implemented 
					
					// Client		_clients[currentFd];


					/**========================================================================
					 * todo                             TODO
					 *   - set client_socket buffer by client.getResponse();
					 *   - set client_socket buffer size by client.getResponse size
					 *========================================================================**/
					std::cout << "POLLOUT" << std::endl;
					// simulateClient(client); // this is an example of response because there are not strategy request implemented

					sendHttpResponse(client);
					removeClientHttp(client->getFd());
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

		std::map<int, Client*>::iterator it = _clients.begin();
		while (!_clients.empty() && it != _clients.end())
		{
			int tmp;

			bool check = verify_deadline_ms(it->second->getStartTime(), 500000); 
			if (!it->second->isParsed() && check)
			{
				std::cout << "nbr: " << _clients.size() << std::endl;
				tmp = it->first;
				it++;
				removeClientHttp(tmp);
				std::cout << "[" << tmp << "]: timeout" << std::endl;
			}
			else
				++it;
		}
	}	
}


bool Webserv::readtHttpRequest(Client* client)
{
	bool			end = false;
	std::string		data;
	std::string		contents;

	std::cout <<  "read client fd: " << client->getFd() << std::endl;
	while (true)
	{
		data = _epoll.read(client->getFd(), &end);
		client->getRequest()->setBuffer(data);
		std::cout << "****************************************" << std::endl;
		std::cout << data << std::endl;
		std::cout << "****************************************" << std::endl;
		if (data.empty())
		{
			std::cout << "Client disconnected: "<< client->getFd()  << std::endl;
			removeClientHttp(client->getFd());
			std::cout << "Numbers of client: " << _clients.size() << std::endl;
			return (false);
		}

		HttpRequestParser	parse;
		parse.parse(client->getRequest(), client->getEndpoint());

		if (parse.finished())
		{
			client->parsed();
			std::cout << "Finished" << std::endl;
			return (true);
		}
		if (end)
		{
			std::cout << "End buff,,,,,,," << std::endl;
			return (false);
		}
	}
	return (true);
}

void Webserv::sendHttpResponse(Client *client)
{
	_epoll.send(client->getFd(), client->getResponseHttp(), client->getResponseHttpSize());
}

void Webserv::simulateClient(Client *client)
{
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
		client->setBuffer(response);
		client->setBufferSize(response.size());
}
