#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <map>

class Response {
    private:
        int                      _status_code;
        std::string              _status_name;
        std::string              _body;
        std::map<std::string, \
            std::string>	    _headers;
        std::string              _contentType;

    public:
        Response();
        Response(const Response& other);
        Response& operator=(const Response& other);
        ~Response();

        int            getStatusCode(void) const;
        std::string    getStatusMessage(void) const;
        std::string    getBody(void) const;
        const std::map<std::string, \
                std::string>	    &getHeaders() const;
        void            setStatusCode(const int code);
        void            setStatusName(const std::string &name);
        void            setBody(const std::string &body);
        void            setContentLength(long value);
        void            setContentType(std::string value);
        void            addHeader(std::string key, std::string value);
};

#endif
