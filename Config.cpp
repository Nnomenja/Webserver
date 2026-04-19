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
    this->configs = other.configs;
}

Config::~Config() {}

Config& Config::operator=(const Config& other)
{
    if (this != &other)
    {
        this->fileContent = other.fileContent;
        this->configs = other.configs;
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
        skipLeadingSpaces(line);
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
    for (int j = 0; j < n; j++)
    {
        UnitConf_t u;
        u.host             = "-";
        u.port             = -1;
        u.enable_virtual_hosting = -1;
        u.root = "";
        // here
        configs.push_back(u);
    }
    for (int j = 0; j < n; j++)
    {
        parseServerBlock(extractMainConfig(serverBlocks[j]), j);
    }
    for (int i = 0; i < n; i++)
    {
        std::cout << extractMainConfig(serverBlocks[i]) << std::endl;
        // parseLocationBlocks(i);
    }
    for (int j = 0; j < n; j++)
    {
        if (configs[j].host == "-")
            throw ConfigException("host field should be filled");
        if (configs[j].port == -1)
            throw ConfigException("port field should be filled");
        if (configs[j].root == "")
            throw ConfigException("root field should be filled");
        // here
    }

    checkPorts();
}

std::string Config::extractMainConfig(std::string serverBlock)
{
    std::string res;
    std::istringstream iss(serverBlock);
    std::string line;
    while (std::getline(iss, line))
    {
        if (line == "location:")
            break;
        res += line + '\n';
    }
    return (res);
}

void Config::parseServerBlock(std::string serverBlock, int j)
{
    std::istringstream iss(serverBlock);
    std::string line;
    std::map<std::string, int> counts;
    counts["host"] = 0;
    counts["port"] = 0;
    counts["enable_virtual_hosting"] = 0;
    counts["root"] = 0;
    // here

    int i;
    while (std::getline(iss, line))
    {
        std::istringstream iss_(line);
        std::string word;
        std::string currentKey;
        i = 0;
        while (std::getline(iss_, word, ' '))
        {
            if (word == "location:")
                return;
            if (i == 0)
            {
                if (word != "host" && word != "port" && word != "enable_virtual_hosting" && word != "root") // here
                {
                    throw ConfigException("wrong key -> '" + word + "'");
                }
                counts[word]++;
                currentKey = word;
            }
            else
            {
                if (currentKey == "host")
                {
                    if (!Validator::validateHost(word))
                        throw ConfigException("wrong value -> '" + word + "'");
                    configs[j].host = word;
                    if (i > 1)
                        throw ConfigException("host can have only one value");
                }
                if (currentKey == "port")
                {
                    if (!Validator::validatePort(word))
                        throw ConfigException("wrong value -> '" + word + "'");
                    configs[j].port = std::atoi(word.c_str());
                    if (i > 1)
                        throw ConfigException("port can have only one value");
                }
                if (currentKey == "enable_virtual_hosting")
                {
                    if (!Validator::validateBoolStr(word))
                        throw ConfigException(
                            "enable_virtual_hosting should be on/ON or off/OFF");
                    if (word == "ON" || word == "on")
                        configs[j].enable_virtual_hosting = 1;
                    if (word == "off" || word == "OFF")
                        configs[j].enable_virtual_hosting = 0;
                    if (i != 1)
                        throw ConfigException(
                            "enable_virtual_hosting can have only one value");
                }
                if (currentKey == "root")
                {
                    if (!Validator::validateRoot(word))
                        throw ConfigException("root directory should exist");
                    configs[j].root = word;
                    if (i > 1)
                        throw ConfigException("root can have only one value");
                }
                // here
                // std::cout << word << std::endl;
            }
            i++;
        }
    }
    if (counts["host"] > 1)
        throw ConfigException("duplicate keys -> 'host'");
    if (counts["port"] > 1)
        throw ConfigException("duplicate keys -> 'port'");
    if (counts["enable_virtual_hosting"] > 1)
        throw ConfigException("duplicate keys -> 'enable_virtual_hosting'");
    if (counts["root"] > 1)
        throw ConfigException("duplicate keys -> 'root'");
    // here
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

void Config::checkPorts()
{
    std::vector<UnitConf_t> configs_cpy;
    configs_cpy = configs;
    int count;
    for (std::vector<UnitConf_t>::iterator it_cpy = configs_cpy.begin(); it_cpy != configs_cpy.end(); it_cpy++)
    {
        count = 0;
        for (std::vector<UnitConf_t>::iterator it = configs.begin(); it != configs.end(); it++)
        {
            if (it_cpy->port == it->port)
                count++;
        }
        if (count != 1)
            throw ConfigException("duplicate port");
    }
}