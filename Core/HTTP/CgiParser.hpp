#ifndef CGIPARSER_HPP
#define CGIPARSER_HPP

#include <string>
#include <exception>
#include "../../Data/Response.hpp"

class CgiParser {
    private:
        typedef enum s_state
        {
            HEADER = 40,
            BODY
        }   t_state;

        const std::string   &_s;
        t_state              _state;
        int                 _i;
        Response            *_res;
        bool                _has_status_code;

        CgiParser();
        void    parseHeader();
        void    parseBody();
        void    addHeader(std::string &key, std::string &value);
        void    parseStatusCode(std::string &s);
    public:
        void    parse();
        CgiParser(const std::string &s, Response *res);
        ~CgiParser();        
};

#endif
