#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "File.hpp"
#include <stdexcept>
#include <vector>

class Config
{
private:
    std::string fileContent;
    std::vector<std::string> serverBlocks;
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
};

#endif
