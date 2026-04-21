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
        pid_t               _cgi_pid;
        int                 _cgi_output;
        bool                _processing_cgi;
        char                **_environ;

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
        std::string			getDefaultErrorPagePath(int code) const;
        char                **getEnviron() const;
        
        void            	setFd(int fd);
        void				setEndpoint(UnitConf_t  value);
		void				setLocationType(LocationType type);
		void				setBuffer(std::string value);
		void				setBufferSize(size_t value);

        void        		refreshStartTime();
        void                parsed();
        bool                isParsed() const;
        void                generateResponse();
        void                setCGIInfo(pid_t pid, int output_fd);
        bool                isCGI() const;
        int                 getCGIOutput() const;
        pid_t               getCGIPid() const;
        bool                isProcessingCGI() const;
        void                endProcessingCGI();
        void                setProcessingCGI(bool value);
        void                setEnviron(char **environ);
        // void                (*registerFd)(int fd);
};

#endif
