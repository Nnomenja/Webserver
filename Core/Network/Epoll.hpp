/*
   File: Epoll.hpp
   By: Azaria
   Created: 2026/02/20 10:29:23
*/

#ifndef EPOLL_HPP
#define EPOLL_HPP

# include <vector>
# include <string>
# include <sys/epoll.h>
# define MAX_EVENTS 10

class Epoll
{
	public :
		Epoll();
		~Epoll();

		int 								getEpollFd( void ) const;
		std::vector<struct epoll_event>& 	getEvents( void );
		void								setMaxEvent(int maxEvent);

		bool								epollCreate( void );
        bool								registerFd(int fd, unsigned int event);
		int									wait(int timeout);
		bool 								modify(int fd, unsigned int event);
		bool 								unregister(int fd);
		void								close( void );

		std::string							read(const int fd, bool *end);
		void								send(const int fd, const std::string &buff, const size_t lenght);

	private :
		Epoll(const Epoll& epollType);
		Epoll& operator=(const Epoll& epollType);

		int		_epollFd;
		int		_maxEvents;
		std::vector<struct epoll_event> _events;
};

#endif /* EPOLL_HPP */