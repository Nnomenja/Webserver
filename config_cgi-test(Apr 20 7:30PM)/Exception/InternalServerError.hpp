#ifndef INTERNALSERVERERROR_HPP
#define INTERNALSERVERERROR_HPP

#include "ServerException.hpp"

class InternalServerError: public ServerException {
    public:
        InternalServerError():ServerException(500, "Internal Server Error"){};
};

#endif
