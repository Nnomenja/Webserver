#ifndef REDIRECTIONSTRATEGY_HPP
#define REDIRECTIONSTRATEGY_HPP

#include "IRequestStrategy.hpp"
#include <iostream>

class RedirectionStrategy : public IRequestStrategy {
public:
    void process(Client* client, Epoll &epoll, Process &process);
};

#endif