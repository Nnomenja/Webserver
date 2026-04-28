#include "Config.hpp"

#include <set>

Config::Config() {
    
}

Config::Config(std::string filename)
{
    serverParsed = -1;
    locationParsed = -1;
    try
    {
        fileContent = File::getFileContent(filename);
    }
    catch(const std::exception& e)
    {
        fileContent = "";
    }
    parseFileContent();
}

Config::Config(const Config& other)
{
    this->fileContent = other.fileContent;
    this->n = other.n;
    this->serverBlocks = other.serverBlocks;
    this->configs = other.configs;
    this->serverBlockIdToLocationBlocks = other.serverBlockIdToLocationBlocks;
    this->serverParsed = other.serverParsed;
}

Config::~Config() {}

Config& Config::operator=(const Config& other)
{
    if (this != &other)
    {
        this->fileContent = other.fileContent;
        this->n = other.n;
        this->serverBlocks = other.serverBlocks;
        this->configs = other.configs;
        this->serverBlockIdToLocationBlocks = other.serverBlockIdToLocationBlocks;
        this->serverParsed = other.serverParsed;
    }
    return *this;
}

Config::ConfigException::ConfigException(const std::string& msg, int serverParsed, int locationParsed)
    : message("ConfigException: " + msg), serverParsed(serverParsed), locationParsed(locationParsed)
{
}

const char* Config::ConfigException::what() const throw()
{
    return (message.c_str());
}

int Config::ConfigException::getServerParsed() const throw()
{
    return (serverParsed);
}

int Config::ConfigException::getLocationParsed() const throw()
{
    return (locationParsed);
}

Config::ConfigException::~ConfigException() throw() {}

void Config::parseFileContent()
{
    if (fileContent == "")
        throw ConfigException("Error opening file", serverParsed, locationParsed);
    int                i = -1;
    std::istringstream iss(fileContent);
    std::string        line;

    while (std::getline(iss, line))
    {
        skipLeadingSpaces(line);

        if (line.size() != 0)
        {
            if (line.at(0) == '#')
            {
                continue;
            }
        }
        if (line.size()== 0)
        {
            continue;
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
            throw ConfigException("unexpected line ->'" + line + "'", serverParsed, locationParsed);
        }
    }
    if (i == -1)
        throw ConfigException("No 'server:' keyword", serverParsed, locationParsed);
    n = i + 1;
    for (int j = 0; j < n; j++)
    {
        UnitConf_t u;
        u.host                   = "-";
        u.port                   = -1;
        u.enable_virtual_hosting = false;
        u.root                   = "";
        u.max_body_size          = 52428800;
        u.server_name            = "";

        configs.push_back(u);
    }
    for (int j = 0; j < n; j++)
    {
        parseServerBlock(extractMainConfig(serverBlocks[j]), j);
    }
    for (int i = 0; i < n; i++)
    {
        locationParsed = -1;
        getLocationBlocks(i);
    }
    for (int j = 0; j < n; j++)
    {
        if (configs[j].host == "-")
            throw ConfigException("host field should be filled", serverParsed, locationParsed);
        if (configs[j].port == -1)
            throw ConfigException("port field should be filled", serverParsed, locationParsed);
        if (configs[j].root == "")
            throw ConfigException("root field should be filled", serverParsed, locationParsed);
    }

    checkPorts();
    checkServerNames();
    checkVirtualHosting();
}

