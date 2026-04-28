#include "DynamicStrategy.hpp"
#include "../../../../Data/Client.hpp"
#include "../../../../Exception/InternalServerError.hpp"
#include "../../../../Exception/Forbiden.hpp"
#include "../../../../utils/PathUtils.hpp"
#include "../../ErrorProcess.hpp"
#include "../RequestParserState/HeaderParser.hpp"
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <sys/wait.h>

void DynamicStrategy::process(Client *client, Epoll &epoll, Process &process)
{
    int pipeIn[2];
    int pipeOut[2];
    std::string bin = client->getCGIbin();
    Request *req = client->getRequest();
    std::string script_path = req->getFullPath();

    if (!PathUtils::isPathExist(bin) || !PathUtils::isExecutable(bin))
        throw Forbiden();

    if (pipe(pipeIn) == -1)
        throw InternalServerError();
    if (pipe(pipeOut) == -1)
    {
        close(pipeIn[0]);
        close(pipeIn[1]);
        throw InternalServerError();
    }
    std::stringstream   ss;
    std::stringstream   sy;

    char *script = const_cast<char *>(script_path.c_str());
    
    sy << req->getFullPath();

    std::string method = std::string("REQUEST_METHOD=") + req->getMethodString();
    std::string server_name = std::string("SERVER_NAME=") + client->getRequest()->getServerName();
    std::string script_name = std::string("SCRIPT_NAME=") + script_path;
    std::string script_filename = std::string("SCRIPT_FILENAME=") + script_path;
    std::string path_info = std::string("PATH_INFO=") + req->getFullPath();
    std::string request_uri = std::string("REQUEST_URI=") + req->getFullPath();
    std::string host = std::string("HTTP_HOST=") + req->getHeaderBykey("host");
    std::string cookie = std::string("HTTP_COOKIE=") + req->getHeaderBykey("cookie");

    ss << client->getEndpoint().port;

    std::string port = std::string("SERVER_PORT=") + ss.str();
    
    ss.str("");
    
    std::string query = std::string("QUERY_STRING=") + req->getQuery();
    std::string contentType = std::string("CONTENT_TYPE=") + req->getHeaderBykey("Content-Type");
    
    ss << req->getContentLength();
    
    std::string contentLength = std::string("CONTENT_LENGTH=") + ss.str();
    
    char *exec_envp[] = {
        const_cast<char *>(script_filename.c_str()),
        const_cast<char *>(script_name.c_str()),
        const_cast<char *>(method.c_str()),
        const_cast<char *>(server_name.c_str()),
        const_cast<char *>("SERVER_SOFTWARE=webserver/1.0"),
        const_cast<char *>("SERVER_PROTOCOL=HTTP/1.1"),
        const_cast<char *>("REDIRECT_STATUS=200"),
        const_cast<char *>("GATEWAY_INTERFACE=CGI/1.1"),
        const_cast<char *>(path_info.c_str()),
        const_cast<char *>(port.c_str()),
        const_cast<char *>(query.c_str()),
        const_cast<char *>(host.c_str()),
        const_cast<char *>(cookie.c_str()),
        const_cast<char *>(contentType.c_str()),
        const_cast<char *>(request_uri.c_str()),
        NULL
    };

    const char* args[3];
    args[0] =   bin.c_str();
    args[1] = script;
    args[2] = 0;

    pid_t pid = fork();
    if (pid == -1)
        throw InternalServerError();
    if (pid == 0)
    {
        dup2(pipeIn[0], STDIN_FILENO);
        dup2(pipeOut[1], STDOUT_FILENO);
        close(pipeIn[0]);
        close(pipeIn[1]);
        close(pipeOut[0]);
        close(pipeOut[1]);

        execve(args[0], const_cast<char **>(args), exec_envp);
        exit(EXIT_FAILURE);
    }
    else
    {
        close(pipeIn[0]);
        close(pipeOut[1]);
        if (client->getRequest()->getBody()._str_buffer.size())
            write(pipeIn[1], client->getRequest()->getBody()._str_buffer.c_str(),  client->getRequest()->getBody()._str_buffer.size());
        if (client->getRequest()->getBody()._file_buffer.is_open())
        {
            client->getRequest()->getBody()._file_buffer.seekg(0, std::ios::beg);
            char buffer[4096];

            while (true)
            {
                client->getRequest()->getBody()._file_buffer.read(buffer, sizeof(buffer));
                std::streamsize n = client->getRequest()->getBody()._file_buffer.gcount();

                if (n <= 0)
                    break;

                write(pipeIn[1], buffer, n);
            }
        }
        close(pipeIn[1]);
        
        client->setCGIInfo(pid, pipeOut[0]);
        process.addProcess(pipeOut[0], client->getFd());
        epoll.registerFd(pipeOut[0], EPOLLIN);
        client->setProcessingCGI(true);
        epoll.modify(client->getFd(), (EPOLLOUT | EPOLLHUP));
    }
    (void)client;
    (void)epoll;
    (void)process;
}

void DynamicStrategy::error(Client *client, Epoll &epoll, Process &process, ServerException e)
{
    process.removeProcess(client->getCGIfd());
    epoll.remove(client->getCGIfd());
    ErrorProcess::processError(e, client);
}

void    destroyEnvp(char **envp, int envCount, char **envStrings)
{
    for (int i = 0; i < envCount; i++)
    {
        delete[] envStrings[i];
    }
    delete[] envStrings;
    delete[] envp;
}

bool DynamicStrategy::readCgiOutput(Client *client)
{
    char buff[CGI_MAX_OUTPUT_BYTES];
    bzero(buff, CGI_MAX_OUTPUT_BYTES);
    ssize_t n;
    
    while (1)
    {
        n = read(client->getCGIfd(), buff, CGI_MAX_OUTPUT_BYTES);
        if ((n) + client->getResponse()->getCgiResponseSize() >  CGI_MAX_OUTPUT_BYTES)
            return (false);
        if (n == -1)
            return (false);
        client->getResponse()->addCgiResponse(std::string(buff, n), n);
        if (n == 0 || n < CGI_MAX_OUTPUT_BYTES)
            return (true);
    }
    if (client->isCGIProcessEnd())
        client->setCGIOutputReaded();
    return (true);
}
