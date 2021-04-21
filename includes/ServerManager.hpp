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
		std::string m_tmp_path;
		std::string m_default_type;

		/* Socket */
		int m_maxfd;
		fd_set m_main_fds, m_copy_fds;
		fd_set m_read_fds, m_write_fds;
		fd_set m_copy_write_fds;

		/* Flag */
		std::string m_err_message;

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
			std::string tmp_path,
			std::map<std::string, std::string> mime_types,
			std::string default_type,
			int maxfd, fd_set main_fds, fd_set read_fds, fd_set write_fds, fd_set copy_write_fds
		);
		std::string initServers();
		void runServers();
		static void exitServers(int signo);
};

#endif
