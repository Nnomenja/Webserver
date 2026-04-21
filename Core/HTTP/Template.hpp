#ifndef TEMPLATE_HPP
#define TEMPLATE_HPP

#include <sstream>
#include <vector>
#include <string>


typedef struct s_dirEntry
{
    std::string name;
    std::string path;
    bool        isDirectory;
}   t_dirEntry;

class Template
{
    private:
        Template();
    public:
        static std::string  Error(int code, std::string message);
        static std::string  DirectoryListing(const std::vector<t_dirEntry>& entries);
    };

#endif