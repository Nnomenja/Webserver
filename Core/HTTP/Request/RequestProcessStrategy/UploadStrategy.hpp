/*
   File: UploadStrategy.hpp
   By: Azaria
   Created: 2026/04/08 16:13:50
*/

#ifndef UPLOADSTRATEGY_HPP
#define UPLOADSTRATEGY_HPP

# include "IRequestStrategy.hpp"

class UploadStrategy : public IRequestStrategy
{
    public :
        UploadStrategy();
        ~UploadStrategy();

        void process(Client* client, Epoll &epoll, Process &process);

    private :

};

#endif /* UPLOADSTRATEGY_HPP */

