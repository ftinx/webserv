#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

#include <string>
#include <vector>

#include "Server.hpp"
#include "HttpConfig.hpp"

class ServerManager
{
	private:
		HttpConfig m_httpConfig;

		std::map<std::string, std::string> m_mime_types;
		std::vector<HttpConfigServer> m_server_block;
		std::vector<Server*> m_server;
		size_t m_server_size;

		std::string m_software_name;
		std::string m_software_version;
		std::string m_mime_include;
		std::string m_root;

	private:
		void storeParseValue();

	public:
		ServerManager();
		~ServerManager();
		ServerManager(ServerManager const &other);
		ServerManager& operator=(ServerManager const &rhs);

		void parseHttpConfig();
		Server generateServer(
			HttpConfigServer server_block,
			std::string server_name,
			int port,
			std::string err_page_path,
			int content_length,
			size_t location_size,
			std::string root,
			std::map<std::string, std::string> mime_types
		);
		void initServers();
		void runServers();
		void exitServers();
};

#endif
