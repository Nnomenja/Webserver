#include "RequestLogger.hpp"
#include <iostream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <arpa/inet.h>

// ANSI colors
#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define RED     "\033[31m"

// Default constructor
RequestLogger::RequestLogger()
    : _method(""), _path(""), _status(0), _clientIp(""), _responseTime(0.0),
      _hasMethod(false), _hasPath(false), _hasStatus(false),
      _hasIp(false), _hasResponseTime(false) {}

RequestLogger::RequestLogger(const std::string& method,
                             const std::string& path,
                             int status,
                             const std::string& clientIp,
                             uint64_t responseTime)
    : _method(method),
      _path(path),
      _status(status),
      _clientIp(clientIp),
      _responseTime(responseTime),
      _hasMethod(true),
      _hasPath(true),
      _hasStatus(true),
      _hasIp(true),
      _hasResponseTime(true) {}

// Copy constructor
RequestLogger::RequestLogger(const RequestLogger& other) {
    *this = other;
}

// Assignment
RequestLogger& RequestLogger::operator=(const RequestLogger& other) {
    if (this != &other) {
        _method = other._method;
        _path = other._path;
        _status = other._status;
        _clientIp = other._clientIp;
        _responseTime = other._responseTime;

        _hasMethod = other._hasMethod;
        _hasPath = other._hasPath;
        _hasStatus = other._hasStatus;
        _hasIp = other._hasIp;
        _hasResponseTime = other._hasResponseTime;
    }
    return *this;
}

// Destructor
RequestLogger::~RequestLogger() {}


// ---------------- SETTERS ----------------

void RequestLogger::setMethod(const std::string& method) {
    _method = method;
    _hasMethod = true;
}

void RequestLogger::setPath(const std::string& path) {
    _path = path;
    _hasPath = true;
}

void RequestLogger::setStatus(int status) {
    _status = status;
    _hasStatus = true;
}

void RequestLogger::setIp(std::string ip) {
    _clientIp = ip;
    _hasIp = true;
}

void RequestLogger::setResponseTime(uint64_t time) {
    _responseTime = time;
    _hasResponseTime = true;
}


// ---------------- UTILS ----------------

bool RequestLogger::isComplete() const {
    return _hasMethod && _hasPath && _hasStatus && _hasIp && _hasResponseTime;
}

std::string RequestLogger::getTimestamp() const {
    std::time_t now = std::time(NULL);
    std::tm* t = std::localtime(&now);

    std::ostringstream oss;
    oss << std::setfill('0')
        << (t->tm_year + 1900) << "-"
        << std::setw(2) << (t->tm_mon + 1) << "-"
        << std::setw(2) << t->tm_mday << " "
        << std::setw(2) << t->tm_hour << ":"
        << std::setw(2) << t->tm_min << ":"
        << std::setw(2) << t->tm_sec;

    return oss.str();
}


// ---------------- LOGGING ----------------

static const char* getColor(int status) {
    if (status >= 200 && status < 300)
        return GREEN;
    else if (status >= 300 && status < 500)
        return YELLOW;
    else
        return RED;
}

void RequestLogger::log() const {
    if (!isComplete()) {
        std::cerr << RED << "[LOGGER ERROR] Incomplete log data" << RESET << std::endl;
        return;
    }

    const char* color = getColor(_status);

    std::cout << "["
              << getTimestamp()
              << "] "
              << _clientIp << " \""
              << _method << " " << _path
              << "\" "
              << color << _status << RESET << " "
              << _responseTime << "ms"
              << std::endl;
}