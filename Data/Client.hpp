#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../Core/Network/Socket.hpp"
#include "./Request.hpp"
#include "./Response.hpp"
#include "../Enum/LocationType.hpp"
#include "../Core/Settings/Config.hpp"

class Request;
class Response;
class IRequestParseState;
class Client {
    private:
        int					_fd;
        std::string			_buffer;
		size_t				 _bufferSize;
        uint64_t			_start_time_ms;
        UnitConf_t		    _endpoint;

        Request		        *_req;
		Response	        *_res;
        bool                _parsed;

        Client(const Client& other);
        Client& operator=(const Client& other);
    public:
        Client();
        ~Client();

        int			    	getFd() const;
        std::string	    	getResponseHttp() const;
        size_t		    	getResponseHttpSize() const;
        uint64_t	    	getStartTime() const;
        UnitConf_t    		getEndpoint() const;

        Request             *getRequest();
        Response            *getResponse();

        void            	setFd(int fd);
        void				setEndpoint(UnitConf_t  value);
		void				setLocationType(LocationType type);
		void				setBuffer(std::string &value);
		void				setBufferSize(size_t value);

        void        		refreshStartTime();
        void                parsed();
        bool                isParsed() const;
};

#endif
