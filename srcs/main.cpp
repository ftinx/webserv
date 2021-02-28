#include "Server.hpp"
#include "Response.hpp"

int
main()
{
	Server server = Server();

	server.setServerAddr(3601);
	server.setServerSocket();
	server.runServer();
	
	return (EXIT_SUCCESS);
}
