#ifndef DIRECTORYSTRATEGY_HPP
#define DIRECTORYSTRATEGY_HPP

#include "IRequestStrategy.hpp"
#include <iostream>

class DirectoryStrategy : public IRequestStrategy {
public:
    void process(Client& client);
};

#endif