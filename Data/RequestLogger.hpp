#ifndef REQUESTLOGGER_HPP
#define REQUESTLOGGER_HPP

#include <string>
#include <netinet/in.h>

class RequestLogger {
private:
    std::string _method;
    std::string _path;
    int         _status;
    std::string _clientIp;
    uint64_t      _responseTime;

    bool _hasMethod;
    bool _hasPath;
    bool _hasStatus;
    bool _hasIp;
    bool _hasResponseTime;

public:
    RequestLogger();
    RequestLogger(const std::string& method,
              const std::string& path,
              int status,
              const std::string& clientIp,
              uint64_t responseTime);
    RequestLogger(const RequestLogger& other);
    RequestLogger& operator=(const RequestLogger& other);
    ~RequestLogger();

    void setMethod(const std::string& method);
    void setPath(const std::string& path);
    void setStatus(int status);
    void setIp(std::string ip);
    void setResponseTime(uint64_t time);

    bool        isComplete() const;
    std::string getTimestamp() const;

    void log() const;
};

#endif