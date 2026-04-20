#ifndef BADREQUESTEXCEPTION_HPP
#define BADREQUESTEXCEPTION_HPP

#include "ServerException.hpp"

class BadRequestException: public ServerException {
    public:
        BadRequestException():ServerException(400, "Bad Request"){};
};

#endif
