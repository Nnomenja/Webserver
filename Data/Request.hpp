#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <vector>
#include <map>

#include "../Enum/RequestParserStateName.hpp"
#include "../Core/HTTP/Request/RequestParserState/ARequestParserState.hpp"

#define BODY_BUFFER_SIZE_MAX 8192

typedef enum    s_body_encode
{
    BODY_CONTENT_LENGTH = 300,
    BODY_CHUNKED 
}   t_body_encode;


typedef struct s_body
{
    std::string     _str_buffer;
    std::ofstream   _file_buffer;
    int             _content_length;
}   t_body;

class ARequestParserState;
class Request {
    private:
        size_t                              _index;
        ARequestParserState					*_parserState;
        std::string							_buffer;
        size_t                              _buffer_size;
        t_location                          _location;
        std::string                         _root;

        /*==== Request line ====*/
        HttpMethod                          _method;
        std::string							_pathname;
        std::string                         _query;

        /*==== Request header ====*/

        std::map<std::string, std::string>	_headers;
        t_body_encode                       _body_encode;
        /*==== Request Body ====*/
        t_body                              _body;

        std::string                         _fullpath;

        Request(const Request& other);
        Request& operator=(const Request& other);
    public:
        Request();
        ~Request();

        ARequestParserState*	getParserState() const;
		size_t					getParserIndex() const;
		std::string             getBuffer() const;
        HttpMethod              getMethod() const;
        std::string             getMethodString() const;
        size_t                  getBufferSize() const;
		const std::string       &getPathname() const;
		std::string             getQuery() const;
        std::map<std::string \
                , std::string>  getHeaders() const;
        const std::string \
                                &getHeaderBykey(std::string key);
        t_body_encode           getBodyEncode() const;
        long                    getContentLength() const;
        const t_body&           getBody() const;        
        size_t                  getBodySize() const;        
        t_location              getLocation() const;
        const std::string 
                                &getLocationDefaultIndex() const;
        std::string             getFullPath() const;
        std::string             getRootDir() const;
        std::string             getPathExtension() const;

        void                    setLocation(t_location &value);
        void                    setLocationType(LocationType value);
        void					setParseState(ARequestParserState *state);
		void					setBuffer(std::string &value);
        void                    setPathname(std::string value);
        void                    setMethod(HttpMethod value);
        void                    setFullPath(std::string value);
        void                    setRoot(std::string value);
        void                    addPathname(char c);
        void                    setBody(std::string value);
        void                    addQuery(char c);
		void					incrementParserIndex();
		void					resetParserIndex();
        void                    setHeader(std::string key, std::string value);
		bool                    hasHeader(std::string value) const;
        void                    setContenLength(long value);
        void                    setBodyEncode(t_body_encode value);
        void                    setRedirectionPath(std::string &value);
        void                    addBody(char c, bool inc);
        std::string             &toLowerCase(std::string &src) const;
};

#endif
