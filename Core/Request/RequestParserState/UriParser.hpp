#ifndef URIPARSER_HPP
#define URIPARSER_HPP

#include "./ARequestParserState.hpp"

class UriParser: public ARequestParserState {
	private:
		int		_tmp;
		bool	_hasQuery;
		bool	_encoded;

		bool	checkPathEncode(char c) const;
		bool	checkQueryEncode(char c) const;
		bool	checkPathPourcentEncoded(char c) const;
		bool	checkQueryPourcentEncoded(char c) const;
		bool 	isHexa(char c) const;
		int 	HexaToInt(char c) const;
		char 	PourcentHexaToChar(char first, char second) const;
	public:
		UriParser(Request *target, UnitConf_t endpoint);
		~UriParser();

		void	execute();
};

#endif
