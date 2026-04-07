#ifndef ERRORPROCESS_HPP
#define ERRORPROCESS_HPP

#include "../../Exception/ServerException.hpp"

class Client;
class ErrorProcess
{
    public:
        static void processError(const ServerException &e, Client *client);
}; 

#endif