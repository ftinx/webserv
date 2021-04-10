#ifndef HTTPCONFIG_HPP
# define HTTPCONFIG_HPP

# include <string>
# include <fcntl.h>
# include <unistd.h>
# include <iostream>
# include <stack>
# include <map>
# include "HttpConfigServer.hpp"
# include "Utils.hpp"
# include "Exception.hpp"

class HttpConfig
{
	private:
		std::string m_config_file; // config file -> string
		std::vector<std::string> m_lines; // m_config_file -> split lines(\n)

		std::string m_name;
		std::string m_version;
		std::string m_include;
		std::string m_default_type;
		std::string m_root;
		std::vector<HttpConfigServer> m_server_block;
		std::map<std::string, std::string> m_mime_types;

	private:
		/* utils */
		bool checkCurlyBracketsFaired(); // 중괄호의 갯수와 열고 닫음에 에러가 없는지 확인
		void parseMimeTypes();
		void setDefaultRootPath();

	public:
		HttpConfig();
		HttpConfig(HttpConfig const &other);
		HttpConfig& operator=(HttpConfig const &rhs);
		~HttpConfig();

		/* getter */
		std::string get_m_name() const;
		std::string get_m_version() const;
		std::string get_m_include() const;
		std::string get_m_default_type() const;
		std::string get_m_root() const;
		std::vector<HttpConfigServer> get_m_server_block() const;
		std::map<std::string, std::string> get_m_mime_types() const;

		/* setter */

		/* key func. */
		void parseConfigFile(std::string &file_path);

		/* debug */
		void printConfigFileInfo();
};

#endif
