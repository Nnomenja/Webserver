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

typedef struct s_return
{
  int code;
  std::string target;
} t_return;

typedef struct s_cgi
{
    std::string     extension;
    std::string     bin;
}   t_cgi;

typedef struct s_location
{
    LocationType                type;
    std::string                 path;
    int                         methods;
    std::map<int, std::string>   error_pages;
    std::string                 root;
    bool                        auto_index;
    std::vector<std::string>                index;
    std::string uploads;
    t_return ret;
    std::map<std::string, std::string> CGI;
}   t_location;

typedef struct UnitConf
{
    std::string                 host;
    bool                        enable_virtual_hosting;
    std::string server_name;
    int                         port;
    std::string                 root;    
    // std::vector<std::string>    method_arr;
    int                        max_body_size;
    std::map<int, std::string>   error_pages;

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
    void getLocationBlocks(int i);
    void parseLocationBlock(std::string locationBlock, int i, int j);
    void checkLocationBlock(std::vector<t_location> &locations, int j);
    void checkPaths(const std::vector<t_location>& locations);
    void checkPorts();
    void checkServerNames();
    void checkVirtualHosting();

    std::vector<UnitConf_t> getConfigs() const;
    int                     getN() const;
};

#endif
