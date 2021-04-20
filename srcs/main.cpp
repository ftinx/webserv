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
		std::string err_message = "";
		serverManager.parseHttpConfig(config_path);
		err_message = serverManager.initServers();
		if (err_message != "")
		{
			std::cout << err_message + ": Please make the port idle." << std::endl;
			return (EXIT_SUCCESS);
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	serverManager.runServers();
	return (EXIT_SUCCESS);
}
