#include "Config.hpp"

Config::Config()
{
}

Config::Config(std::string filename)
{
    fileContent = File::getFileContent(filename);
}

Config::Config(const Config& other)
{
    this->fileContent  = other.fileContent;
}

Config::~Config()
{
}

Config& Config::operator=(const Config& other)
{
    if (this != &other)
    {
        this->fileContent  = other.fileContent;
    }
    return *this;
}

Config::ConfigException::ConfigException(const std::string& msg)
    : message("ConfigException: " + msg)
{
}

const char* Config::ConfigException::what() const throw()
{
    return (message.c_str());
}

Config::ConfigException::~ConfigException() throw() {}

void Config::parseFileContent()
{
    
}