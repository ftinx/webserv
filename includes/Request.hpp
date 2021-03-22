#ifndef REQUEST_HPP
# define REQUEST_HPP

/*
** rfc 7230 3.1.1
*/

#include <string>
#include <map>
#include "Uri.hpp"
#include "Utils.hpp"

#define MAXLINE 2048
#define SOCK_SETSIZE 1021

class Request
{
	private:
		std::string m_message;
		std::string m_http_version;
		std::string m_cgi_version;
		bool m_check_cgi;
		Method m_method;
		Uri m_uri;
		std::map<std::string, std::string> m_headers;
		std::string m_body;
		int m_error_code;

	public:
		Request();
		Request(Request const &other);
		Request& operator=(Request const &rhs);
		~Request();

		/* getter */
		std::string get_m_message() const;
		std::string get_m_http_version() const;
		std::string get_m_cgi_version() const;
		bool get_m_check_cgi() const;
		Method get_m_method() const;
		Uri get_m_uri() const;
		std::map<std::string, std::string> get_m_headers() const;
		std::string get_m_body() const;
		int	get_m_error_code() const;
		void set_m_http_version(std::string);
		void set_m_cgi_version(std::string);
		void set_m_check_cgi(bool);
		void set_m_method(Method);
		//setHeaders
		void set_m_body(std::string);
		void set_m_error_code(int);
		std::string getMethod();
		std::string getContentLength();
		std::string getContentType();
		bool isBreakCondition(bool*, int, int);
		bool getMessage(int);
		bool parseMessage();
		bool parseRequestLine(std::string);
		bool checkMethod();
		bool checkCGI();
		bool parseHeader(std::string);
		bool checkBlankLine(std::string);
		bool parseBody(std::string, int, int);
		void checkChunked(std::string);
		void printHeaders() const;
};

std::ostream& operator<<(std::ostream &os, const Request &ref);

#endif
