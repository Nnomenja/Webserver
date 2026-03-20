/*
   File: main.cpp
   By: Azaria
   Created: 2026/02/26 12:30:39
*/


# include "./Core/Webserv.hpp"
# include "./Core/Signal.hpp"

volatile sig_atomic_t stop = 0;

void handler(int signal)
{
	static_cast<void>(signal);
	stop = 1;
}

int main(int c, char **v)
{
	if (c != 2)
		return (1);

	Signal::set(SIGINT, handler);

	Webserv webserv(v[1]);

	if (webserv.init())
		webserv.run();

	return (0);
}
