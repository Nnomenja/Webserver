#ifndef STATICSTRATEGY_HPP
#define STATICSTRATEGY_HPP

#include "IRequestStrategy.hpp"
#include <iostream>

class StaticStrategy : public IRequestStrategy {
public:
    void process(Client* client, Epoll &epoll, Process &process);
};

#endif