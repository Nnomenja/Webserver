#ifndef PATHUTILS_HPP
#define PATHUTILS_HPP

#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <fstream>

class PathUtils
{
    public:
        static bool isPathExist(const std::string& path);
        static bool isFileReadable(const std::string& path);
        static bool isDirectory(const std::string &path);
        static bool isTraversableDir(const std::string& path);
        static std::string getFileContentbypath(std::string pathname);

};


#endif