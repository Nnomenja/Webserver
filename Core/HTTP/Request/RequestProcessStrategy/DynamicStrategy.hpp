#ifndef DYNAMICSTRATEGY_HPP
#define DYNAMICSTRATEGY_HPP

#include "IRequestStrategy.hpp"
#include <iostream>
#include "../../../../Exception/ServerException.hpp"
class DynamicStrategy : public IRequestStrategy {
public:
    void            process(Client *client, Epoll &epoll, Process &process);
    void            setEnv(Client *client);
    char            **buildEnvp(const std::map<std::string, std::string> &env);

    static void    error(Client *client, Epoll &epoll, Process &process, ServerException e);
    static void    ParseCGIoutput(Client *client, std::string &response);
};

#endif