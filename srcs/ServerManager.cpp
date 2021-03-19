#include "ServerManager.hpp"

/*============================================================================*/
/****************************  Static variables  ******************************/
/*============================================================================*/

/*============================================================================*/
/******************************  Constructor  *********************************/
/*============================================================================*/


ServerManager::ServerManager(){};
ServerManager::ServerManager(ServerManager const &other)
{
	*this = other;
};
ServerManager& ServerManager::operator=(ServerManager const &rhs)
{
	m_httpConfig = rhs.m_httpConfig;

	m_server_block = rhs.m_server_block;
	m_server = rhs.m_server;
	m_server_size = rhs.m_server_size;

	m_software_name = rhs.m_software_name;
	m_software_version = rhs.m_software_version;
	m_mime_include = rhs.m_mime_include;
	m_root = rhs.m_root;
	return (*this);
};

/*============================================================================*/
/******************************  Destructor  **********************************/
/*============================================================================*/

ServerManager::~ServerManager(){};

/*============================================================================*/
/*******************************  Overload  ***********************************/
/*============================================================================*/

/*============================================================================*/
/********************************  Getter  ************************************/
/*============================================================================*/

/*============================================================================*/
/********************************  Setter  ************************************/
/*============================================================================*/

/*============================================================================*/
/******************************  Exception  ***********************************/
/*============================================================================*/

/*============================================================================*/
/*********************************  Util  *************************************/
/*============================================================================*/

void
ServerManager::storeParseValue()
{
	this->m_software_name = this->m_httpConfig.get_m_name();
	this->m_software_version = this->m_httpConfig.get_m_version();
	this->m_mime_include = this->m_httpConfig.get_m_include();
	this->m_root = this->m_httpConfig.get_m_root();
	this->m_server_size = this->m_httpConfig.get_m_server_block().size();
	this->m_server_block = this->m_httpConfig.get_m_server_block();
	return ;
}

void
ServerManager::parseHttpConfig()
{
	// this->m_httpConfig.setConfigFileCheckValid("ftinx_sample.conf");
	this->m_httpConfig.parseConfigFile("ftinx_sample.conf");
	// this->m_httpConfig.printConfigFileInfo();

	storeParseValue();
	return ;
}

Server
ServerManager::generateServer(HttpConfigServer server_block, std::string server_name, int port, std::string err_page_path, int content_length, size_t location_size)
{
	Server server;
	server.init(server_block, server_name, port, err_page_path, content_length,location_size);
	server.setServerAddr(port);
	server.setServerSocket();
	return (server);
}

void
ServerManager::initServers()
{
	for (size_t i = 0; i < m_server_size; i++) {
		this->m_server.push_back(
			generateServer(
				this->m_server_block[i],
				this->m_server_block[i].get_m_server_name(),
				this->m_server_block[i].get_m_listen(),
				this->m_server_block[i].get_m_default_error_page(),
				this->m_server_block[i].get_m_content_length(),
				this->m_server_block[i].get_m_location_block().size()
			)
		);
	}
	return ;
}

void
ServerManager::runServers()
{
	for (size_t i = 0; i < m_server_size; i++) {
		this->m_server[i].runServer();
	}
	return ;
}

void
ServerManager::exitServers()
{
	return ;
}
