#ifndef IREQUESTSTRATEGY_HPP
#define IREQUESTSTRATEGY_HPP

class Client;

class IRequestStrategy {
public:
    virtual ~IRequestStrategy() {}

    virtual void process(Client* client) = 0;
};

#endif