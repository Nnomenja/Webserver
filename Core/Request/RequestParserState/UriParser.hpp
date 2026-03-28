#ifndef URIPARSER_HPP
#define URIPARSER_HPP

#include "./ARequestParserState.hpp"

class UriParser: public ARequestParserState {
	private:

	public:
		UriParser(Request *target, UnitConf_t endpoint);
		~UriParser();

		void	execute();
};

#endif
