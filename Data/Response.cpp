#include "Response.hpp"

Response::Response():_status_code(0){};

Response::Response(const Response& other){
	*this = other;
};

Response& Response::operator=(const Response& other){
	if (this == &other)
		return *this;
	return *this;
};

Response::~Response(){};

void Response::setStatusCode(int code)
{
	_status_code = code;
}

void Response::setStatusName(std::string &name)
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
