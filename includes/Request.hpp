#ifndef REQUEST_HPP
# define REQUEST_HPP

/*
** rfc 7230 3.1.1
*/

#include <string>
#include <map>
#include "Uri.hpp"

class Request
{
	private:
		std::string m_message;
		std::string m_http_version;
		std::string m_cgi_version;
		bool m_check_cgi;
		std::string m_method;
		Uri m_uri;
		std::map<std::string, std::string> m_headers;
		std::string m_body;
		int m_error_code;

	private:
		Request(Request const &other);
		Request& operator=(Request const &rhs);

	public:
		Request();
		~Request();
		std::string get_m_message() const;
		std::string get_m_http_version() const;
		//getCgiVersion
		bool get_m_check_cgi() const;
		std::string get_m_method() const;
		Uri get_m_uri() const;
		void printHeaders() const;
		std::string get_m_body() const;
		int	get_m_error_code() const;
		void set_m_http_version(std::string);
		void set_m_cgi_version(std::string);
		void set_m_check_cgi(bool);
		void set_m_method(std::string);
		//setHeaders
		void set_m_body(std::string);
		void set_m_error_code(int);
		bool parseMessage(std::string);
		bool parseRequestLine(std::string);
		bool checkMethod();
		void checkCGI();
		bool parseHeader(std::string);
		bool checkBlankLine(std::string);
		bool parseBody(std::string);
		void checkChunked(std::string);
};

std::ostream& operator<<(std::ostream &os, const Request &ref);

#endif
