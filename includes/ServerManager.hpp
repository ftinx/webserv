#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

#include <string>
#include <vector>

#include <signal.h>

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

		/* Socket */
		int m_maxfd;
		fd_set m_main_fds, m_copy_fds;
		fd_set m_read_fds, m_write_fds;
		fd_set m_copy_write_fds;

	private:
		void storeParseValue();

	public:
		ServerManager();
		~ServerManager();
		ServerManager(ServerManager const &other);
		ServerManager& operator=(ServerManager const &rhs);

		void parseHttpConfig(std::string config_path);
		Server generateServer(
			HttpConfigServer server_block,
			std::string server_name,
			int port,
			int content_length,
			size_t location_size,
			std::string root,
			std::map<std::string, std::string> mime_types,
			int maxfd, fd_set main_fds, fd_set read_fds, fd_set write_fds, fd_set copy_write_fds
		);
		void initServers();
		void runServers();
		void exitServers(int signo);
};

#endif
