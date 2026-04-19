#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "File.hpp"
#include "helpers.hpp"

#include "LocationType.hpp"

#include "Validator.hpp"

#include <stdexcept>
#include <vector>
#include <sstream>
#include <map>



enum HttpMethod
{
    GET    = 1,  // 001
    POST   = 2,  // 010
    DELETE = 4   // 100
};


typedef struct s_error_page
{
    std::vector<int>    codes;
    std::string         path;
}   t_error_page;


typedef struct s_location
{
    LocationType                type;
    std::string                 path;
    int                         methods;
    std::vector<t_error_page>   error_pages;
    std::string                 root;
    bool                        auto_index;
    std::string                 return_path;
    std::string                 index;
}   t_location;

typedef struct UnitConf
{
    std::string                 host;
    int                        enable_virtual_hosting;
    int                         port;
    
    std::string                 root;
    std::vector<std::string>    method_arr;
    long                        max_body_size;
    std::vector<t_error_page>   error_pages;
    std::vector<t_location>     locations;
}	UnitConf_t;



class Config
{
private:
    int n;
    std::string fileContent;
    std::vector<std::string> serverBlocks;
    std::map<int, std::vector<std::string> > serverBlockIdToLocationBlocks;

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
    std::string extractMainConfig(std::string serverBlock);
    void parseFileContent();
    void parseServerBlock(std::string serverBlock, int j);
    void parseLocationBlocks(int i);
    void checkPorts();
};

#endif
