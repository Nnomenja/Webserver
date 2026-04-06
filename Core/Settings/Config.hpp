#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "../../utils/utils.hpp"
#include "Validator.hpp"
#include "../../Enum/LocationType.hpp"
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


typedef struct s_cgi
{
    std::string     extension;
    std::string     bin;
}   t_cgi;

typedef struct s_error_page
{
    int         code;
    std::string path;
}   t_error_page;


typedef struct s_location
{
    LocationType                type;
    std::string                 path;
    // std::vector<t_error_page>   error_pages;
    std::string                 root;
    // std::vector<std::string>    methods;
    // std::string                 index;
    bool                        auto_index;
    std::string                 return_path;
    // bool                        upload_enable;
    // std::string                 upload_store;
    // std::vector<t_cgi>          cgi;
    std::string                 index;
}   t_location;

typedef struct UnitConf
{
    std::string                 host;
    bool                        enable_virtual_hosting;
    int                         port;
    int                         methods;
    // LocationType                type; // temp
    std::vector<std::string>    method_arr;
    long                        max_body_size;
    // std::vector<t_error_page>   error_pages;
    std::vector<t_location>     locations;
}	UnitConf_t;

class Config
{
private:
    std::string fileContent;
    int n;
    std::vector<std::string>    serverBlocks;
    std::vector<UnitConf_t>     configs;
    std::map<int, UnitConf_t>   _endpoints;
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
    int                     getN() const;
    UnitConf_t              findEndpointByFd(int fd);
    
    void                    setEndpointByFd(UnitConf_t &value, int fd);
};

#endif
