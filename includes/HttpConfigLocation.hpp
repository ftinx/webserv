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
		std::vector<Method> get_m_limit_except() const;
		std::string get_m_root() const;
		std::vector<std::string> get_m_index() const;
		std::vector<std::string> get_m_cgi() const;
		std::string get_m_cgi_path() const;
		bool get_m_autoindex() const;

		/* setter */

		/* utils */
		static bool checkCommentLine(std::string str);

		/* key func. */
		HttpConfigLocation& parseLocationBlock(std::vector<std::string> lines, int &idx);
};

#endif
