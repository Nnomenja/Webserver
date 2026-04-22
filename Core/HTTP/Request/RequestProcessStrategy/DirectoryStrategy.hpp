#ifndef DIRECTORYSTRATEGY_HPP
#define DIRECTORYSTRATEGY_HPP

#include "IRequestStrategy.hpp"
#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>

class DirectoryStrategy : public IRequestStrategy {
    public:
        void process(Client* client, Epoll &epoll, Process &process);
};

#endif