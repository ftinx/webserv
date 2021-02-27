#include "Server.hpp"

int
main()
{
	Server server = Server();

	server.setServerAddr();
	server.setServerSocket();
	server.runServer();

	return (EXIT_SUCCESS);
}
