#ifndef NOTIMPLEMENT_HPP
#define NOTIMPLEMENT_HPP

#include "ServerException.hpp"

class NotImplement: public ServerException {
    public:
        NotImplement():ServerException(501, "Not implement"){};
};

#endif
