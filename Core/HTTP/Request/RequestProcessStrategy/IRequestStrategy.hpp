#ifndef IREQUESTSTRATEGY_HPP
#define IREQUESTSTRATEGY_HPP

#include "../../../Network/Epoll.hpp"
#include "../../../../Data/Process.hpp"
#include "../../Webserv.hpp"

class Client;
class Epoll;
class IRequestStrategy {
public:
    virtual ~IRequestStrategy() {}

    virtual void process(Client* client, Epoll &epoll, Process &process) = 0;
};

#endif