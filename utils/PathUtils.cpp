#include "PathUtils.hpp"

bool PathUtils::isPathExist(const std::string& path)
{
    if (path.empty())
        return (false);
    struct stat st;
    if (stat(path.c_str(), &st) != 0)
        return (false);
    if (!S_ISREG(st.st_mode))
        return (false);
    return (true);
}

bool PathUtils::isTraversableDir(const std::string& path)
{
    return (access(path.c_str(), X_OK) == 0);
}

bool PathUtils::isFileReadable(const std::string& path)
{
    return (access(path.c_str(), R_OK) == 0);
}

bool PathUtils::isDirectory(const std::string &path)
{
    struct stat st;
    if (stat(path.c_str(), &st) == -1)
        return (false);
    return (S_ISDIR(st.st_mode));
}


std::string PathUtils::getFileContentbypath(std::string pathname)
{
    std::ifstream file(pathname.c_str());
    std::string line;
    std::string fileContent = "";
    if (!file.is_open())
        throw std::exception();
    while (std::getline(file, line))
    {
        fileContent  += line + "\n";
    }
    file.close();
    return (fileContent);
}
