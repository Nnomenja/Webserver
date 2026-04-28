#ifndef METADATAPARSER_HPP
#define METADATAPARSER_HPP

#include "./ARequestParserState.hpp"
#include "../../../../utils/Encoding.hpp"


class MetadataParser: public ARequestParserState 
{
    private:
        typedef enum COOKIE_PARSING_STATE
        {
            KEY,
            VALUE,
            SEPARATOR
        }   COOKIE_PARSING_STATE;

        size_t  _longest_matching;

        void    matchConfiguredRoute();
        long    parseContentLength(const std::string& value);
        void    parseBodyMetadata();
        void    parseHostHeader();
        void    parseCookies();

    public:
        MetadataParser(Request *target, UnitConf_t endpoint);
        ~MetadataParser();

		void	execute();
};

#endif
