#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "File.hpp"
#include "helpers.hpp"
#include <stdexcept>
#include <vector>
#include <sstream>
#include <map>

class Config
{
private:
    int n;
    std::string fileContent;
    std::vector<std::string> serverBlocks;
    std::map<int, std::vector<std::string> > serverBlockIdToLocationBlocks;
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
    void parseLocationBlocks(int i);
};

#endif
