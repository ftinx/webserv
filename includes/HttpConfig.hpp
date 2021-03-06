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

class HttpConfig
{
	private:
		std::string m_file_path; // config file addr
		std::string m_config_file; // config file -> string
		std::vector<std::string> m_lines; // m_config_file -> lines(\n)

		std::string m_name;
		std::string m_version;
		std::string m_include;
		std::string m_root;
		std::vector<HttpConfigServer> m_server_block;

	private:
		HttpConfig();
		HttpConfig& operator=(HttpConfig const &rhs);

		/* utils */
		bool checkStartHttp(); // m_liens의 시작이 "http" 인지 여부 확인
		bool checkBlankLine(std::string str);
		bool checkCurlyBracketsFaired(); // 중괄호의 갯수와 열고 닫음에 에러가 없는지 확인
		bool checkCurlyBracketsDouble(std::string str);

		/* debug */
		// void printParseInfo();

	public:
		HttpConfig(std::string file_path);
		HttpConfig(HttpConfig const &other);
		~HttpConfig();

		/* getter */
		std::string get_m_name() const;
		std::string get_m_version() const;
		std::string get_m_include() const;
		std::string get_m_root() const;
		std::vector<HttpConfigServer> get_m_server_block() const;

		/* setter */
		// void set_m_lines();
		void setConfigFile();
		void setConfigLines();
		void checkValidHttpBlock();

		/* key func. */
		void parseHttpBlock();
};

#endif
