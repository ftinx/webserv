#ifndef HTTPCONFIGLOCATION_HPP
# define HTTPCONFIGLOCATION_HPP

# include <string>
# include "Utils.hpp"

enum Method
{
	DEFAULT,
	GET,
	HEAD,
	POST,
	PUT,
	DELETE,
	OPTIONS,
	TRACE,
	ERROR,
};

class HttpConfigLocation
{
	private:
		std::string m_path;
		std::vector<Method> m_limit_except;
		std::string m_root;
		std::vector<std::string> m_index;
		std::vector<std::string> m_cgi;
		std::string m_cgi_path;

	private:
		HttpConfigLocation(HttpConfigLocation const &other);
		HttpConfigLocation& operator=(HttpConfigLocation const &rhs);

		Method convertStringToMethod(std::string str);

	public:
		HttpConfigLocation();
		virtual ~HttpConfigLocation();

		/* getter */
		std::string get_m_path() const;
		std::string get_m_root() const;
		std::string get_m_cgi_path() const;

		/* setter */

		/* key func. */
		int parseLocationBlock(std::vector<std::string> lines, int idx);
};

#endif
