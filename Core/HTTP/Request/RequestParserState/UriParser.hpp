#ifndef URIPARSER_HPP
#define URIPARSER_HPP

#include "./ARequestParserState.hpp"
#include "../../../../utils/Encoding.hpp"

class UriParser: public ARequestParserState {
	private:
		int			_tmp;
		bool		_hasQuery;
		bool		_encoded;
		bool		_finished;

		bool		checkPathEncode(char c) const;
		bool		checkQueryEncode(char c) const;
		bool		checkPathPourcentEncoded(char c) const;
		bool		checkQueryPourcentEncoded(char c) const;
	public:
		UriParser(Request *target, UnitConf_t endpoint);
		~UriParser();

		void	execute();
};

#endif
