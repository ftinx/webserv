#ifndef HTTPCONFIG_HPP
# define HTTPCONFIG_HPP

# include <string>

# include "HttpConfigServer"

class HttpConfig
{
	private:
		std::string m_include;
		std::string m_root;
		vector<HttpConfigServer> m_httpServer();
	private:
		HttpConfig();
		virtual ~HttpConfig();
		HttpConfig(const HttpConfig &other);
		HttpConfig& operator=(const HttpConfig &rhs);
	public:
		void	configFileToString(std::ifstream, std::string);
		bool	checkStartHttp(std::string);
		bool	checkCurlyBrackets(std::string);
		bool	checkValidHttpConfig(void);
}

#endif
