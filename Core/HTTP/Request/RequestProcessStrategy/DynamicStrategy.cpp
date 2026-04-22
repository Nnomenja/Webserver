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
    std::string bin = "./cgi/cgi";
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
        int    envCount   = client->getEnv().size();
        char** envp       = new char*[envCount + 1];
        char** envStrings = new char*[envCount];

        int idx = 0;
        for (std::map<std::string, std::string>::const_iterator it =
                 client->getEnv().begin();
             it != client->getEnv().end(); ++it)
        {
            std::string envStr = it->first + "=" + it->second;
            envStrings[idx]    = new char[envStr.size() + 1];
            strcpy(envStrings[idx], envStr.c_str());
            envp[idx] = envStrings[idx];
            idx++;
        }
        envp[envCount] = NULL;

        // Execute PHP
        char* args[3];
        args[0] = const_cast<char*>(bin.c_str());
        args[1] = NULL; // No additional arguments for now);
        args[2] = NULL;

        execve(args[0], args, envp);

        // Cleanup on failure
        for (int i = 0; i < envCount; i++)
        {
            std::cout << "Setting env: " << envp[i] << std::endl;
            delete[] envStrings[i];
        }
        delete[] envStrings;
        delete[] envp;
        exit(EXIT_SUCCESS);
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

void DynamicStrategy::setEnv(Client *client)
{
    std::stringstream   ss;
    Request *req = client->getRequest();

    ss << req->getContentLength();
    client->setEnv("REQUEST_METHOD", req->getMethodString().c_str());
    std::cout << "METHOD: " << req->getMethodString() << std::endl;
    client->setEnv("QUERY_STRING", req->getQuery().c_str());
    // client->setEnv("SCRIPT_NAME", req->getFullPath().c_str(), 1);
    client->setEnv("CONTENT_LENGTH", ss.str().c_str());
    client->setEnv("CONTENT_TYPE", req->getHeaderBykey("Content-Type").c_str());
    ss.str("");
    ss << client->getEndpoint().port;
    client->setEnv("SERVER_PORT", ss.str().c_str());
    client->setEnv("SCRIPT_FILENAME", req->getFullPath().c_str());
}


void DynamicStrategy::ParseCGIoutput(Client *client, std::string &response)
{
    (void)client;
    (void)response;
}