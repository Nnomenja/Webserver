#include "helpers.hpp"

void skipLeadingSpaces(std::string& str)
{
    std::string::size_type i = 0;

    while (i < str.length() && (str[i] == ' ' || str[i] == '\t'))
        ++i;
    str.erase(0, i);
}

int countWords(const std::string& str)
{
    std::istringstream iss(str);
    std::string        word;
    int                count = 0;

    while (iss >> word)
        count++;

    return count;
}

void fillMap(std::map<std::string, std::string>& m, std::string s)
{
    if (m.empty())
        return;
    for (std::map<std::string, std::string>::iterator it = m.begin();
         it != m.end(); ++it)
    {
        it->second = s;
    }
}

void fillMap(std::map<int, std::string>& m,
             const std::string& s,
             const std::vector<std::string>& codes)
{
    for (std::vector<std::string>::const_iterator it = codes.begin();
         it != codes.end();
         ++it)
    {
        int code = std::atoi(it->c_str());
        m[code] = s;
    }
}

void printMap(std::map<int, std::string>& m)
{
    for (std::map<int, std::string>::const_iterator it = m.begin();
         it != m.end(); ++it)
    {
        std::cout << it->first << " => " << it->second << std::endl;
    }
}

void printVector(const std::vector<std::string>& v)
{
    for (std::vector<std::string>::const_iterator it = v.begin(); it != v.end();
         ++it)
    {
        std::cout << *it << std::endl;
    }
}