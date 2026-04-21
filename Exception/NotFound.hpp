#ifndef NOTFOUND_HPP
#define NOTFOUND_HPP

#include "ServerException.hpp"

class NotFound: public ServerException {
    public:
        NotFound():ServerException(404, "Not Found"){};
};

#endif
