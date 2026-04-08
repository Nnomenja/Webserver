#ifndef METHODNOTALLOWED_HPP
#define METHODNOTALLOWED_HPP

#include "ServerException.hpp"

class MethodNotAllowed: public ServerException {
    public:
        MethodNotAllowed():ServerException(405, "Method Not Allowed"){};
};

#endif
