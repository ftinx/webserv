#include "Server.hpp"
#include "Request.hpp"
#include "fcntl.h"
#include <string>
#include <iostream>

#define BUFFER_SIZE 4096
std::string fileToString(std::string file_path)
{
	int fd;
	int bytes;
	char buffer[BUFFER_SIZE];
	std::string ret;
	if ((fd = open(file_path.c_str(), O_RDONLY)) < 0)
		throw std::exception();
	memset(buffer, 0, BUFFER_SIZE);
	while ((bytes = read(fd, buffer, BUFFER_SIZE) > 0))
		ret += std::string(buffer);
	if (bytes < 0)
		throw std::exception();
	return (ret);
}

int
main()
{
	// Server server = Server();

	// server.setServerAddr();
	// server.setServerSocket();
	// server.runServer();
	std::string message = fileToString("./request_msg.txt");
	Request request;
	request.parseMessage(message);

	std::cout << request << std::endl;
	request.printHeaders();

	return (EXIT_SUCCESS);
}
