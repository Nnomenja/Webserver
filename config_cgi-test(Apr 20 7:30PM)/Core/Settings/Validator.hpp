#ifndef VALIDATOR_HPP
#define VALIDATOR_HPP

#include <cctype>
#include <string>
#include <cstdlib>

#include <dirent.h>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>

#include <vector>

#include <string>
#include <cctype>



#define SUPERROOT "www"

class Validator
{
  private:
    Validator();
    Validator(const Validator& other);
    Validator& operator=(const Validator& other);

  public:
    ~Validator();

    static bool isPositiveInt(const std::string& s);
    static bool isIpV4(const std::string& s);
    
    static bool isFile(const std::string& path);
    static bool isDirectory(const std::string& path);
    static bool isFile(const std::string& path, std::string& errorMsg);
    static bool isDirectory(const std::string& path, std::string& errorMsg);

    static bool validateHost(std::string host);
    static bool validateServerName(std::string host);
    static bool validatePort(std::string port);
    static bool validateMethod(std::string method);
    static bool validateBoolStr(std::string value);
    static bool validateRoot(std::string root);
    static bool validateLocationRoot(std::string root);
    static bool validateUploads(std::string root, std::string uploads);
    static bool validateIndex(std::string index);
    static bool validateMaxBodySize(std::string maxBodySize);
    static bool validateCode(const std::string& code);
    static bool isSuccessCode(const std::string& code);
    static bool isRedirectCode(const std::string& code);
    static bool isErrorCode(const std::string& code);
    static bool validateURI(const std::string &uri);
    static bool isValidPath(const std::string& path);
    static bool isValidRedirectTarget(const std::string& target);
    static bool isValidCgiExtension(const std::string& ext);
    static bool isExecutable(const std::string& path);
    // here

    static bool isInVect(int integer, std::vector<int> vect);
    static bool isValidChar(char c);
    
};

#endif
