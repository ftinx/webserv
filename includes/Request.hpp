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
		std::string getHttpVersion() const;
		//getCgiVersion
		bool getCheckCgi() const;
		std::string getMethod() const;
		Uri getUri() const;
		void printHeaders() const;
		std::string getBody() const;
		int	getErrorCode() const;
		void setHttpVersion(std::string);
		void setCgiVersion(std::string);
		void setCheckCgi(bool);
		void setMethod(std::string);
		//setHeaders
		void setBody(std::string);
		void setErrorCode(int);
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
