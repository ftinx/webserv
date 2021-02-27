#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <string>
#include <map>

class Response
{
	private:
		int m_status_code;
		std::string m_status_description;
		std::map<std::string, std::string> m_headers;
		std::string m_html_document;

	public:
		Response();
		virtual ~Response();
		Response(Response const &other);
		Response& operator=(Response const &rhs);

		std::string get_m_html_document();

		bool	parseCgiResponse(std::string&);
		void	setStatusCode(int statusCode);
		void	setStatusDescription(std::string statusDescription);
		void	addHeaders(std::string key, std::string value);
		void	setHtmlDocument();
		std::string	makeResponseMessage();
};

#endif
