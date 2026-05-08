#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "File.hpp"
#include "helpers.hpp"

#include "../../Enum/LocationType.hpp"

#include "Validator.hpp"

#include <stdexcept>
#include <vector>
#include <sstream>
#include <map>

enum HttpMethod
{
    GET    = 1,
    POST   = 2,
    DELETE = 4 
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
    std::vector<t_error_page>   error_pages;
    std::map<int, std::string>   error_pages_map;
    std::string                 root;
    bool                        auto_index;
    bool auto_index_set;
    std::vector<std::string>                index_vect;
    std::string index;
    std::string upload_store;
    t_return ret;

    
    std::string return_path;

    std::map<std::string, std::string> CGI;
}   t_location;

typedef struct UnitConf
{
    std::string                 host;
    bool                        enable_virtual_hosting;
    std::string server_name;
    int                         port;
    std::string                 root;    
    
    std::vector<std::string>    method_arr;
    int methods;

    unsigned int                       max_body_size;
    std::vector<t_error_page>   error_pages;
    std::map<int, std::string>   error_pages_map;

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
    std::map<int, UnitConf_t>   _endpoints;
    int serverLine;
    int locationLine;

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
        int serverLine;
        int locationLine;
        std::string info;

      public:
        ConfigException(const std::string& msg, int serverLine, int locationLine, std::string info);
        virtual const char* what() const throw();
        virtual int getserverLine() const throw();
        virtual int getlocationLine() const throw();
        virtual std::string getInfo() const throw();

        virtual ~ConfigException() throw();
    };
    std::string extractMainConfig(std::string serverBlock);
    void parseFileContent();
    void parseServerBlock(std::string serverBlock, int j);
    void getLocationBlocks(int i);
    void parseLocationBlock(std::string locationBlock, int i, int j);
    void checkLocationBlock(std::vector<t_location> &locations, int i, int j);
    void checkPaths(const std::vector<t_location>& locations);
    void checkPorts();
    void checkServerNames();
    void checkVirtualHosting();

    std::vector<UnitConf_t> getConfigs() const;
    int                     getN() const;

    UnitConf_t              findEndpointByFd(int fd);
    void                    setEndpointByFd(UnitConf_t &value, int fd);
};

#endif
