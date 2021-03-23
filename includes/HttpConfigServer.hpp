#ifndef HTTPCONFIGSERVER_HPP
# define HTTPCONFIGSERVER_HPP

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

	public:
		HttpConfigServer();
		HttpConfigServer(HttpConfigServer const &other);
		HttpConfigServer& operator=(HttpConfigServer const &rhs);
		~HttpConfigServer();

		/* getter */
		std::string get_m_server_name() const;
		int get_m_listen() const;
		std::string get_m_default_error_page() const;
		int get_m_content_length() const;
		const std::vector<HttpConfigLocation> get_m_location_block() const;

		/* setter */

		void set_m_root(std::string m_root);

		/* key func. */
		HttpConfigServer& parseServerBlock(std::vector<std::string> lines, std::string root, size_t &idx);
};

#endif
