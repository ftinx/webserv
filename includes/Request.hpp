#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <string>
#include <map>

class Request
{
	private:
		std::string m_http_version;
		std::string m_cgi_version;
		bool m_check_cgi;
		std::string m_method;
		std::string m_path;
		std::map<std::string, std::string> m_headers;
		std::string m_body;

	private:
		Request(Request const &other);
		Request& operator=(Request const &rhs);

	public:
		Request();
		~Request();
		void parseMessage(std::string);
		void getNextLine(std::string);
		void parseHttpRequest(std::string);
		void checkCGI(std::string);
		void parseHeader(std::string);
		void parseBody(std::string);
		void checkChunked(std::string);
};

#endif