std::string Config::extractMainConfig(std::string serverBlock)
{
    std::string        res;
    std::istringstream iss(serverBlock);
    std::string        line;
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
    std::istringstream         iss(serverBlock);
    std::string                line;
    std::map<std::string, int> counts;
    counts["host"]                   = 0;
    counts["port"]                   = 0;
    counts["enable_virtual_hosting"] = 0;
    counts["root"]                   = 0;
    counts["max_body_size"]          = 0;
    counts["error_pages"]            = 0;
    counts["server_name"]            = 0;

    int i;
    while (std::getline(iss, line))
    {
        int                wordCount = countWords(line);
        std::istringstream iss_(line);
        std::string        word;
        std::string        currentKey;
        i = 0;

        std::vector<std::string> error_pages_words;

        while (iss_ >> word)
        {
            if (i == 0)
            {
                if (word != "host" && word != "port" &&
                    word != "enable_virtual_hosting" && word != "root" &&
                    word != "max_body_size" && word != "error_pages" &&
                    word != "server_name")
                {
                    throw ConfigException("wrong key -> '" + word + "'", serverParsed, locationParsed);
                }
                counts[word]++;
                currentKey = word;
            }
            else
            {
                if (currentKey == "host")
                {
                    if (!Validator::validateHost(word))
                        throw ConfigException("wrong value -> '" + word + "'", serverParsed, locationParsed);
                    configs[j].host = word;
                    if (i > 1)
                        throw ConfigException("host can have only one value", serverParsed, locationParsed);
                }
                if (currentKey == "port")
                {
                    if (!Validator::validatePort(word))
                        throw ConfigException("wrong value -> '" + word + "'", serverParsed, locationParsed);
                    configs[j].port = std::atoi(word.c_str());
                    if (i > 1)
                        throw ConfigException("port can have only one value", serverParsed, locationParsed);
                }
                if (currentKey == "enable_virtual_hosting")
                {
                    if (!Validator::validateBoolStr(word))
                        throw ConfigException("enable_virtual_hosting should "
                                              "be on/ON or off/OFF", serverParsed, locationParsed);
                    if (word == "ON" || word == "on")
                        configs[j].enable_virtual_hosting = 1;
                    if (word == "off" || word == "OFF")
                        configs[j].enable_virtual_hosting = 0;
                    if (i != 1)
                        throw ConfigException(
                            "enable_virtual_hosting can have only one value", serverParsed, locationParsed);
                }
                if (currentKey == "root")
                {
                    std::string err;
                    if (!Validator::validatePath(word, err))
                        throw ConfigException(err, serverParsed, locationParsed);
                    configs[j].root = word;
                    if (i > 1)
                        throw ConfigException("root can have only one value", serverParsed, locationParsed);
                }
                if (currentKey == "max_body_size")
                {
                    if (!Validator::validateMaxBodySize(word))
                        throw ConfigException(
                            "max_body_size should be less than 52428800", serverParsed, locationParsed);
                    configs[j].max_body_size = std::atoi(word.c_str());
                    if (i != 1)
                        throw ConfigException(
                            "max_body_size can have only one value", serverParsed, locationParsed);
                }
                if (currentKey == "error_pages")
                {
                    if (wordCount < 3)
                        throw ConfigException("error_pages should have at least a code and a path", serverParsed, locationParsed);
                    if (i < wordCount - 1 && i > 0)
                    {
                        if (!Validator::isErrorCode(word))
                            throw ConfigException("invalid error code", serverParsed, locationParsed);
                        error_pages_words.push_back(word);
                    }
                    if (i == wordCount - 1 && i > 0)
                    {
                        if (!Validator::validateURI(word))
                            throw ConfigException("invalid URI -> '" + word +
                                                  "'", serverParsed, locationParsed);
                        fillMap(configs[j].error_pages_map, word, error_pages_words);
                        fillVect(configs[j].error_pages, word, error_pages_words);
                        error_pages_words.clear();
                    }
                }
                if (currentKey == "server_name")
                {
                    if (!Validator::validateServerName(word))
                        throw ConfigException("invalid server name", serverParsed, locationParsed);
                    configs[j].server_name = word;
                    if (i > 1)
                        throw ConfigException(
                            "server_name can have only one value", serverParsed, locationParsed);
                }
            }
            i++;
            
        }
    }
    if (counts["host"] > 1)
        throw ConfigException("duplicate keys -> 'host'", serverParsed, locationParsed);
    if (counts["port"] > 1)
        throw ConfigException("duplicate keys -> 'port'", serverParsed, locationParsed);
    if (counts["enable_virtual_hosting"] > 1)
        throw ConfigException("duplicate keys -> 'enable_virtual_hosting'", serverParsed, locationParsed);
    if (counts["root"] > 1)
        throw ConfigException("duplicate keys -> 'root'", serverParsed, locationParsed);
    if (counts["max_body_size"] > 1)
        throw ConfigException("duplicate keys -> 'max_body_size'", serverParsed, locationParsed);
    if (counts["server_name"] > 1)
        throw ConfigException("duplicate keys -> 'server_name'", serverParsed, locationParsed);
    serverParsed = j;
}

