#include "./BodyParser.hpp"
#include "../../../Exception/BadRequestException.hpp"
#define CONTENT_LENGTH_MAX 10485760
BodyParser::BodyParser(Request *target, UnitConf_t endpoint):ARequestParserState(BODY, target, endpoint), _checked_headers(false){
};
BodyParser::~BodyParser(){
};


void BodyParser::checkHeader() 
{
    bool    hasContentLength = false;

    if (!_target->hasHeader("host"))
        throw BadRequestException();
    if (_target->hasHeader("content-length"))
    {
        _target->setContenLength(parseContentLength(_target->getHeaderBykey("content-length")));
        _target->setBodyEncode(BODY_CONTENT_LENGTH);
        hasContentLength = true;
    }
    if (_target->hasHeader("Transfer-Encoding"))
    {
        if(_target->getHeaderBykey("Transfer-Encoding") == "chuncked" && hasContentLength)
            throw BadRequestException();
        _target->setBodyEncode(BODY_CHUNKED);
    }
}

long BodyParser::parseContentLength(const std::string& value)
{
    // 1. Trim whitespace
    size_t start = 0;
    while (start < value.length() && std::isspace(static_cast<unsigned char>(value[start]))) {
        ++start;
    }
    size_t end = value.length();
    while (end > start && std::isspace(static_cast<unsigned char>(value[end - 1]))) {
        --end;
    }

    // 2. check if empty
    if (start == end) {
        throw BadRequestException();
    }
    
    // 3 Check if all characters are digits
    for (size_t i = start; i < end; ++i) {
        if (!std::isdigit(static_cast<unsigned char>(value[i]))) {
            throw BadRequestException();
        }
    }
    
    // 4. Convert to long
    char* endptr;
    errno = 0;
    long length = std::strtol(value.c_str() + start, &endptr, 10);
    
    // 5. Check for conversion errors
    if (errno == ERANGE || length < 0 || length > CONTENT_LENGTH_MAX) {
        throw BadRequestException();
    }

    // 6. Check if the entire string was consumed
    if (static_cast<size_t>(endptr - (value.c_str() + start)) != end - start) {
        throw BadRequestException();
    }

    return (length);
}

void BodyParser::execute()
{
	std::cout << "BodyParser executing..." << std::endl;
    if (!_checked_headers)
        checkHeader();
    if (_target->getMethod() == GET)
		return;
    // else
    // {
    //     char c;
    
    //     if (_target->getBodyEncode() == BODY_CONTENT_LENGTH)
    //     {
    //         for (size_t i = _target->getParserIndex(); i < _target->getBufferSize(); i++)
    //         {
    //             c = _target->getBuffer()[i];
    //             _target->addBody(c);
    //             _target->incrementParserIndex();
    //         }
    //     }
    // }
    _target->resetParserIndex();
	throw EagainParser();
}
