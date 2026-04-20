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
    if (!PathUtils::isPathExist("./cgi/test.cgi") || !PathUtils::isExecutable("./cgi/test.cgi"))
        throw Forbiden();
    int pipefd[2];
    if (pipe(pipefd) == -1)
        throw InternalServerError();
    setEnv(client->getRequest());
    pid_t pid = fork();
    
    setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
    setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
    setenv("REQUEST_METHOD", "GET", 1);
    setenv("QUERY_STRING", "", 1);
    setenv("SCRIPT_NAME", "./cgi/test.cgi", 1);   // simplified
    setenv("PATH_INFO", "", 1);
    setenv("CONTENT_LENGTH", "0", 1);        // no body for GET
    setenv("CONTENT_TYPE", "", 1);
    setenv("SERVER_SOFTWARE", "MiniCGI/1.0", 1);

    if (pid == -1)
        throw InternalServerError();
    if (pid == 0)
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        execl("./cgi/test.cgi", "./cgi/test.cgi" , (char*)NULL);

        std::cerr << "Failed to execute CGI script: " << strerror(errno) << std::endl;
        exit(EXIT_SUCCESS);
    }
    else
    {
        close(pipefd[1]);
        client->setCGIInfo(pid, pipefd[0]);
        process.addProcess(pipefd[0], client->getFd());
        std::cout << GREEN << "New CGI:" << pipefd[0] << RESET << std::endl;
        epoll.registerFd(pipefd[0], EPOLLIN);
        client->setProcessingCGI(true);
        epoll.modify(client->getFd(), EPOLLOUT);
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

void            setEnv(Request *req)
{
    // setenv("REQUEST_METHOD", req->getMethod() == GET ? "GET" : req->getMethod() == POST ? "POST" : "DELETE", 1);
    // setenv("QUERY_STRING", req->getQuery().c_str(), 1);
    // setEnv("SCRIPT_NAME", req->getFullPath().c_str(), 1);
    // setEnv()
}


void DynamicStrategy::ParseCGIoutput(Client *client, std::string &response)
{

}