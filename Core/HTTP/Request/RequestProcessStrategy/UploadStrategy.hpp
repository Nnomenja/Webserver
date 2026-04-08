#ifndef STRATEGYSTRATEGY_HPP
#define STRATEGYSTRATEGY_HPP

#include "IRequestStrategy.hpp"
#include <iostream>

class UploadStrategy : public IRequestStrategy {
public:
    void process(Client* client);
};

#endif