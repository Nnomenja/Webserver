# include "./Core/HTTP/Webserv.hpp"
# include "./Core/Network/Signal.hpp"

volatile sig_atomic_t stop = 0;

void handler(int signal)
{
	static_cast<void>(signal);
	stop = 1;
}

int main(int c, char **v)
{
	if (c != 2)
	{
		std::cout << "Usage: ./webserv [configuration file]" << std::endl;
		return (1);
	}

	Signal::set(SIGINT, handler);

	Webserv webserv(v[1]);

	if (webserv.init())
		webserv.run();

	return (0);
}
