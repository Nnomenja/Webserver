#include "Config.hpp"

Config::Config() {}

Config::Config(std::string filename)
{
    fileContent = File::getFileContent(filename);
    parseFileContent();
}

Config::Config(const Config& other)
{
    this->fileContent = other.fileContent;
}

Config::~Config() {}

Config& Config::operator=(const Config& other)
{
    if (this != &other)
    {
        this->fileContent = other.fileContent;
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
    int                i = -1;
    std::istringstream iss(fileContent);
    std::string        line;

    while (std::getline(iss, line))
    {
        // ignore comments
        if (line.size() != 0)
        {
            if (line.at(0) == '#')
            {
                continue;
            }
        }
        if (line == "server:")
        {
            i++;
            serverBlocks.push_back("");
            continue;
        }
        if (i >= 0)
        {
            serverBlocks[i] += line + '\n';
        }
        if (i == -1)
        {
            throw ConfigException("unexpected line ->'" + line + "'");
        }
    }
    if (i == -1)
        throw ConfigException("No 'server:' keyword");
    n = i + 1;
    for (int i = 0; i < n; i++)
    {
        // std::cout << serverBlocks[i] << std::endl;
        parseLocationBlocks(i);
    }
}

void Config::parseLocationBlocks(int i)
{
    int                j = -1;
    int                m = 0;
    std::istringstream iss(serverBlocks[i]);
    std::string        line;

    while (std::getline(iss, line))
    {
        // std::cout << line << std::endl;
        if (line == "location:")
        {
            j++;
            serverBlockIdToLocationBlocks[i].push_back("");
            continue;
        }
        if (j >= 0)
        {
            serverBlockIdToLocationBlocks[i][j] += line + '\n';
        }
    }
    m = j + 1;
    for (int j = 0; j < m; j++)
    {
        std::cout << serverBlockIdToLocationBlocks[i][j] << std::endl;
    }
}