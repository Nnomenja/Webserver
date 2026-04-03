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
		long			_tmp;
		bool			_end;

		void    readBodyThroughContentLength();	
		void	readBodyAsChuncked();
	public:
		BodyParser(Request *target, UnitConf_t endpoint);
		~BodyParser();

		void	execute();
};

#endif
