#ifndef DYNAMICSTRATEGY_HPP
#define DYNAMICSTRATEGY_HPP

#include "IRequestStrategy.hpp"
#include <iostream>

class DynamicStrategy : public IRequestStrategy {
public:
    void process(Client *client);
};

#endif