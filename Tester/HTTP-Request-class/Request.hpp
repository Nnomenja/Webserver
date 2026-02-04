/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnomenja <nnomenja@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 10:22:53 by nnomenja          #+#    #+#             */
/*   Updated: 2026/02/01 20:53:55 by nnomenja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <strings.h>
#include <vector>
#include <iostream>
#include <exception>
#include <vector>
#include <algorithm>
#include <sstream>
#include <map>
#include <ctype.h>
#include <sys/socket.h>

#define BUFFER_MAX 4000

#define CRLF "\r\n"

class Request {
    private:
        std::string       	&_buffer;
        int                 _receivedByte;
		int	                _i;

        std::string		    _method;
        std::string		    _pathname;
        std::string		    _file;
        std::string		    _tmp;
        std::string 	    _httpVersion;
        bool                _hasPourcentEncode;
        bool			    _hasQuery;
        bool			    _hasContentLength;
        ssize_t             _contentLength;

        int                             _port;
        std::map<std::string, std::string> _headers;
        /*------- Utils -------*/
        bool    checkHeaderLineEncode(char c) const;
        bool    checkPathEncode(char c) const;
        bool    checkPathPourcentEncoded(char c) const; 
        bool    checkQueryPourcentEncoded(char c) const;
        bool    checkQueryEncode(char c) const;
        int     HexaToInt(char c) const;
		bool	isHexa(char c) const;
        char    PourcentHexaToChar(char first, char second) const;
        bool    checkHeaderEncode(char c) const;
        std::string &toLowerCase(std::string &src) const;
        /*------- Request Line -------*/
        void    setPathname();
        void    setMethod();
        void    readProtocol();
        void    setHttpVersion();
        char    readByte(); 
        void    readQuery();
        char    readPourcentHexa();
		void	normalizePathname();
        void    DecodePourcentEncode();

        /*------- Header -------*/
        void    readHeader();
        void    setHost();
        void    setContentLength();
	public:

        /*------- Getters -------*/
        std::string getMethod() const;
        std::string getPathName() const;
        std::string getHTTPversion() const;
        bool        getHasQuery() const;
        int         getPort() const;
        int         getContentLength() const;

        class NotImplementedMethod: public std::exception{
            const char *what () const throw();
        };
        class InvalidMethod: public std::exception{
            const char *what () const throw();
        }; 
        class BadRequest: public std::exception{
            const char *what () const throw();
        }; 
        class URITooLong: public std::exception{
            const char *what () const throw();
        }; 
        class NotFound: public std::exception{
            const char *what () const throw();
        };

        Request(std::string &value, int receivedBytes);
        ~Request();
};

#endif
