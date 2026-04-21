#ifndef SERVEREXCEPTION_HPP
#define SERVEREXCEPTION_HPP

#include <exception>
#include <string>

class ServerException: public std::exception{
	private:
        int         _code;
        std::string _name;

    public:
        explicit ServerException(int code, std::string message);
        ~ServerException() throw();
        const char *what() const throw();
        
        int         getCode() const;
        std::string getName() const;

};

#endif
