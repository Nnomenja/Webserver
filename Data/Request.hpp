#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <vector>
#include <map>

#include "../Enum/RequestParserStateName.hpp"
#include "../Core/Request/RequestParserState/ARequestParserState.hpp"

typedef enum    s_body_encode
{
    BODY_CONTENT_LENGTH = 300,
    BODY_CHUNKED 
}   t_body_encode;

class ARequestParserState;
class Request {
    private:
        // std::string							_method;
        // std::string							_pathname;
        // std::string							_file;
        // std::string							_tmp;
        // std::string							_httpVersion;
        // bool								_hasPourcentEncode;
        // bool								_hasQuery;
        // bool								_hasContentLength;
        // bool								_hasContentType;
        // size_t								_contentLength;
        // int									_port;
        // std::string							_body;
        // bool								_hasBody;
        // std::vector<std::string>			_contentType;

        size_t                              _index;
        ARequestParserState					*_parserState;
        std::string							_buffer;
        size_t                              _buffer_size;
        t_location                          _location;

        /*==== Request line ====*/
        HttpMethod                          _method;
        std::string							_pathname;
        std::string                         _query;

        /*==== Request header ====*/

        std::map<std::string, std::string>	_headers;
        long                                _contentLength;
        t_body_encode                       _body_encode;
        /*==== Request Body ====*/
        std::string                         _body;

        Request(const Request& other);
        Request& operator=(const Request& other);
    public:
        Request();
        ~Request();

        ARequestParserState*	getParserState() const;
		size_t					getParserIndex() const;
		std::string             getBuffer() const;
        HttpMethod              getMethod() const;
        size_t                  getBufferSize() const;
		const std::string       &getPathname() const;
		std::string             getQuery() const;
        std::map<std::string \
                , std::string>  getHeaders() const;
        const std::string \
                                &getHeaderBykey(std::string key);
        t_body_encode           getBodyEncode() const;
        long                    getContentLength() const;
        const std::string       &getBody() const;        
        t_location              getLocation() const;

        void                    setLocation(t_location &value);
        void                    setLocationType(EndpointType &value);
        void					setParseState(ARequestParserState *state);
		void					setBuffer(std::string &value);
        void                    setPathname(std::string value);
        void                    setMethod(HttpMethod value);
        void                    addPathname(char c);
        void                    addQuery(char c);
		void					incrementParserIndex();
		void					resetParserIndex();
        void                    setHeader(std::string key, std::string value);
		bool                    hasHeader(std::string value) const;
        void                    setContenLength(long value);
        void                    setBodyEncode(t_body_encode value);
        void                    addBody(char c);
        std::string             &toLowerCase(std::string &src) const;
};

#endif
