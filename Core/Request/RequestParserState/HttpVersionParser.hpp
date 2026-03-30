#ifndef HTTPVERSIONPARSER_HPP
#define HTTPVERSIONPARSER_HPP

#include "./ARequestParserState.hpp"


class HttpVersionParser: public ARequestParserState 
{
    private:
        std::string                 _protocol_name;
        bool                        protocol_readed;
        std::vector<std::string>	_versions;
        size_t                      _j;
    public:
        HttpVersionParser(Request *target, UnitConf_t endpoint);
        ~HttpVersionParser();

		void	execute();
};

#endif
