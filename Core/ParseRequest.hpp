#ifndef PARSEREQUEST_HPP
#define PARSEREQUEST_HPP


/**
 * @brief Incremental HTTP request parser with configurable limits.
 * @details
 */
class Request;
class ParseRequest {
    private:
        int     _header_timeout;
        int     _body_timeout;
        int     _header_field_limit;
        int     _uri_limit;
    public:
        ParseRequest();
        ParseRequest(int header_timeout,int body_timeout,int header_field_limit, int uri_limit);
        ParseRequest(const ParseRequest& other);
        ParseRequest& operator=(const ParseRequest& other);
        ~ParseRequest();

        void        parse(Request &req) const;
};

#endif
