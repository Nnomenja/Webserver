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
    protected:
        const RequestParserStateName _name;
        Request                     *_target;
        UnitConf_t                  _endpoint;                   
    public:
        virtual ~ARequestParserState();
        ARequestParserState(RequestParserStateName value, Request *target, UnitConf_t endpoint);

        virtual bool    completed() const = 0;
        virtual void	execute() = 0;

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