#ifndef VALIDATOR_HPP
#define VALIDATOR_HPP

class Validator
{
public:
    Validator();
    Validator(const Validator& other);
    ~Validator();

    Validator& operator=(const Validator& other);
};

#endif