void Config::getLocationBlocks(int i)
{
    int                j = -1;
    int                m = 0;
    std::istringstream iss(serverBlocks[i]);
    std::string        line;

    while (std::getline(iss, line))
    {
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
        t_location l;
        l.methods       = 0;
        l.path          = "";
        l.root          = "";
        l.upload_store       = "";
        l.auto_index    = false;
        l.ret.code      = -1;
        l.ret.target    = "";
        l.CGI["status"] = "OFF";
        configs[i].locations.push_back(l);
        parseLocationBlock(serverBlockIdToLocationBlocks[i][j], i, j);
        locationParsed = j;
    }
    for (int j = 0; j < m; j++)
    {
        checkLocationBlock(configs[i].locations, j);
        checkPaths(configs[i].locations);
    }
}

void Config::parseLocationBlock(std::string locationBlock, int i, int j)
{
    std::istringstream         iss(locationBlock);
    std::string                line;
    std::map<std::string, int> counts;
    counts["methods"]     = 0;
    counts["error_pages"] = 0;
    counts["path"]        = 0;
    counts["index"]       = 0;
    counts["root"]        = 0;
    counts["upload_store"]     = 0;
    counts["auto_index"]  = 0;
    counts["return"]      = 0;
    counts["CGI"]         = 0;


    std::vector<std::string> error_pages_words;

    std::string extension = "";

    int k;
    while (std::getline(iss, line))
    {
        int wordCount = countWords(line);
        if (wordCount == 1)
            throw ConfigException("invalid synthax-> '" + line + "'", serverParsed, locationParsed);
        std::istringstream iss_(line);
        std::string        word;
        std::string        currentKey;

        k = 0;

        while (iss_ >> word)
        {
            if (k == 0)
            {
                if (word != "methods" && word != "error_pages" &&
                    word != "path" && word != "index" && word != "root" &&
                    word != "upload_store" && word != "auto_index" &&
                    word != "return" && word != "CGI")
                {
                    throw ConfigException("wrong key -> '" + word + "'", serverParsed, locationParsed);
                }
                counts[word]++;
                currentKey = word;
                
            }
            else
            {
                if (currentKey == "methods")
                {
                    if (!Validator::validateMethod(word))
                        throw ConfigException(
                            "method should be GET , POST or DELETE", serverParsed, locationParsed);
                    if (word == "GET")
                        configs[i].locations[j].methods |= GET;
                    if (word == "POST")
                        configs[i].locations[j].methods |= POST;
                    if (word == "DELETE")
                        configs[i].locations[j].methods |= DELETE;
                    if (k > 3)
                        throw ConfigException(
                            "methods can have only three values", serverParsed, locationParsed);
                }
                if (currentKey == "error_pages")
                {
                    if (wordCount < 3)
                        throw ConfigException("error_pages should have at least a code and a path", serverParsed, locationParsed);
                    if (k < wordCount - 1 && k > 0)
                    {
                        if (!Validator::isErrorCode(word))
                            throw ConfigException("invalid error code", serverParsed, locationParsed);
                        error_pages_words.push_back(word);
                    }
                    if (k == wordCount - 1 && k > 0)
                    {
                        if (!Validator::validateURI(word))
                            throw ConfigException("invalid URI -> '" + word +
                                                  "'", serverParsed, locationParsed);
                        fillMap(configs[i].locations[j].error_pages_map, word, error_pages_words);
                        fillVect(configs[i].locations[j].error_pages, word, error_pages_words);
                        error_pages_words.clear();
                    }
                }
                if (currentKey == "path")
                {
                    if (!Validator::validateURI(word))
                        throw ConfigException("invalid URI -> '" + word + "'", serverParsed, locationParsed);
                    configs[i].locations[j].path = word;
                    if (k > 1)
                        throw ConfigException("path can have only one value", serverParsed, locationParsed);
                }
                if (currentKey == "index")
                {
                    if (!Validator::validateIndex(word))
                        throw ConfigException("invalid index", serverParsed, locationParsed);
                    configs[i].locations[j].index_vect.push_back(word);
                    configs[i].locations[j].index = word;
                }
                if (currentKey == "root")
                {
                    std::string err;
                    if (!Validator::validatePath(word, err))
                        throw ConfigException(err, serverParsed, locationParsed);
                    configs[i].locations[j].root = word;
                    if (k > 1)
                        throw ConfigException("root can have only one value", serverParsed, locationParsed);
                }
                if (currentKey == "upload_store")
                {
                    std::string err;
                    if (!Validator::validatePath(word, err))
                        throw ConfigException(err, serverParsed, locationParsed);
                    configs[i].locations[j].upload_store = word;
                    if (k > 1)
                        throw ConfigException(
                            "upload_store can have only one value", serverParsed, locationParsed);
                }
                if (currentKey == "auto_index")
                {
                    if (!Validator::validateBoolStr(word))
                        throw ConfigException("auto_index should "
                                              "be on/ON or off/OFF", serverParsed, locationParsed);
                    if (word == "ON" || word == "on")
                        configs[i].locations[j].auto_index = 1;
                    if (word == "off" || word == "OFF")
                        configs[i].locations[j].auto_index = 0;
                    if (k != 1)
                        throw ConfigException(
                            "auto_index can have only one value", serverParsed, locationParsed);
                }
                if (currentKey == "return")
                {
                    if (k == 1)
                    {
                        if (!Validator::isRedirectCode(word))
                            throw ConfigException("invalid return code", serverParsed, locationParsed);
                        configs[i].locations[j].ret.code =
                            std::atoi(word.c_str());
                    }
                    if (k == 2)
                    {
                        if (!Validator::isValidRedirectTarget(word))
                            throw ConfigException("invalid return target", serverParsed, locationParsed);
                        configs[i].locations[j].ret.target = word;
                        configs[i].locations[j].return_path = word;
                    }
                    if (k > 2)
                        throw ConfigException("return can have only two value", serverParsed, locationParsed);
                }
                if (currentKey == "CGI")
                {
                    if (k == 1)
                    {
                        if (!Validator::isValidCgiExtension(word))
                            throw ConfigException("invalid CGI extension", serverParsed, locationParsed);
                        extension = word;
                    }
                    if (k == 2)
                    {
                        if (!Validator::isExecutable(word))
                            throw ConfigException("invalid CGI interpreter", serverParsed, locationParsed);
                        configs[i].locations[j].CGI[extension] = word;
                        extension                              = "";
                        configs[i].locations[j].CGI["status"]  = "ON";
                    }
                    if (k > 2)
                        throw ConfigException("CGI can have only 2 values "
                                              "(file extension, executable)", serverParsed, locationParsed);
                }
            }
            k++;
            
        }
    }
    if (counts["methods"] > 1)
        throw ConfigException("duplicate keys -> 'methods'", serverParsed, locationParsed);
    if (counts["path"] > 1)
        throw ConfigException("duplicate keys -> 'path'", serverParsed, locationParsed);
    if (counts["root"] > 1)
        throw ConfigException("duplicate keys -> 'root'", serverParsed, locationParsed);
    if (counts["upload_store"] > 1)
        throw ConfigException("duplicate keys -> 'upload_store'", serverParsed, locationParsed);
    if (counts["auto_index"] > 1)
        throw ConfigException("duplicate keys -> 'auto_index'", serverParsed, locationParsed);
    if (counts["return"] > 1)
        throw ConfigException("duplicate keys -> 'return'", serverParsed, locationParsed);
}

