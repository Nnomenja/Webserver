#ifndef CLIENT_HPP
#define CLIENT_HPP

# include "../Core/Socket.hpp"
# include "./Request.hpp"
# include "./Response.hpp"

class Request;
class Response;
class Client {
    private:
        SocketInfo	_clientInfo;
        int			_fd;
        std::string	_buffer;
        uint64_t	_start_time_ms;

    public:
        Client();
        Client(const Client& other);
        Client& operator=(const Client& other);
        ~Client();

        Request		req;
		Response	res;

        SocketInfo	getCLientInfo() const;
        int			getFd() const;
        std::string	getBuffer() const;
        uint64_t	getStartTime() const;

        void        setClientInfo(SocketInfo value);

        void        refreshStartTime();
};

#endif
