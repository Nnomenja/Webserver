#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <poll.h>
#include <cstring>

#define CRLF "\r\n"
#define MAX 1024

 int main(int argc, char const *argv[])
 {
	int serverFd;
	int clientFd;

	sockaddr_in serverAddr;
	sockaddr_in clientAddr;
	char buffer[1024] = {0};
	int receivedByte = 0;

	serverFd  = socket(AF_INET, SOCK_STREAM, 0);
	if (serverFd < 0)
	{
		perror("Socket: ");
		return (1);
	}
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(8080);
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	int opt = 1;
    if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        close(serverFd);
        return EXIT_FAILURE;
    }

	if (bind(serverFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)))
	{
		perror("Bind: ");
		return (1);
	}
	listen(serverFd, 50);
	std::cout << "Start listing..." << std::endl;


	pollfd fds[MAX];
    //*------- Set server -------*/

    fds[0].fd = serverFd;
    fds[0].events = POLLIN;

    //*------- Fill the value of client -------*/    

    for (size_t i = 1; i < MAX; i++)
    {
        fds[i].fd = -1;
        fds[i].events = POLLIN;
    }
    
	int ret;
	int i;
	while (1)
	{
		// std::cout << "Waitting new client..." << std::endl;
        ret = poll(fds, MAX, 0);
		if (ret > 0) {
			if (fds[0].revents & POLLIN)
			{
				i = 0;
                while (++i < 1024)
                {
                    if (fds[i].fd == -1)
                    {
                        fds[i].fd = accept(serverFd, NULL, NULL);
						fds[i].revents = 0;
						std::cout << "New client added["<< fds[i].fd << " | " << i <<"]..." << std::endl;
						break;
                    }
				}
				continue;
			}
			i = 0;
			while (++i < MAX)
			{
				if (!fds[i].revents)
					continue;
				if (fds[i].revents == POLLIN)
				{
					receivedByte = recv(fds[i].fd, buffer, sizeof(buffer), 0); 
					if (receivedByte == 0)
					{
						std::cout << "client disconnected" << std::endl;
						return (1);
					}
					else if (receivedByte == -1)
					{
						perror("Received");
						return (1);
					}
					std::cout << "-----------------" << fds[i].fd << "------------------" << std::endl;
					std::cout << buffer << std::endl;
					std::cout << "-----------------------------------" << std::endl;
					fds[i].events = POLLOUT;
				}
				else if (fds[i].revents == POLLOUT)
				{
					std::string http_response = 
					"HTTP/1.1 200 OK\r\n"
					"Content-Type: text/plain\r\n"
					"Content-Length: 13\r\n"
					"\r\n"
					"Hello, World!";
					send(fds[i].fd, http_response.c_str(), http_response.length(), 0);
					close(fds[i].fd);
					fds[i].fd = -1;
					fds[i].events = POLLIN;
				}
			}
		} else if (ret == 0) {
			// std::cout << "No client found!" << std::endl;
		} else {
			perror("poll");
		}
	}
	close(serverFd);
   return 0;
 }
