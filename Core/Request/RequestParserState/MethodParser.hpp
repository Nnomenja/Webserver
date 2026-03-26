#ifndef METHODPARSER_HPP
#define METHODPARSER_HPP

#include "./ARequestParserState.hpp"

class MethodParser: public ARequestParserState {
	private:
		bool		_finished;
	public:
		MethodParser(Request *target);
		~MethodParser();

		bool	completed() const;
		void	execute() const;
};

#endif
