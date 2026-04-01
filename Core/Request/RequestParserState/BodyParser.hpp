#ifndef BODYPARSER_HPP
#define BODYPARSER_HPP

#include "./ARequestParserState.hpp"
#include <string>
#include <cstdlib>
#include <climits>
#include <cctype>
#include <cerrno>


class BodyParser: public ARequestParserState {
	private:
        bool    _checked_headers;

        void    checkHeader();
		long parseContentLength(const std::string& value);
	public:
		BodyParser(Request *target, UnitConf_t endpoint);
		~BodyParser();

		void	execute();
};

#endif
