#include "ServerManager.hpp"
#include <iostream>

int
main()
{
	ServerManager serverManager;

	try
	{
		serverManager.parseHttpConfig();
		serverManager.initServers();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	serverManager.runServers();
	return (EXIT_SUCCESS);
}
