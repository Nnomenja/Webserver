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
    std::string bin = client->getCGIbin();
    std::cout << "CGI bin: " << bin << std::endl;
    if (!PathUtils::isPathExist(bin) || !PathUtils::isExecutable(bin))
        throw Forbiden();
    int pipeIn[2];
    int pipeOut[2];

    if (pipe(pipeIn) == -1)
        throw InternalServerError();
    if (pipe(pipeOut) == -1)
    {
        close(pipeIn[0]);
        close(pipeIn[1]);
        throw InternalServerError();
    }
    setEnv(client);
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

        char **envp =  buildEnvp(client->getEnv());
        char* args[3];
        args[0] = const_cast<char*>(bin.c_str());
        args[1] = const_cast<char*>(client->getRequest()->getPathname().c_str());
        args[2] = NULL;
        execve(args[0], args, envp);

        // Cleanup on failure
        // for (int i = 0; i < envCount; i++)
        // {
        //     std::cout << "Setting env: " << envp[i] << std::endl;
        //     delete[] envStrings[i];
        // }
        // delete[] envStrings;
        // delete[] envp;
        exit(EXIT_FAILURE);
    }
    else
    {
        close(pipeIn[0]);
        close(pipeOut[1]);
        write(pipeIn[1], client->getRequest()->getBody()._str_buffer.c_str(),  client->getRequest()->getBody()._content_length);
        close(pipeIn[1]);
        client->setCGIInfo(pid, pipeOut[0]);
        process.addProcess(pipeOut[0], client->getFd());
        std::cout << GREEN << "New CGI:" << pipeOut[0] << RESET << std::endl;
        epoll.registerFd(pipeOut[0], EPOLLIN);
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
    client->setEnv("QUERY_STRING", req->getQuery().c_str());
    client->setEnv("CONTENT_LENGTH", ss.str().c_str());
    client->setEnv("CONTENT_TYPE", req->getHeaderBykey("Content-Type").c_str());
    ss.str("");
    ss << client->getEndpoint().port;
    client->setEnv("SERVER_PORT", ss.str().c_str());
    client->setEnv("SCRIPT_NAME", req->getFullPath().c_str());
}


void DynamicStrategy::ParseCGIoutput(Client *client, std::string &response)
{
    (void)client;
    (void)response;
}

char **DynamicStrategy::buildEnvp(const std::map<std::string, std::string> &env)
{
    int    envCount   = env.size();
    char** envp       = new char*[envCount + 1];
    char** envStrings = new char*[envCount];

    int idx = 0;
    for (std::map<std::string, std::string>::const_iterator it = env.begin(); it != env.end(); ++it)
    {
        std::string envStr = it->first + "=" + it->second;
        envStrings[idx]    = new char[envStr.size() + 1];
        strcpy(envStrings[idx], envStr.c_str());
        envp[idx] = envStrings[idx];
        idx++;
    }
    envp[envCount] = NULL;
    return (envp);
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