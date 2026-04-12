#ifndef TEMPLATE_HPP
#define TEMPLATE_HPP

#include <sstream>
#include <string>

class Template
{
    private:
        Template();
    public:
        static std::string Error(int code, std::string message);
};

#endif