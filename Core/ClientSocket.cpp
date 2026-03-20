/*
   File: ClientSocket.cpp
   By: Azaria
   Created: 2026/02/20 15:11:46
*/

# include "ClientSocket.hpp"
# include "../utils/utils.hpp"
# include "../Data/Client.hpp"

# include <unistd.h>
# include <cerrno>

/* ************************************************************************** */
/*                            Canonical Form                                  */
/* ************************************************************************** */

ClientSocket::ClientSocket()
	: Socket(), _fdAcceptor(-1)
{ }

ClientSocket::ClientSocket(const ClientSocket& clientSocketType)
	: Socket(clientSocketType)
{
	*this = clientSocketType;
}

ClientSocket& ClientSocket::operator=(const ClientSocket& clientSocketType)
{
	if (this != &clientSocketType)
	{
		Socket::operator=(clientSocketType);
	}
	return (*this);
}

ClientSocket::~ClientSocket()
{ }

/* ************************************************************************** */
/*                         Specific Constructors                              */
/* ************************************************************************** */

ClientSocket::ClientSocket(const Client &clientData)
	: Socket()
{
	this->_socketFd = clientData.getCLientInfo().fd;
	this->_addr = clientData.getCLientInfo().addr;
	this->_addrLen = clientData.getCLientInfo().addrlen;
	this->_buffer = clientData.getBuffer();
	this->_fdAcceptor = clientData.getFd();
}


/* ************************************************************************** */
/*                                Getters                                     */
/* ************************************************************************** */

std::string ClientSocket::getBuffer( void ) const
{
	return (_buffer);
}

int ClientSocket::getFdAcceptor( void ) const
{
	return (_fdAcceptor);
}

/* ************************************************************************** */
/*                                Setters                                     */
/* ************************************************************************** */

void ClientSocket::setBuffer(const std::string& buff)
{
	_buffer = buff;
}

void ClientSocket::setStartTime(uint64_t value)
{
	_start_time_ms = value;
}

/* ************************************************************************** */
/*                             Other Methods                                  */
/* ************************************************************************** */

std::string ClientSocket::recv(unsigned int count)
{
	CHARARRAY(buff, count);

	int bytes = ::read(_socketFd, buff, count);

	if (bytes < 0 && errno == EAGAIN)
		throw Eagain();
	if (bytes == 0)
		return ("");
	return (std::string(buff, bytes));
}

ssize_t ClientSocket::send(const std::string& buff)
{
	ssize_t sent = ::send(_socketFd
						, buff.c_str()
						, buff.size()
						, MSG_DONTWAIT);
	if (sent < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
		throw Eagain();
	return (sent);
}

void ClientSocket::appendBuffer(const std::string& newBuff)
{
	_buffer += newBuff;
}
