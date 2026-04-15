#include "helpers.hpp"

void skipLeadingSpaces(std::string& str)
{
    std::string::size_type i = 0;

    while (i < str.length() && (str[i] == ' ' || str[i] == '\t'))
        ++i;
    str.erase(0, i);
}