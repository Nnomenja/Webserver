#ifndef REQUESTPARSERNAME_HPP
#define REQUESTPARSERNAME_HPP

enum RequestParserStateName {
    METHOD = 100,
    URI,
    VERSION,
    HEADER,
    BODY,
    COMPLETE
};

#endif