#ifndef PAYLOADTOOLARGE_HPP
#define PAYLOADTOOLARGE_HPP

#include "ServerException.hpp"

class PayloadTooLarge: public ServerException {
    public:
        PayloadTooLarge():ServerException(413, "Payload Too Large"){};
};

#endif
