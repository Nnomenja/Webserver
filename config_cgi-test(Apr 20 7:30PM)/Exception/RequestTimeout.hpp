#ifndef REQUESTTIMEOUT_HPP
#define REQUESTTIMEOUT_HPP

#include "ServerException.hpp"

class RequestTimeout: public ServerException {
    public:
        RequestTimeout():ServerException(408, "Request Timeout"){};
};

#endif
