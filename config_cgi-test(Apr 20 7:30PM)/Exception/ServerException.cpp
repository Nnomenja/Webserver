#include "ServerException.hpp"

ServerException::ServerException(int code, std::string message):_code(code), _name(message){};
ServerException::~ServerException() throw(){};

const char *ServerException::what() const throw()
{
	return (_name.c_str());
}

int ServerException::getCode() const
{
	return (_code);
}

std::string ServerException::getName() const
{
	return (_name);
}