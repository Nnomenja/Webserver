#include "Validator.hpp"

Validator::Validator()
{
}

Validator::Validator(const Validator& other)
{
    (void)other;
}

Validator::~Validator()
{
}

Validator& Validator::operator=(const Validator& other)
{
    if (this != &other)
    {
        // copy members here
        (void)other;
    }
    return *this;
}


bool Validator::validateHost(std::string host)
{
    if (isIpV4(host))
        return (1);
    return (0);
}



bool Validator::validatePort(std::string port)
{
    if (!isPositiveInt(port))
        return (0);
    double value = std::strtod(port.c_str(), NULL);
    if (value > 65535 && value < 1024)
        return (0); 
    return (1);
}

bool Validator::validateMethod(std::string method)
{
    if (method != "GET" && method != "POST" && method != "DELETE")
        return (0);
    return (1);
}