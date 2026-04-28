#include "File.hpp"

File::File() {}

File::File(const File& other)
{
    (void)other;
}

File::~File() {}

File& File::operator=(const File& other)
{
    if (this != &other)
    {
        (void)other;
    }
    return *this;
}

std::string File::getFileContent(std::string fileName)
{
    std::ifstream file(fileName.c_str());
    std::string   line;
    std::string   fileContent = "";
    if (!file.is_open())
    {
        throw std::runtime_error("Error: could not open file.");
    }
    while (std::getline(file, line))
    {
        fileContent += line + "\n";
    }
    file.close();
    return (fileContent);
}