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
		std::vector<std::string> m_auth_basic;
		std::string m_auth_basic_user_file;
		int m_limit_body_size;

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
		std::vector<std::string> get_m_auth_basic() const;
		std::string get_m_auth_basic_user_file() const;
		int get_m_limit_body_size() const;

		/* setter */
		void set_m_root(std::string root);

		/* utils */
		static bool checkCommentLine(std::string str);
		static void checkDirExist(std::string path);
		static void checkFileExist(std::string root, std::string path);

		/* exceptions */
		class parseErrorException : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};

		/* key func. */
		HttpConfigLocation& parseLocationBlock(std::vector<std::string> lines, std::string root, size_t &idx);
};

#endif
