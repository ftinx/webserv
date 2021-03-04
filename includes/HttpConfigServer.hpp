#ifndef HTTPCONFIGSERVER_HPP
# define HTTPCONFIGSERVER_HPP

# include <string>
# include "HttpConfigLocation.hpp"
# include "Utils.hpp"

class HttpConfigServer
{
	private:
		std::string m_server_name;
		int m_listen;
		std::string m_default_error_page;
		int m_content_length;
		std::vector<HttpConfigLocation> m_location_block;

	private:
		HttpConfigServer(HttpConfigServer const &other);
		HttpConfigServer& operator=(HttpConfigServer const &rhs);

	public:
		HttpConfigServer();
		virtual ~HttpConfigServer();

		/* getter */
		std::string get_m_server_name() const;
		int get_m_listen() const;
		std::string get_m_default_error_page() const;
		int get_m_content_length() const;

		/* setter */

		/* key func. */
		int parseServerBlock(std::vector<std::string> lines, int idx);
};

#endif
