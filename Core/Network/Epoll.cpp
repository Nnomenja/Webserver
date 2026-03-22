/*
   File: Epoll.cpp
   By: Azaria
   Created: 2026/02/20 10:29:01
*/

# include "Epoll.hpp"
# include <unistd.h>
# include <iostream>

/* ************************************************************************** */
/*                            Canonical Form                                  */
/* ************************************************************************** */

Epoll::Epoll( )
	: _epollFd(-1), _maxEvents(MAX_EVENTS)
{ }

Epoll::Epoll(const Epoll& epollType)
	: _maxEvents(epollType._maxEvents)
{
	*this = epollType;
}

Epoll& Epoll::operator=(const Epoll& epollType)
{
	if (this != &epollType)
	{
		_epollFd = epollType._epollFd;
	}
	return (*this);
}

Epoll::~Epoll()
{
	this->close();
}

/* ************************************************************************** */
/*                             Getters / seters                               */
/* ************************************************************************** */

int Epoll::getEpollFd( void ) const
{
	return (_epollFd);
}

void Epoll::setMaxEvent(int maxEvent)
{
	_maxEvents = maxEvent;
}

std::vector<struct epoll_event>& Epoll::getEvents( void )
{
	return (_events);
}

/* ************************************************************************** */
/*                             Other Methods                                  */
/* ************************************************************************** */

bool Epoll::epollCreate( void )
{
	_epollFd = epoll_create(1);

	if (_epollFd < 0)
	{
		std::cerr << "Error: Epoll: Cannot create Epoll" << std::endl;
	}
	return (true);
}

bool Epoll::registerFd(int fd, unsigned int event)
{
	struct epoll_event ev;

	ev.events = event;
	ev.data.fd = fd;

	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, fd, &ev) < 0)
	{
        std::cerr << "Error: Epoll: registerFd failed" << std::endl;
		return (false);
	}
	return (true);
}

int Epoll::wait(int timeout)
{
	struct epoll_event ev;
	ev.events = 0;
	ev.data.fd = 0;
	
	_events.assign(_maxEvents, ev);
	return (::epoll_wait(_epollFd, _events.data(), _maxEvents, timeout));
}

bool Epoll::modify(int fd, unsigned int event)
{
	struct epoll_event ev;

	ev.data.fd = fd;
	ev.events = event;

	if (::epoll_ctl(_epollFd, EPOLL_CTL_MOD, fd, &ev) < 0)
		return (false);
	return (true);
}

bool Epoll::unregister(int fd)
{
	if (::epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, 0) < 0)
		return (false);
	return (true);
}

void Epoll::close( void )
{
	if (_epollFd >= 0)
		::close(_epollFd);
}
