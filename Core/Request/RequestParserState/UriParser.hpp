#ifndef URIPARSER_HPP
#define URIPARSER_HPP

#include "./ARequestParserState.hpp"

class UriParser: public ARequestParserState {
	private:
		bool		_finished;
	public:
		UriParser(Request *target);
		~UriParser();

		bool	completed() const;
		void	execute() const;
};

#endif
