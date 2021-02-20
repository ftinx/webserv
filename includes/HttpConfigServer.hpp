#ifndef HTTPCONFIGSERVER_HPP
# define HTTPCONFIGSERVER_HPP

# include <string>

#include "HttpConfigLocation"

class HttpConfigServer
{
	private:
		std::string m_server_name;
		int m_listen;
		std::string m_default_error_page;
		int m_content_length;
		vector<HttpConfigLocation> m_location();

	private:
		HttpConfigServer();
		virtual ~HttpConfigServer();
		HttpConfigServer(HttpConfigServer const &other);
		HttpConfigServer& operator=(HttpConfigServer const &rhs);
	public:
		bool	checkStartServer(std::string);
		bool	checkValidHttpConfigServer();
}

#endif
