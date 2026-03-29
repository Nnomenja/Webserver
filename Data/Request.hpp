#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <vector>
#include <map>

#include "../Enum/RequestParserStateName.hpp"
#include "../Core/Request/RequestParserState/ARequestParserState.hpp"

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
        // std::map<std::string, std::string>	_headers;

        size_t                              _index;
        ARequestParserState					*_parserState;
        std::string							_buffer;
        size_t                              _buffer_size;

        /*==== Request line ====*/
        HttpMethod                          _method;
        std::string							_pathname;
        std::string                         _query;

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
		std::string             getPathname() const;
		std::string             getQuery() const;

        void					setParseState(ARequestParserState *state);
		void					setBuffer(std::string &value);
    
        void                    setMethod(HttpMethod value);
        void                    addPathname(char c);
        void                    addQuery(char c);
		void					incrementParserIndex();
		void					resetParserIndex();
		
};

#endif
