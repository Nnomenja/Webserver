/*
   File: Signal.cpp
   By: Azaria
   Created: 2026/03/02 10:21:13
*/

#include "Signal.hpp"

/* ************************************************************************** */
/*                            Canonical Form                                  */
/* ************************************************************************** */

Signal::Signal()
{ }

Signal::Signal(const Signal& signalType)
{
	*this = signalType;
}

Signal& Signal::operator=(const Signal& signalType)
{
	static_cast<void>(signalType);
	return (*this);
}

Signal::~Signal()
{ }

/* ************************************************************************** */
/*                             Other Methods                                  */
/* ************************************************************************** */

void Signal::set(int sig, void (*func)(int))
{
	::signal(sig, func);
}
