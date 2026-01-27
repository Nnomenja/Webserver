all: server

server: 
	c++ server.cpp -o server

clean: 
	rm -rf server

re:  clean all