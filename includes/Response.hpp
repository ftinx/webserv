#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <sys/time.h>
#include <string>
#include <map>

enum htmlTag
{
	TITLE,
	P,
	DIV
};

class Response
{
	private:
		int m_status_code;
		std::string m_date;
		std::string m_content_language;
		std::string m_content_type;
		std::string m_server;
		std::string m_status_description;

		/* HTML document */
		std::map<std::string, std::string> m_headers;
		std::string m_html_document;
		std::string m_body;
		std::string m_head;
		int m_content_length;

		/* response message */
		std::string m_response_message;
		int m_response_size;

	public:
		Response();
		virtual ~Response();
		Response(Response const &other);
		Response& operator=(Response const &rhs);

		/* getter */
		int get_m_status_code() const;
		std::string get_m_html_document() const;
		std::string get_m_reponse_message() const;
		int get_m_response_size() const;
		std::string get_m_body() const;
		int get_m_content_length() const;
		std::string get_m_date() const;
		std::string get_m_content_language() const;
		std::string get_m_content_type() const;
		std::string get_m_server() const;

		/* setter */
		void set_m_status_code(int statusCode);
		void set_m_status_description(std::string statusDescription);

		/* CGI */
		bool parseCgiResponse(std::string&);

		/* HTTP Response Util */
		Response& makeHttpResponseMessage();
		std::string httpResponseStartLine(std::string httpVersion, int statusCode);
		std::string httpResponseHeader();
		std::string setCRLF();
		Response& setHttpResponseHeader(std::string key, std::string value);
		Response& setStatusCode(int statusCode);
		Response& setContentLanguage(std::string contentLanguage);
		Response& setContentType(std::string contentType);
		Response& setServer(std::string server);

		/* HTML Document Util */
		Response& setHtmlDocument();
		std::string set404HtmlDocument();
		Response& setHtmlAttribute(htmlTag tag, std::string value);
		void setTitleTag(std::string value);
		void setPTag(std::string value);
		void setDivTag(std::string value);

		/* util */
		std::string getDate();
		Response& setCurrentDate();
};

#endif
