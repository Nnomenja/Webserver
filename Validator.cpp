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
