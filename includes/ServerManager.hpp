#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

#include <string>
#include <vector>

#include "Server.hpp"
#include "HttpConfig.hpp"

class ServerManger
{
	private:
		std::string m_ip_address;
		std::vector<Server> m_server;
		HttpConfig m_httpConfig;

	private:
		ServerManger(ServerManger const &other);
		ServerManger& operator=(ServerManger const& rhs);

	public:
		ServerManger();
		~ServerManger();
		void checkIPAddress();
};

#endif
