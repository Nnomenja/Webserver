#include "MetadataParser.hpp"
#include "../../../../Exception/BadRequestException.hpp"
#include "../../../../Exception/NotFound.hpp"
#include "../../../../Exception/PayloadTooLarge.hpp"
#include "../../../../Exception/Forbiden.hpp"

MetadataParser::MetadataParser(Request *target, UnitConf_t endpoint):ARequestParserState(METADATA, target, endpoint), _longest_matching(0){

};
MetadataParser::~MetadataParser(){
};

size_t    computeMatchingPrefix(const std::string &src, const std::string &value)
{
    size_t  count = 1;
    size_t i = 0;

    if (src == "/")
        return (1);
    if (src.size() > value.size())
        return (0);

    while (src[i])
    {
        if (src[i] != value[i])
            return (0);
        i++;
        count++;
    }
    if (value[i] && value[i] != '/')
        return (0);
    return (count);
}

void    trimWhiteSpace(std::string  &s)
{
    size_t start = -1;
    size_t  size = 0;

    while (s[++start] == ' ')
        ;    
    s.erase(0, start);
    
    start = s.length();
    while (s[--start] == ' ')
        size++;
    s.erase(s.length() -  size, size);
}

long MetadataParser::parseContentLength(const std::string& value)
{
	std::string tmp;
    long        res;
    char*		endptr;

    tmp = value;
    endptr = NULL;
    trimWhiteSpace(tmp);
    if (!tmp.size())
        throw BadRequestException();

    for (size_t i = 0; i < tmp.size(); ++i)
    {
        if (!std::isdigit(static_cast<unsigned char>(tmp[i])))
            throw BadRequestException();
    }
    
    res = std::strtol(tmp.c_str(), &endptr, 10);
    
    if ( res > _endpoint.max_body_size)
        throw PayloadTooLarge();
    return (res);
}

void MetadataParser::matchConfiguredRoute()
{
    size_t      tmp;
    t_location  location;
    std::string host;

    // 1. find the longest matching prefix location.
    for (size_t i = 0; i < _endpoint.locations.size(); i++)
    {
        tmp = computeMatchingPrefix(_endpoint.locations[i].path, _target->getPathname());
        if (tmp && tmp > _longest_matching)
        {
            location = _endpoint.locations[i];
            _longest_matching = tmp;
        }
    }
    _target->setLocation(location);
    if (!_longest_matching)
        throw (NotFound());
    
    // 2. Hostname is required
    if (!_target->hasHeader("host"))
        throw BadRequestException();

    // 3. If virtual hosting is enable. So we must verify th hostname 
    if (_endpoint.enable_virtual_hosting)
    {
        host = _target->getHeaderBykey("host");
        trimWhiteSpace(host);
        if (host != _endpoint.host)
            throw  NotFound();
    }
}

void MetadataParser::parseBodyMetadata()
{
    const bool hasContentLength = _target->hasHeader("content-length");
    const bool hasTransferEncoding = _target->hasHeader("transfer-encoding");

    if (hasContentLength && hasTransferEncoding)
        throw BadRequestException();

    if (hasTransferEncoding)
    {
        std::string transferEncoding = _target->getHeaderBykey("transfer-encoding");
        trimWhiteSpace(transferEncoding);
        if (transferEncoding != "chunked")
            throw BadRequestException();

        _target->setBodyEncode(BODY_CHUNKED);
        return;
    }
    else if(hasContentLength)
    {
        _target->setContenLength(parseContentLength(_target->getHeaderBykey("content-length")));
        _target->setBodyEncode(BODY_CONTENT_LENGTH);
    }
}

void MetadataParser::execute()
{
    matchConfiguredRoute();
    parseBodyMetadata();
	std::cout << "...................." << std::endl;
}
