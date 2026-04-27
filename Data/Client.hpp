#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../Core/Network/Socket.hpp"
#include "./Request.hpp"
#include "./Response.hpp"
#include "./RequestLogger.hpp"
#include "../Enum/LocationType.hpp"
#include "../Core/Settings/Config.hpp"

// NEW ->
#include <sstream>
#include <arpa/inet.h>
// NEW <-

class Request;
class Response;
class IRequestParseState;
class Client {
    private:
        int					     _fd;
        std::string			     _buffer;
		size_t				      _bufferSize;
        uint64_t			     _start_time_ms;
        UnitConf_t		         _endpoint;

        Request		             *_req;
		Response	             *_res;
        bool                     _parsed;

        pid_t                   _cgi_pid;
        int                     _cgi_output;
        std::string				_cgi_bin;
        bool                    _cgi_processing;
        bool                    _cgi_processing_end;
        bool                    _cgi_output_readed;
        std::map<std::string\
            , std::string>      &_envs;

        // NEW ->
        std::string _ip;
        RequestLogger _logger;
        // <- NEW

        Client(const Client& other);
        Client& operator=(const Client& other);
    public:
        Client(std::map<std::string, std::string> &envs);
        ~Client();

        int			    	getFd() const;
        std::string	    	getResponseHttp() const;
        size_t		    	getResponseHttpSize() const;
        uint64_t	    	getStartTime() const;
        UnitConf_t    		getEndpoint() const;

        Request             *getRequest();
        Response            *getResponse();
        std::string			getDefaultErrorPagePath(int code) const;
        const std::map<std::string, std::string> &getEnv() const;
        
        // NEW ->
        std::string         getIp() const;
        void setIp(sockaddrIn addr);
        std::string my_inet_ntoa(sockaddrIn addr);
        RequestLogger &getLogger();
        // <- NEW

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
        int                 getCGIfd() const;
        std::string         getCGIOutput() const;
        pid_t               getCGIPid() const;
        bool                isProcessingCGI() const;
        void                endProcessingCGI();
        void                setProcessingCGI(bool value);
        void                setEnv(std::string key, std::string value);
        std::string         getCGIbinByExtension(std::string ext) const;
        void                setCGIbin(std::string &value);
        std::string         getCGIbin() const;
        void                setCGIOutputReaded();
        void                setCGICGIProcessEnd();
        bool                isCGIOutputReaded() const;
		bool				isCGIProcessEnd();
        // void                (*registerFd)(int fd);
};

#endif
