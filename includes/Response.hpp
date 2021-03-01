#ifndef RESPONSE_HPP
# define RESPONSE_HPP

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
		int get_m_status_code();
		std::string get_m_html_document();
		std::string get_m_reponse_message();
		int get_m_response_size();
		std::string get_m_body();
		int get_m_content_length();

		/* setter */
		void set_m_status_code(int statusCode);
		void set_m_status_description(std::string statusDescription);

		bool parseCgiResponse(std::string&);
		Response& setHtmlDocument();
		Response& makeHttpResponseMessage();

		/* HTTP Response Util */
		std::string httpResponseStartLine(std::string httpVersion, int statusCode);
		std::string httpResponseHeader();
		std::string setCRLF();
		Response& setHttpResponseHeader(std::string key, std::string value);
		Response& setStatusCode(int statusCode);

		/* HTML Document Util */
		Response& setAttribute(htmlTag tag, std::string value);
		void setTitleTag(std::string value);
		void setPTag(std::string value);
		void setDivTag(std::string value);
};

#endif
