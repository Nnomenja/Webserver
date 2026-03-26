#ifndef ERRORSTRATEGY_HPP
#define ERRORSTRATEGY_HPP

#include "IRequestStrategy.hpp"
#include <iostream>

class ErrorStrategy : public IRequestStrategy {
public:
    void process(Client* client);
};

#endif