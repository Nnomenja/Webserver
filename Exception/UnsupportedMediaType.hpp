#ifndef UNSUPPORTEDMEDIATYPE_HPP
#define UNSUPPORTEDMEDIATYPE_HPP

# include "ServerException.hpp"

class UnsupportedMediaType : public ServerException
{
    public :
        UnsupportedMediaType() : ServerException(415, "Unsupported Media Type"){ }

};

#endif /* UNSUPPORTEDMEDIATYPE_HPP */
