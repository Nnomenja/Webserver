#include "Response.hpp"

#include <iostream>
#include <sstream>

Response::Response():_status_code(0){
	std::cout << "Response called" << std::endl;
    _cgi_response.size = 0;
    _cgi_response.value = "";
};

Response::Response(const Response& other){
	*this = other;

};

Response& Response::operator=(const Response& other){
	if (this == &other)
		return *this;
	return *this;
};

Response::~Response(){};


Response::t_status Response::getResponseStatus(int code)
{
	Response::t_status status [] = {
        // 1xx: Informational
        {100, "Continue"},
        {101, "Switching Protocols"},
        {102, "Processing"},
        {103, "Early Hints"},

        // 2xx: Success
        {200, "OK"},
        {201, "Created"},
        {202, "Accepted"},
        {203, "Non-Authoritative Information"},
        {204, "No Content"},
        {205, "Reset Content"},
        {206, "Partial Content"},
        {207, "Multi-Status"},
        {208, "Already Reported"},
        {226, "IM Used"},

        // 3xx: Redirection
        {300, "Multiple Choices"},
        {301, "Moved Permanently"},
        {302, "Found"},
        {303, "See Other"},
        {304, "Not Modified"},
        {305, "Use Proxy"},
        {306, "Unused"},          // 306 was "Switch Proxy", now deprecated/unused
        {307, "Temporary Redirect"},
        {308, "Permanent Redirect"},

        // 4xx: Client Errors
        {400, "Bad Request"},
        {401, "Unauthorized"},
        {402, "Payment Required"},
        {403, "Forbidden"},
        {404, "Not Found"},
        {405, "Method Not Allowed"},
        {406, "Not Acceptable"},
        {407, "Proxy Authentication Required"},
        {408, "Request Timeout"},
        {409, "Conflict"},
        {410, "Gone"},
        {411, "Length Required"},
        {412, "Precondition Failed"},
        {413, "Payload Too Large"},
        {414, "URI Too Long"},
        {415, "Unsupported Media Type"},
        {416, "Range Not Satisfiable"},
        {417, "Expectation Failed"},
        {418, "I'm a teapot"},
        {421, "Misdirected Request"},
        {422, "Unprocessable Entity"},
        {423, "Locked"},
        {424, "Failed Dependency"},
        {425, "Too Early"},
        {426, "Upgrade Required"},
        {428, "Precondition Required"},
        {429, "Too Many Requests"},
        {431, "Request Header Fields Too Large"},
        {451, "Unavailable For Legal Reasons"},

        // 5xx: Server Errors
        {500, "Internal Server Error"},
        {501, "Not Implemented"},
        {502, "Bad Gateway"},
        {503, "Service Unavailable"},
        {504, "Gateway Timeout"},
        {505, "HTTP Version Not Supported"},
        {506, "Variant Also Negotiates"},
        {507, "Insufficient Storage"},
        {508, "Loop Detected"},
        {510, "Not Extended"},
        {511, "Network Authentication Required"}
    };

    const std::size_t size = sizeof(status) / sizeof(status[0]);

    for (std::size_t i = 0; i < size; ++i) {
        if (status[i].code == code) {
            return ((Response::t_status){code, std::string(status[i].message)});
        }
    }
	return ((Response::t_status){code, "Unknown Status Code"});
}
void Response::setStatus(int code)
{
	if (_status_code)
		return;
	t_status status = getResponseStatus(code);
	_status_code = status.code;
	_status_name = status.message;
}

void Response::setStatusCode(const int code)
{
	_status_code = code;
}

void Response::setStatusName(const std::string &name)
{
	_status_name = name;
}

int Response::getStatusCode() const
{
	return (this->_status_code);
}

std::string Response::getStatusMessage() const
{
	return (this->_status_name);
}

std::string Response::getBody() const
{
	return (_body);
}

std::string Response::getCgiResponse() const
{
    return (_cgi_response.value);
}

void Response::setCgiResponse(const std::string &response)
{
    _cgi_response.value = response;
    _cgi_response.size += response.size();
}

size_t Response::getCgiResponseSize() const
{
    return (_cgi_response.size);
}

const std::map<std::string, std::string> &Response::getHeaders() const
{
	return (_headers);
}

void Response::setBody(const std::string &body)
{
	_body = body;
    _contentType = _body.size();
}

void Response::addHeader(std::string key, std::string value)
{
	_headers[key] = value;
}
void Response::setContentType(std::string value)
{
	_contentType = value;
	addHeader("Content-Type", value);
}

void Response::setContentLength(long value)
{
	std::stringstream ss;
	ss << value;
	addHeader("Content-Length", ss.str());
}

void Response::addCgiResponse(const std::string &response, size_t size)
{
    _cgi_response.value += response;
    _cgi_response.size += size;
}
