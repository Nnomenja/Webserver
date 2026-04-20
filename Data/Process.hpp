#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <map>
#include <ctime>
#include <stdint.h>
#include "../utils/utils.hpp"
class Process
{
    private:
        typedef struct s_process
        {
            int         clientFd;
            uint64_t    time_ms;
        }   t_process;
        std::map<int, t_process> _processes; // int fd, t_process   
    public:
        Process();
        ~Process();

        bool    isProcess(int fd) const;
        void    addProcess(int fd, int clientFd);
        void    removeProcess(int fd);
        int     getClientFd(int fd) const;
        int     getProcessSize() const;
        bool    isTimeout(int timeout_ms, int fd);
};

#endif