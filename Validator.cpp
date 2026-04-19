#include "Validator.hpp"

Validator::Validator() {}

Validator::Validator(const Validator& other)
{
    (void)other;
}

Validator::~Validator() {}

Validator& Validator::operator=(const Validator& other)
{
    if (this != &other)
    {
        // copy members here
        (void)other;
    }
    return *this;
}

bool Validator::isPositiveInt(const std::string& s)
{
    size_t i = 0;

    if (s.empty())
        return (false);
    if (i == s.size())
        return (false);
    for (; i < s.size(); i++)
        if (!std::isdigit(static_cast<unsigned char>(s[i])))
            return (false);
    std::string numStr = s;
    if (numStr.length() > 10)
        return (false);
    if (numStr.length() == 10)
    {
        if (numStr > "2147483647")
            return (false);
    }
    return (true);
}

bool Validator::isIpV4(const std::string& s)
{
    int dots = 0;
    for (std::string::const_iterator it = s.begin(); it != s.end(); it++)
    {
        if (*it == '.')
            dots++;
    }
    int                c = 0;
    std::istringstream iss(s);
    std::string        word;

    while (std::getline(iss, word, '.'))
    {
        if (!isPositiveInt(word))
            return (false);
        c++;
    }
    if (c != 4 || dots != 3)
        return (false);
    return (true);
}

bool Validator::isFile(const std::string& path)
{
    if (path.empty())
    {
        return (false);
    }
    struct stat st;
    if (stat(path.c_str(), &st) != 0)
    {
        return (false);
    }
    if (!S_ISREG(st.st_mode))
    {
        return (false);
    }
    if (access(path.c_str(), R_OK) != 0)
    {
        return (false);
    }
    return (true);
}

bool Validator::isDirectory(const std::string& path)
{
    struct stat st;
    if (stat(path.c_str(), &st) == -1)
        return (false);
    return (S_ISDIR(st.st_mode));
}

bool Validator::isFile(const std::string& path, std::string& errorMsg)
{
    if (path.empty())
    {
        errorMsg = "File path cannot be empty";
        return (false);
    }
    struct stat st;
    if (stat(path.c_str(), &st) != 0)
    {
        errorMsg = "File does not exist: " + path;
        return (false);
    }
    if (!S_ISREG(st.st_mode))
    {
        errorMsg = "Path is not a regular file: " + path;
        return (false);
    }
    if (access(path.c_str(), R_OK) != 0)
    {
        errorMsg = "File is not readable: " + path;
        return (false);
    }
    return (true);
}

bool Validator::isDirectory(const std::string& path, std::string& errorMsg)
{
    if (path.empty())
    {
        errorMsg = "Path cannot be empty";
        return (false);
    }
    struct stat st;
    if (stat(path.c_str(), &st) != 0 || !S_ISDIR(st.st_mode))
    {
        errorMsg = "Path does not exist or is not a directory: " + path;
        return (false);
    }
    if (access(path.c_str(), W_OK) != 0)
    {
        errorMsg = "Directory is not writable: " + path;
        return (false);
    }
    return (true);
}

bool Validator::validateServerName(std::string host)
{
    if (host.empty() || host.size() > 255)
        return false;
    size_t labelLength = 0;
    for (size_t i = 0; i < host.size(); ++i)
    {
        char c = host[i];
        if (c == '.')
        {
            if (labelLength == 0) // empty label
                return false;
            labelLength = 0; // reset for next label
        }
        else
        {
            // check valid characters: a-z, A-Z, 0-9, -
            if (!std::isalnum(c) && c != '-')
                return false;
            if (labelLength == 0 && c == '-') // label cannot start with -
                return false;
            labelLength++;
            if (labelLength > 63) // label cannot exceed 63 chars
                return false;
        }
    }
    // last label cannot end with '-'
    if (host[host.size() - 1] == '-')
        return false;
    return true;
}

bool Validator::validateHost(const std::string host)
{
    if (!isIpV4(host))
    {
        if (!validateServerName(host))
            return false;
    }
    return true;
}

bool Validator::validatePort(std::string port)
{
    if (!isPositiveInt(port))
        return (0);
    double value = std::strtod(port.c_str(), NULL);
    if (value > 65535 && value < 1024)
        return (0);
    return (1);
}

bool Validator::validateMethod(std::string method)
{
    if (method != "GET" && method != "POST" && method != "DELETE")
        return (0);
    return (1);
}

bool Validator::validateBoolStr(std::string value)
{
    if ((value != "ON" && value != "OFF") &&
        (value != "on" && value != "off"))
        return (0);
    return (1);
}

bool Validator::validateRoot(std::string root)
{
    std::string path = std::string(SUPERROOT) + "/" + root;
    if (!isDirectory(path))
        return (0);
    return (1);
}

bool Validator::validateUploads(std::string root, std::string uploads)
{
    std::string path = std::string(SUPERROOT) + "/" + root + "/" + uploads;
    if (!isDirectory(path) || root == "-")
        return (0);
    return (1);
}

bool Validator::validateIndex(std::string index)
{
    std::vector<std::string> extensions;
    extensions.push_back(".html");
    extensions.push_back(".htm");
    extensions.push_back(".php");

    int len = index.length();
    if (len == 0)
        return false;
    for (size_t i = 0; i < extensions.size(); ++i)
    {
        const std::string& ext = extensions[i];
        int extLen = ext.length();
        if (extLen > len)
            continue;
        if (index.compare(len - extLen, extLen, ext) == 0)
            return true;
    }
    return false;
}

bool Validator::validateMaxBodySize(std::string maxBodySize)
{
    if (!isPositiveInt(maxBodySize))
        return (0);
    double value = std::strtod(maxBodySize.c_str(), NULL);
    if (value > 52428800 + 1)
        return (0);
    return (1);
}

bool Validator::validateErrorCode(std::string errorCode)
{
    if (!isPositiveInt(errorCode))
        return false;
    int value = std::atoi(errorCode.c_str());
    if (value < 100 || value > 599)
        return false;
    return true;
}

bool Validator::validateURI(const std::string &uri)
{
    if (uri.empty())
        return false;
    if (uri[0] != '/')
        return false;
    for (size_t i = 0; i < uri.length(); i++)
    {
        if (std::isspace(uri[i]))
            return false;
        if (!isValidChar(uri[i]))
            return false;
        if (i > 0 && uri[i] == '/' && uri[i - 1] == '/')
            return false;
    }
    return true;
}

// here

bool Validator::isInVect(int integer, std::vector<int> vect)
{
    for (std::vector<int>::iterator it = vect.begin(); it != vect.end(); it++)
    {
        if (integer == *it)
            return (1);
    }
    return (0);
}

bool Validator::isValidChar(char c)
{
    return (std::isalnum(c) ||
            c == '/' || c == '-' || c == '_' ||
            c == '.' || c == '~');
}