#include "utils.hpp"

bool isPositiveInt(const std::string &s)
{
    size_t i = 0;

    if (s.empty())
        return (false);
    if (i == s.size())
        return (false);
    for (; i < s.size(); i++)
        if (!std::isdigit(static_cast<unsigned char>(s[i])))
            return (false);
    std::string numStr = s;
    if (numStr.length() > 10)
        return (false);
    if (numStr.length() == 10)
    {
        if (numStr > "2147483647")
            return (false);
    }
    return (true);
}

bool isIpV4(const std::string &s)
{
    int dots = 0;
    for (std::string::const_iterator it = s.begin(); it != s.end(); it++)
    {
        if (*it == '.')
            dots ++;
    }
    int c = 0;
    std::istringstream iss(s);
    std::string word;

    while (std::getline(iss, word, '.'))
    {
        if (!isPositiveInt(word))
            return (false);
        c++;
    }
    if (c != 4 || dots != 3)
        return (false);
    return (true);
}