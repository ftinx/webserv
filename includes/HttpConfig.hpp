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

# define BUFFER_SIZE 4096

class HttpConfig
{
	private:
		std::string m_config_file; // config파일이 통째로 저장될 문자열
		std::string m_include;
		std::string m_root;
		std::vector<HttpConfigServer> m_httpServer();

	private:
		HttpConfig();
		virtual ~HttpConfig();
		HttpConfig(HttpConfig const &other);
		HttpConfig& operator=(HttpConfig const &rhs);

	public:
		HttpConfig(std::string file_path);

		void saveConfigFileToString(std::string file_path); // 파일을 읽어서 m_config_file에 저장 (가공하지 않은 상태)
		bool checkStartHttp(); // m_config_file의 시작이 "http" 인지 여부 확인 -> 꼭 필요할까? 전체 문법 확인할 때 거를까?
		bool checkCurlyBrackets(); // 중괄호의 갯수와 열고 닫음에 에러가 없는지 확인
		bool checkValidHttpConfig();
};

#endif
