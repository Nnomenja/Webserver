#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

enum RequestParserState {
    METHOD = 100,
    URI,
    VERSION,
    BODY,
    COMPLETE
};

#endif