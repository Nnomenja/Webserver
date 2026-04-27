#include "Webserv.hpp"
#include "./Request/HttpRequestParser.hpp"
#include "../../Data/Request.hpp"
#include "../../Data/Client.hpp"
#include "../../Exception/RequestTimeout.hpp"
#include "./Request/RequestProcessor.hpp"
#include "./Request/RequestProcessStrategy/DynamicStrategy.hpp"
#include "./CgiParser.hpp"
#include <signal.h>
#include <string.h>
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
		for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		{
				delete it->second;
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
/*                             Other Methods                                  */
/* ************************************************************************** */


void Webserv::removeClientHttp(int fd)
{
	::close(fd);
	delete  _clients[fd];
	_clients.erase(fd);
	_epoll.unregister(fd);
}

bool Webserv::init()
{
    if (_isAlreadyInit)
        return (true);
	// REAL ->
    try
    {
        Config configTmp(_fileConfigName);
        _config = configTmp;
    }
    catch(const Config::ConfigException& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return (false);
    }
	// <- REAL

    if (!createServerSockets())
	{
        return (false);
	}

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
    _isAlreadyInit = true;
    return (true);
}

bool Webserv::createServerSockets( void )
{

    _serverSocketsNumber = _config.getN();

	std::vector<UnitConf_t> configs = _config.getConfigs();

    for (int i = 0; i < _serverSocketsNumber; i++)
    {
        ServerSocket *tmp = new ServerSocket;
        if (!tmp)
            return (false);
        _serverSockets.push_back(tmp);

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
				_epoll.registerFd(tmp.fd, EPOLLIN);

				_clients[tmp.fd] = new Client(_envs);
				_clients[tmp.fd]->setFd(tmp.fd);
				_clients[tmp.fd]->setEndpoint(_config.findEndpointByFd(tmp.serverFd));
			}

			// 2. Socket's type is client
			else
			{
				Client *client = NULL;
				/**============================================
				 *               SOCKET ERROR
				 *=============================================**/
				if ((_epoll.getEvents()[i].events & (EPOLLERR | EPOLLHUP)))
				{
					if (_process.isProcess(currentFd))
					{
						client = _clients[_process.getClientFd(currentFd)];
						int status;

						waitpid(client->getCGIPid(), &status, WNOHANG);
						_process.removeProcess(currentFd);
						client->endProcessingCGI();
						DynamicStrategy::readCgiOutput(client);
						if (!client->getCGIOutput().size() || (WIFSIGNALED(status) && WTERMSIG(status) == SIGKILL))
						{
							DynamicStrategy::error(client, _epoll, _process, ServerException(504, "Gateway Timeout"));
							continue;
						}
						CgiParser cgi_parser(client->getResponse()->getCgiResponse(), client->getResponse());
						try
						{
							cgi_parser.parse();
						}
						catch(const std::exception& e)
						{
							DynamicStrategy::error(client, _epoll, _process, ServerException(504, "Gateway Timeout"));
							continue;
						}
						_epoll.remove(currentFd);
						_epoll.modify(client->getFd(), EPOLLOUT);
					}
					else
					{
						removeClientHttp(currentFd);
					}
					continue;
				}

				/**============================================
				 *               SOCKET READ
				 *=============================================**/

				if (_epoll.getEvents()[i].events & EPOLLIN)
				{
					try
					{
						if (_process.isProcess(currentFd))
						{
							client = _clients[_process.getClientFd(currentFd)];
							DynamicStrategy::readCgiOutput(client);
							continue;
						}
						else
						{
							 client = _clients[currentFd];
							if (!readtHttpRequest(client))
								continue;
							RequestProcessor	process;
							process.processRequest(_clients[currentFd], _epoll, _process);
						}
					}
					catch(const ServerException& e)
					{
						ErrorProcess::processError(e, _clients[currentFd]);
					}
					_epoll.modify(client->getFd(), EPOLLOUT);
				}
				/**============================================
				 *               SOCKET WRITE
				 *=============================================**/
				else if (_epoll.getEvents()[i].events & EPOLLOUT)
				{
					if (_clients.find(currentFd) == _clients.end())
						continue;
					client = _clients[currentFd];
					if (client->isProcessingCGI())
					{
						if (_process.isTimeout(CGI_TIMEOUT, client->getCGIfd()))
						{
							kill(client->getCGIPid(), SIGKILL);
							DynamicStrategy::error(client, _epoll, _process, ServerException(504, "Gateway Timeout"));
						}
						else
							continue;
					}			
					client->generateResponse();
					sendHttpResponse(client);
					removeClientHttp(client->getFd());
				}
			}
		}

		std::map<int, Client*>::iterator it = _clients.begin();
		while (!_clients.empty() && it != _clients.end())
		{
			int tmp;
			bool check;
			ARequestParserState *state = it->second->getRequest()->getParserState();
			RequestParserStateName stateName = METHOD;

			if (state)
				stateName = state->getParserStateName();
			if (stateName != BODY)
				check = verify_deadline_ms(it->second->getStartTime(), REQUEST_HEADER_TIMEOUT_MS);
			else
				check = verify_deadline_ms(it->second->getStartTime(), REQUEST_BODY_TIMEOUT_MS);
			if (!it->second->isParsed() && check)
			{
				tmp = it->first;
				_epoll.modify(it->first, EPOLLOUT);
				ErrorProcess::processError(RequestTimeout(), _clients[tmp]);
			}
			++it;
		}		
	}	
}


bool Webserv::readtHttpRequest(Client* client)
{
	bool			end = false;
	std::string		data;
	std::string		contents;

	while (true)
	{
		data = _epoll.read(client->getFd(), &end);
		client->getRequest()->setBuffer(data);
		if (data.empty())
		{
			removeClientHttp(client->getFd());
			return (false);
		}

		HttpRequestParser	parse;
		parse.parse(client->getRequest(), client->getEndpoint());

		if (parse.finished())
		{
			client->parsed();
			return (true);
		}
		if (end)
			return (false);
	}
	return (true);
}

void Webserv::sendHttpResponse(Client *client)
{
	_epoll.send(client->getFd(), client->getResponseHttp(), client->getResponseHttpSize());
}
