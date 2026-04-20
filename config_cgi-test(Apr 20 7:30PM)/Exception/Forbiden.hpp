#ifndef FORBIDEN_HPP
#define FORBIDEN_HPP

#include "ServerException.hpp"

class Forbiden: public ServerException {
    public:
    Forbiden():ServerException(403, "Forbiden"){};
};

#endif
