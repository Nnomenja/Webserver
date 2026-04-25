#ifndef DYNAMICSTRATEGY_HPP
#define DYNAMICSTRATEGY_HPP

#include "IRequestStrategy.hpp"
#include <iostream>
#include "../../../../Exception/ServerException.hpp"
class DynamicStrategy : public IRequestStrategy {
    public:
        void            process(Client *client, Epoll &epoll, Process &process);
        
        static bool     readCgiOutput(Client *client);
        static void     error(Client *client, Epoll &epoll, Process &process, ServerException e);
};

#endif