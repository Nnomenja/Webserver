#ifndef AREQUESTPARSERSTATE_HPP
#define AREQUESTPARSERSTATE_HPP

#include  "../../../Enum/RequestParserStateName.hpp"
#include <string>

class ARequestParserState
{
    private:
        const RequestParserStateName _name;
    public:
        virtual ~ARequestParserState();
        ARequestParserState(RequestParserStateName value);

        virtual bool    completed() const = 0;
        virtual void	execute() const = 0;

        RequestParserStateName  getParserStateName() const;
};

#endif