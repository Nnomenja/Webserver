#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>

class Response {
    private:
        int             _status_code;
        std::string     _status_name;
    public:
        Response();
        Response(const Response& other);
        Response& operator=(const Response& other);
        ~Response();

        int            getStatusCode(void) const;
        std::string    getStatusName(void) const;

        void    setStatusCode(int code);
        void    setStatusName(std::string &name);
};

#endif
