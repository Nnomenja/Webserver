#ifndef METHODPARSER_HPP
#define METHODPARSER_HPP

#include "./ARequestParserState.hpp"

class MethodParser: public ARequestParserState {
	private:
		int							_i;
		HttpMethod					detectMethod(std::string &value) const;						
	public:
		MethodParser(Request *target, UnitConf_t endpoint);
		~MethodParser();

		bool	completed() const;
		void	execute();
};

#endif
