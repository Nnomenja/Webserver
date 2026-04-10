#ifndef METADATAPARSER_HPP
#define METADATAPARSER_HPP

#include "./ARequestParserState.hpp"


class MetadataParser: public ARequestParserState 
{
    private:
        size_t  _longest_matching;

        void    matchConfiguredRoute();
        void    resolveFilesystemPath();
        long    parseContentLength(const std::string& value);
        void    parseBodyMetadata();
		void   parseRefererPath(std::string &referer);
    public:
        MetadataParser(Request *target, UnitConf_t endpoint);
        ~MetadataParser();

		void	execute();
};

#endif
