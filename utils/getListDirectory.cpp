
#include "utils.hpp"



#include <string>
#include <sstream>

std::string getPath(const std::string& request)
{
    std::istringstream ss(request);
    std::string method, target, version;

    ss >> method >> target >> version;
    return (target);
}

bool isDirectory(const std::string &path)
{
    struct stat st;
    if (stat(path.c_str(), &st) == -1)
        return (false);
    return (S_ISDIR(st.st_mode));
}

std::string getListDirectory(const std::string &host, int port, const std::string &path)
{
    (void)path;
    std::string root = std::string("test/") + std::string("a");
    
    if (root.find("..") != std::string::npos)
    {
        return ("Error");
    }

    DIR *dir = opendir(root.c_str());
    if (!dir)
        return ("Cannot open directory");
    std::stringstream html;
    html << "<html>\n<body>\n";
    html << "<h1>Directory listing for " << "/a/" << "</h1>\n<hr>\n<ul>\n";
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        std::string name = entry->d_name;
        if (isDirectory(root + "/" + std::string(entry->d_name)))
            name += "/";
        if (name == "./" || name == "../")
            continue;
        html << "<li><a href=\"http://" << host << ":" << port << "/" << "a" << "/" << name << "\">"
             << name << "</a></li>\n";
    }
    html << "</ul>\n<hr>\n</body>\n</html>\n";
    closedir(dir);
    return (html.str());
}