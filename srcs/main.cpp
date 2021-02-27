#include "Server.hpp"
#include "Response.hpp"

int
main()
{
	// Server server = Server();

	// server.setServerAddr();
	// server.setServerSocket();
	// server.runServer();

	Response response = Response();
	// response.setHtmlDocument();
	// printf("%s", response.get_m_html_document().c_str());
	response.makeResponseMessage();

	return (EXIT_SUCCESS);
}
