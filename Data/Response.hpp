#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <map>



class Response {
    private:
        typedef struct s_cgi_response
        {
            size_t      size;
            std::string value;
        }   t_cgi_response;

        int                      _status_code;
        std::string              _status_name;
        std::string              _body;
        std::map<std::string, \
            std::string>	    _headers;
        std::string              _contentType;
        t_cgi_response           _cgi_response;
        typedef struct s_status
        {
            int code;
            std::string message;
        }   t_status;
        
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
        void            setStatus(int code);
        void            setStatusCode(const int code);
        void            setStatusName(const std::string &name);
        void            setBody(const std::string &body);
        void            setContentLength(long value);
        void            setContentType(std::string value);
        void            addHeader(std::string key, std::string value);
        void            setCgiResponse(const std::string &response);
        void            addCgiResponse(const std::string &response, size_t size);
        const std::string     &getCgiResponse(void) const;
        size_t          getCgiResponseSize(void) const;
        bool            hasHeader(std::string &key) const;
        static t_status    getResponseStatus(const int code);
        
};

#endif
