#ifndef HTTPREQUESTPARSER_HPP
#define HTTPREQUESTPARSER_HPP

#include "../../Data/Client.hpp"

/**
 * @brief  HttpRequestParser.cpp
 * @details 
 */

class HttpRequestParser {
	private:
		bool	_finished;
    public:
        HttpRequestParser();
        HttpRequestParser(const HttpRequestParser& other);
        HttpRequestParser& operator=(const HttpRequestParser& other);
        ~HttpRequestParser();

        /**
         * @details
         * 		1. Get parse state from client's request
         * 		2. Apply parsing  in the according method
         * 		3. In the case where
         */
        void    parse(Request *req);
		bool	finished();	

};

#endif
