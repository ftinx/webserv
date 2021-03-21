#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <sys/time.h>
#include <string>
#include <map>

#include "Utils.hpp"

enum htmlTag
{
	TITLE,
	P,
	DIV
};

/*
**	<< General headers >>
**	- 메세지 전체에 적용
**	- Connection, Date, Keep-Alive, Transfer-Encoding
**	<< Entity headers >>
**	- 요청 본문에 적용
**	- Content-Encoding, Content-Type, Last-Modified
**	<< Response headers >>
**	- 서버에 대한 추가 정보
**	- Access-Control-Allow-Origin, Etag, Server, Set-Cookie, Vary
**	- Allow: GET, POST, HEAD
*/
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

		/* Config */
		std::vector<std::string> m_index_file;
		std::string m_err_page_path;
		std::string m_root;

		/* CGI */
		std::vector<std::string> m_cgi_extension;
		std::string m_cgi_server_name;
		in_addr_t m_cgi_client_addr;
		int m_cgi_port;
		std::string m_cgi_path;

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
		std::string get_m_err_page_path() const;
		in_addr_t get_m_cgi_client_addr() const;
		std::string get_m_cgi_server_name() const;
		int get_m_cgi_port() const;
		std::vector<std::string> get_m_cgi_extension() const;
		std::vector<std::string> get_m_index_file() const;
		std::string get_m_root() const;
		std::string get_m_cgi_path() const;

		/* setter */
		void set_m_status_code(int statusCode);
		void set_m_status_description(std::string statusDescription);
		void set_m_err_page_path(std::string err_page_path);
		void set_m_cgi_client_addr(in_addr_t cgi_client_addr);
		void set_m_cgi_server_name(std::string cgi_server_name);
		void set_m_cgi_port(int cgi_port);
		void set_m_cgi_extension(std::vector<std::string> cgi_extension);
		void set_m_index_file(std::vector<std::string> index_file);
		void set_m_root(std::string root);
		void set_m_cgi_path(std::string cgi_path);

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
		Response& setPublicFileDocument(std::string publicPath);
		Response& setBodyDocument(std::string body);

		/* util */
		Response& setCurrentDate(int hour=0, int minute=0, int second=0);
};

#endif
