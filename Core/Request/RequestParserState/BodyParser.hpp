#ifndef BODYPARSER_HPP
#define BODYPARSER_HPP

#include "./ARequestParserState.hpp"
#include <string>
#include <cstdlib>
#include <climits>
#include <cctype>
#include <cerrno>

typedef enum s_body_parser
{
	CHECK_HEADERS = 20,
	READ_BODY,
	BODY_ENCODE,
	CRLF
}	t_body_parser;

class BodyParser: public ARequestParserState {
	private:
        bool    		_checked_headers;
		long			_tmp;
		t_body_parser	_state;

        void    checkHeader();
		long	parseContentLength(const std::string& value);
		void    readBodyThroughContentLength();	
	public:
		BodyParser(Request *target, UnitConf_t endpoint);
		~BodyParser();

		void	execute();
};

#endif
