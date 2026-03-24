#ifndef METHODPARSER_HPP
#define METHODPARSER_HPP

#include "./ARequestParserState.hpp"

class MethodParser: public ARequestParserState {
	private:
		size_t 		_i;
		bool		_finished;
	public:
		MethodParser();
		~MethodParser();

		bool	completed() const;
		void	execute() const;
};

#endif
