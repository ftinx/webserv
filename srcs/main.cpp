#include "ServerManager.hpp"
#include <iostream>

int
main(int argc, char *argv[])
{
	ServerManager serverManager;

	if (argc != 2)
		return (EXIT_FAILURE);
	try
	{
		serverManager.parseHttpConfig(argv[1]);
		serverManager.initServers();
	}
	catch (const std::exception& e)
	{
		std::cerr << "[Err] main.c: " << e.what() << std::endl;
	}
	serverManager.runServers();
	return (EXIT_SUCCESS);
}
