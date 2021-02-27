#include "Server.hpp"
#include "Response.hpp"

int
main()
{
	Server server = Server();

	server.setServerAddr(1,2);
	server.setServerSocket();
	server.runServer();

	// Response response = Response();
	// response.setHtmlDocument();
	// printf("%s", response.makeResponseMessage().c_str());
	return (EXIT_SUCCESS);
}
