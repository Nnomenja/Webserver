#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "../utils/utils.hpp"
#include "Validator.hpp"

#include <vector>
#include <exception>
#include <sstream>
#include <map>

enum HttpMethod
{
    GET    = 1,  // 001
    POST   = 2,  // 010
    DELETE = 4   // 100
};


typedef struct UnitConf
{
    std::string host;
    int port;
    int methods;
} UnitConf_t;

class Config
{
private:
    std::string fileContent;
    int n;
    std::vector<std::string> serverBlocks;
    std::vector<UnitConf_t> configs;
public:
    Config();
    Config(std::string filename);
    Config(const Config& other);
    ~Config();

    Config& operator=(const Config& other);

    class ConfigException : public std::exception
    {
    private:
        std::string message;

    public:
        ConfigException(const std::string& msg);
        virtual const char* what() const throw();
        virtual ~ConfigException() throw();       
    };

    void parseFileContent();
    void parseServerBlock(std::string serverBlock, int j);

    void checkPorts();


    std::vector<UnitConf_t> getConfigs() const;
    int getN() const;
};

#endif
