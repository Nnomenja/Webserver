#include "Response.hpp"

#include <iostream>

Response::Response():_status_code(0){
	std::cout << "Response called" << std::endl;
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

const std::map<std::string, std::string> &Response::getHeaders() const
{
	return (_headers);
}

void Response::setBody(const std::string &body)
{
	_body = body;
}

void Response::addHeader(std::string key, std::string value)
{
	_headers[key] = value;
}