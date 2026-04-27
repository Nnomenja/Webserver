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
    double      _responseTime;

    bool _hasMethod;
    bool _hasPath;
    bool _hasStatus;
    bool _hasIp;
    bool _hasResponseTime;

public:
    // Canonical form
    RequestLogger();
    RequestLogger(const std::string& method,
              const std::string& path,
              int status,
              const std::string& clientIp,
              double responseTime);
    RequestLogger(const RequestLogger& other);
    RequestLogger& operator=(const RequestLogger& other);
    ~RequestLogger();

    // Setters
    void setMethod(const std::string& method);
    void setPath(const std::string& path);
    void setStatus(int status);
    void setIp(struct in_addr addr);
    void setResponseTime(double time);

    // Utils
    bool        isComplete() const;
    std::string getTimestamp() const;

    // Logging
    void log() const;
};

#endif