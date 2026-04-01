#ifndef HADERPARSER_HPP
#define HADERPARSER_HPP

#include "./ARequestParserState.hpp"

typedef enum    s_header_state
{
    KEY = 200,
    SEPARATOR,
    VALUE,
    DELIMITER
}   t_header_state;

class HeaderParser: public ARequestParserState {
	private:
        std::string     _plain_text;
        std::string     _key;
        std::string     _value;
        t_header_state  _state;

        void        resetStateData();
        void	    addHeaderAndReset();
        bool        endOfHeadersReached();
        void        receivingHeaders();
        void        parseHeaders();
	public:
		HeaderParser(Request *target, UnitConf_t endpoint);
		~HeaderParser();

        bool        checkHeaderEncode(char c) const;
        std::string &toLowerCase(std::string &src) const;

		bool	completed() const;
		void	execute();
};

#endif
