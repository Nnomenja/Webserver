#ifndef ENDPARSER_HPP
#define ENDPARSER_HPP

#include "./ARequestParserState.hpp"

class EndParser: public ARequestParserState {
	public:
		EndParser(Request *target, UnitConf_t endpoint);
		~EndParser();

		void	execute();
};

#endif
