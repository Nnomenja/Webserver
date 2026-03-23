#include "Config.hpp"

Config::Config()
{}

Config::Config(std::string filename)
{
    try
    {
        n = 0;
        fileContent = getFileContent(filename);
        parseFileContent();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

Config::Config(const Config& other)
{
    this->fileContent = other.fileContent;
    this->n = other.n;
    this->serverBlocks = other.serverBlocks;
    this->configs = other.configs;
}

Config::~Config()
{
}

Config& Config::operator=(const Config& other)
{
    if (this != &other)
    {
        this->fileContent = other.fileContent;
        this->n = other.n;
        this->serverBlocks = other.serverBlocks;
        this->configs = other.configs;
    }
    return *this;
}

Config::ConfigException::ConfigException(const std::string& msg) : message("ConfigException: " + msg)
{}

const char* Config::ConfigException::what() const throw()
{
    return (message.c_str());
}

Config::ConfigException::~ConfigException() throw()
{}

void Config::parseFileContent()
{
    int i = -1;
    std::istringstream iss(fileContent);
    std::string line;

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
    }
    if (i == -1)
        throw ConfigException("No 'server:' keyword");
    n = i + 1;
    for (int j = 0; j < n; j++)
    {
        UnitConf_t u;
        u.host = "-";
        u.port = -1;
        u.methods = 0;
        // here
        configs.push_back(u);
    }
    for (int j = 0; j < n; j++)
    {
        parseServerBlock(serverBlocks[j], j);
    }
    for (int j = 0; j < n; j++)
    {
        if (configs[j].host == "-")
            throw ConfigException("host field should be filled");
        if (configs[j].port == -1)
            throw ConfigException("port field should be filled");
        if (configs[j].methods == 0)
            throw ConfigException("method field should be filled");
        // here
    }
    checkPorts();
}

void Config::parseServerBlock(std::string serverBlock, int j)
{
    std::istringstream iss(serverBlock);
    std::string line;
    std::map<std::string, int> counts;
    counts["host"] = 0;
    counts["port"] = 0;
    counts["methods"] = 0;
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
            if (i == 0)
            {
                if (word != "host" && word != "port" && word != "methods") // here
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
                if (currentKey == "methods")
                {
                    if (!Validator::validateMethod(word))
                        throw ConfigException("method should be GET , POST or DELETE");
                    if (word == "GET")
                        configs[j].methods |= GET;
                    if (word == "POST")
                        configs[j].methods |= POST;
                    if (word == "DELETE")
                        configs[j].methods |= DELETE;
                    if (i > 3)
                        throw ConfigException("host can have only three values");
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
    if (counts["methods"] > 1)
        throw ConfigException("duplicate keys -> 'methods'");
    // here
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

std::vector<UnitConf_t> Config::getConfigs() const
{
    return (configs);
}

int Config::getN() const
{
    return (n);
}

UnitConf_t Config::findEndpointByFd(int fd)
{
    return (_endpoints[fd]);
}

void Config::setEndpointByFd(UnitConf_t &value, int fd)
{
    _endpoints[fd] = value;
}