void Config::checkLocationBlock(std::vector<t_location>& locations, int j)
{
    if (locations[j].ret.code == -1)
    {
        if (locations[j].methods == 0)
            throw ConfigException("methods field should be filled", serverParsed, locationParsed);
        if (locations[j].path == "")
            throw ConfigException("path field should be filled", serverParsed, locationParsed);
        if (locations[j].upload_store != "")
        {
            if (locations[j].methods != POST)
            {
                throw ConfigException(
                    "uploads should be made via POST requests only", serverParsed, locationParsed);
            }
            if (locations[j].auto_index)
            {
                throw ConfigException("auto_index active in mode upload", serverParsed, locationParsed);
            }
            if (locations[j].index_vect.size() != 0)
            {
                throw ConfigException("mode upload does not have index files", serverParsed, locationParsed);
            }
            if (locations[j].CGI["status"] == "ON")
            {
                throw ConfigException(
                    "mode upload does not match with active CGI", serverParsed, locationParsed);
            }
            locations[j].type = UPLOAD;
            return;
        }
        if (locations[j].CGI["status"] == "ON")
        {
            locations[j].type = DYNAMIC;
            return;
        }
    }
    else
    {
        if (locations[j].methods != 0)
            throw ConfigException("methods field should not be filled", serverParsed, locationParsed);
        if (locations[j].path == "")
            throw ConfigException("path field should be filled", serverParsed, locationParsed);
        if (locations[j].root != "")
            throw ConfigException("root field should not be filled", serverParsed, locationParsed);
        locations[j].type = REDIRECTION;
        return;
    }
    locations[j].type = STATIC;
}

