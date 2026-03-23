#ifndef CLIENT_HPP
#define CLIENT_HPP

# include "../Core/Network/Socket.hpp"
# include "./Request.hpp"
# include "./Response.hpp"
# include "../Enum/EndpointType.hpp"
# include "../Core/Settings/Config.hpp"

class Request;
class Response;
class Client {
    private:
        int			    _fd;
        std::string	    _buffer;
		size_t		     _bufferSize;
        uint64_t	    _start_time_ms;
        UnitConf_t		_endpoint;

    public:
        Client();
        Client(const Client& other);
        Client& operator=(const Client& other);
        ~Client();

        Request		req;
		Response	res;

        int			    getFd() const;
        std::string	    getBuffer() const;
        size_t		    getBufferSize() const;
        uint64_t	    getStartTime() const;
        UnitConf_t    	getEndpoint() const;

        void            setFd(int fd);
        void			setEndpoint(UnitConf_t  value);
		void			setEndpointType(EndpointType type);
		void			setBuffer(std::string &value);
		void			setBufferSize(size_t value);

        void        refreshStartTime();
};

#endif
