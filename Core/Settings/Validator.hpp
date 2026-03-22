#ifndef VALIDATOR_HPP
#define VALIDATOR_HPP

#include "../../utils/utils.hpp"

class Validator
{
private:
    Validator();
    Validator(const Validator& other);
    Validator& operator=(const Validator& other);
public:
    ~Validator();

    static bool validateHost(std::string host);
    static bool validatePort(std::string port);
    static bool validateMethod(std::string method);
    // here
    

};

#endif
