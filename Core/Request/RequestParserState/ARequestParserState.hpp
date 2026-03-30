#ifndef AREQUESTPARSERSTATE_HPP
#define AREQUESTPARSERSTATE_HPP

#include  "../../../Enum/RequestParserStateName.hpp"
#include "../../Settings/Config.hpp"
#include "../../../Data/Request.hpp"
#include <string>
#include <iostream>

class Request;
class ARequestParserState
{
	private:
		bool	_crlf;
    protected:
        const RequestParserStateName _name;
        Request                     *_target;
        UnitConf_t                  _endpoint;                   
    public:
        virtual ~ARequestParserState();
        ARequestParserState(RequestParserStateName value, Request *target, UnitConf_t endpoint);

        virtual void	execute() = 0;
        
        virtual void    skipSeparator();
        virtual void	skipCRLF();
        RequestParserStateName  getParserStateName() const;

        class EagainParser: public std::exception {
            public : 
                const char *what() const throw()
                {
                    return ("EagainParser called");
                };
        };
};

#endif