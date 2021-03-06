#ifndef HTTPCONFIGLOCATION_HPP
# define HTTPCONFIGLOCATION_HPP

# include <string>
# include "Utils.hpp"

class HttpConfigLocation
{
	private:
		std::string m_path;
		std::vector<Method> m_limit_except;
		std::string m_root;
		std::vector<std::string> m_index;
		std::vector<std::string> m_cgi;
		std::string m_cgi_path;
		bool m_autoindex;

	private:
		Method convertStringToMethod(std::string str);

	public:
		HttpConfigLocation();
		HttpConfigLocation(HttpConfigLocation const &other);
		HttpConfigLocation& operator=(HttpConfigLocation const &rhs);
		~HttpConfigLocation();

		/* getter */
		std::string get_m_path() const;
		std::string get_m_root() const;
		std::vector<std::string> get_m_index() const;
		std::vector<std::string> get_m_cgi() const;
		std::string get_m_cgi_path() const;

		/* setter */

		/* utils */
		static bool checkAnnotateLine(std::string str);

		/* key func. */
		HttpConfigLocation& parseLocationBlock(std::vector<std::string> lines, int &idx);
};

#endif
