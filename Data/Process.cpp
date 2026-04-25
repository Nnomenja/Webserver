#include "Process.hpp"
#include <iostream>
Process::Process()
{ }

Process::~Process()
{ }

bool Process::isProcess(int fd) const
{
    return (_processes.find(fd) != _processes.end());
}

void Process::addProcess(int fd, int clientFd)
{
    _processes[fd].clientFd = clientFd;
    _processes[fd].time_ms = get_time_ms();
}

void Process::removeProcess(int fd)
{
    _processes.erase(fd);
}

int Process::getClientFd(int fd) const
{
    return (_processes.at(fd).clientFd);
}

int Process::getProcessSize() const
{
    return _processes.size();
}

bool Process::isTimeout(int timeout_ms, int fd)
{
    return (verify_deadline_ms(_processes.at(fd).time_ms, timeout_ms));
}
