#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

#include <string>
#include <vector>

#include "Server.hpp"
#include "HttpConfig.hpp"

class ServerManager
{
	private:
		std::string m_ip_address;
		std::vector<Server> m_server;
		HttpConfig m_httpConfig;

	private:
		ServerManager(ServerManager const &other);
		ServerManager& operator=(ServerManager const &rhs);

	public:
		ServerManager();
		~ServerManager();
		void checkIPAddress();
};

#endif
