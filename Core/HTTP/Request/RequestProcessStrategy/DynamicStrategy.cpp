#include "DynamicStrategy.hpp"
#include "../../../../Data/Client.hpp"
#include "../../../../Exception/InternalServerError.hpp"
#include "../../../../Exception/Forbiden.hpp"
#include "../../../../utils/PathUtils.hpp"
#include "../../ErrorProcess.hpp"
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <sys/wait.h>
/**
 * TODO
 * - fork()
 * - close fd[1]
 * - create CGI
 * - 
 */

void DynamicStrategy::process(Client *client, Epoll &epoll, Process &process)
{
    std::cout << "DynamicStrategy" << std::endl;
    std::string ext = client->getRequest()->getLocation().CGI.begin()->first;
    std::string bin = client->getRequest()->getLocation().CGI[ext];
    if (!PathUtils::isPathExist(bin) || !PathUtils::isExecutable(bin))
        throw Forbiden();
    int pipefd[2];
    if (pipe(pipefd) == -1)
        throw InternalServerError();
    setEnv(client);
    pid_t pid = fork();
    if (pid == -1)
        throw InternalServerError();
    if (pid == 0)
    {
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        execve(bin.c_str(), (char* const[]){(char*)bin.c_str(), NULL}, client->getEnviron());
        exit(EXIT_FAILURE);
    }
    else
    {
        write(pipefd[1], client->getRequest()->getBody()._str_buffer.c_str(),  client->getRequest()->getBody()._content_length);
        close(pipefd[1]);
        client->setCGIInfo(pid, pipefd[0]);
        process.addProcess(pipefd[0], client->getFd());
        std::cout << GREEN << "New CGI:" << pipefd[0] << RESET << std::endl;
        epoll.registerFd(pipefd[0], EPOLLIN);
        client->setProcessingCGI(true);
        // epoll.modify(client->getFd(), EPOLLOUT);
    }
    (void)client;
    (void)epoll;
    (void)process;
}

void DynamicStrategy::error(Client *client, Epoll &epoll, Process &process, ServerException e)
{
    process.removeProcess(client->getCGIOutput());
    epoll.remove(client->getCGIOutput());
    ErrorProcess::processError(e, client);
}

void DynamicStrategy::setEnv(Client *client)
{
    std::stringstream   ss;
    Request *req = client->getRequest();

    ss << req->getContentLength();
    setenv("REQUEST_METHOD", req->getMethodString().c_str(), 1);
    std::cout << "METHOD: " << req->getMethodString() << std::endl;
    setenv("QUERY_STRING", req->getQuery().c_str(), 1);
    // setEnv("SCRIPT_NAME", req->getFullPath().c_str(), 1);
    setenv("CONTENT_LENGTH", ss.str().c_str(), 1);
    setenv("CONTENT_TYPE", req->hasHeader("content-type") ? req->getHeaderBykey("content-type").c_str() : "", 1);
    ss.str("");
    ss << client->getEndpoint().port;
    setenv("SERVER_PORT", ss.str().c_str(), 1);
    setenv("SCRIPT_FILENAME", req->getFullPath().c_str(), 1);
}


void DynamicStrategy::ParseCGIoutput(Client *client, std::string &response)
{
    (void)client;
    (void)response;
}