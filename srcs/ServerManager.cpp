#include "ServerManager.hpp"

ServerManager::ServerManager(){};
ServerManager::~ServerManager(){};

void
ServerManager::parseHttpConfig()
{
	this->m_httpConfig.setConfigFileCheckValid("ftinx_sample.conf");
	return ;
}

Server
ServerManager::generateServer(int port)
{
	Server server;
	server.init();
	server.setServerAddr(port);
	server.setServerSocket();
	server.runServer();
	return (server);
}

void
ServerManager::runServers()
{
	this->m_server.push_back(generateServer(3601));
	return ;
}

void
ServerManager::exitServers()
{
	return ;
}
