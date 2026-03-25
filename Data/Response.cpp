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

std::string Response::getStatusName() const
{
	return (this->_status_name);
}
