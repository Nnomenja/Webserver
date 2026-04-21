/*
   File: Signal.hpp
   By: Azaria
   Created: 2026/03/02 10:20:58
*/

#ifndef SIGNAL_HPP
#define SIGNAL_HPP

# include <signal.h>

class Signal
{
	public :
		~Signal();

    static void set(int sig, void (*func)(int));

	private :
		Signal();
		Signal(const Signal& signalType);
		Signal& operator=(const Signal& signalType);

};

#endif /* SIGNAL_HPP */
