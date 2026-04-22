#ifndef FILE_HPP
#define FILE_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <exception>

class File
{
private:
    File();
    File(const File& other);
    File& operator=(const File& other);
public:
    ~File();
    static std::string getFileContent(std::string fileName);
};

#endif