void Config::checkPaths(const std::vector<t_location>& locations)
{
    std::set<std::string> seenPaths;

    for (std::vector<t_location>::const_iterator it = locations.begin();
         it != locations.end(); ++it)
    {
        if (!seenPaths.insert(it->path).second)
        {
            throw ConfigException("duplicate path", serverParsed, locationParsed);
        }
    }
}

void Config::checkPorts()
{
    std::set<int> seenPorts;

    for (std::vector<UnitConf_t>::iterator it = configs.begin();
         it != configs.end(); ++it)
    {
        if (!seenPorts.insert(it->port).second)
        {
            throw ConfigException("duplicate port", serverParsed, locationParsed);
        }
    }
}

void Config::checkServerNames()
{
    std::set<std::string> seenNames;

    for (std::vector<UnitConf_t>::iterator it = configs.begin();
         it != configs.end(); ++it)
    {
        if (it->server_name.empty())
            continue;
        if (!seenNames.insert(it->server_name).second)
        {
            throw ConfigException("duplicate server_name", serverParsed, locationParsed);
        }
    }
}

void Config::checkVirtualHosting()
{
    for (std::vector<UnitConf_t>::iterator it = configs.begin();
         it != configs.end(); ++it)
    {
        if (it->enable_virtual_hosting && it->server_name.empty())
        {
            throw ConfigException("server_name must be set when virtual hosting is enabled", serverParsed, locationParsed);
        }
    }
}

std::vector<UnitConf_t> Config::getConfigs() const
{
    return configs;
}

int Config::getN() const
{
    return n;
}

UnitConf_t Config::findEndpointByFd(int fd)
{
    return (_endpoints[fd]);
}

void Config::setEndpointByFd(UnitConf_t &value, int fd)
{
    _endpoints[fd] = value;
}