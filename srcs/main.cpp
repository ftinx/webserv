// #include "ServerManager.hpp"
// #include <iostream>

// int
// main(int argc, char *argv[])
// {
// 	ServerManager serverManager;

// 	if (argc != 2)
// 		return (EXIT_FAILURE);
// 	try
// 	{
// 		serverManager.parseHttpConfig(argv[1]);
// 		serverManager.initServers();
// 	}
// 	catch (const std::exception& e)
// 	{
// 		std::cerr << "[Err] main.c: " << e.what() << std::endl;
// 		return (EXIT_FAILURE);
// 	}
// 	serverManager.runServers();
// 	return (EXIT_SUCCESS);
// }

#include "ServerManager.hpp"
#include <iostream>

int
main(int argc, char *argv[])
{
	std::string config_path;
	ServerManager serverManager;

	if (argc == 1)
		config_path = std::string("config/default.conf");
	else if (argc == 2)
		config_path = std::string(argv[1]);
	try
	{
		serverManager.parseHttpConfig(config_path);
		serverManager.initServers();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	serverManager.runServers();
	return (EXIT_SUCCESS);
}
