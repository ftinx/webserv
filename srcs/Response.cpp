#include "Response.hpp"

/*============================================================================*/
/****************************  Static variables  ******************************/
/*============================================================================*/

/*============================================================================*/
/******************************  Constructor  *********************************/
/*============================================================================*/

Response::Response()
:m_status_code(404), m_date(""), m_content_language("ko, en"), m_content_type("text/html; charset=UTF-8"),
m_server("ftnix/1.0 (MacOS)"), m_status_description(""), m_headers(), m_html_document(""), m_body(""),
m_head(""), m_content_length(0), m_response_message(""), m_response_size(0), m_index_file(),
m_err_page_path("./www/errors/default_error.html"), m_root(""), m_cgi_extension(), m_cgi_server_name(""),
m_cgi_client_addr(), m_cgi_port(0)
{
}

Response::Response(Response const &other)
{
	*this = other;
}

Response &
Response::operator=(Response const &rhs)
{
	this->m_status_code = rhs.m_status_code;
	this->m_date = rhs.m_date;
	this->m_content_language = rhs.m_content_language;
	this->m_content_type = rhs.m_content_type;
	this->m_server = rhs.m_server;
	this->m_status_description = rhs.m_status_description;

	/* HTML document */
	this->m_headers = rhs.m_headers;
	this->m_html_document = rhs.m_html_document;
	this->m_body = rhs.m_body;
	this->m_head = rhs.m_head;
	this->m_content_length = rhs.m_content_length;

	/* response message */
	this->m_response_message = rhs.m_response_message;
	this->m_response_size = rhs.m_response_size;

	/* Config */
	this->m_err_page_path = rhs.m_err_page_path;
	this->m_index_file = rhs.m_index_file;
	this->m_root = rhs.m_root;

	/* CGI */
	this->m_cgi_extension = rhs.m_cgi_extension;
	this->m_cgi_client_addr = rhs.m_cgi_client_addr;
	this->m_cgi_server_name = rhs.m_cgi_server_name;
	this->m_cgi_port = rhs.m_cgi_port;
	return (*this);
}

/*============================================================================*/
/******************************  Destructor  **********************************/
/*============================================================================*/

Response::~Response()
{
}

/*============================================================================*/
/*******************************  Overload  ***********************************/
/*============================================================================*/

/*============================================================================*/
/********************************  Getter  ************************************/
/*============================================================================*/

int
Response::get_m_status_code() const
{
	return (this->m_status_code);
}

std::string
Response::get_m_html_document() const
{
	return (this->m_html_document);
}

std::string
Response::get_m_reponse_message() const
{
	return (this->m_response_message);
}

int
Response::get_m_response_size() const
{
	return (this->m_response_size);
}

std::string
Response::get_m_body() const
{
	return (this->m_body);
}

int
Response::get_m_content_length() const
{
	return (this->m_content_length);
}

std::string
Response::get_m_date() const
{
	return (this->m_date);
}

std::string
Response::get_m_content_language() const
{
	return (this->m_content_language);
}

std::string
Response::get_m_content_type() const
{
	return (this->m_content_type);
}

std::string
Response::get_m_server() const
{
	return (this->m_server);
}

std::string
Response::get_m_err_page_path() const
{
	return (this->m_err_page_path);
}

in_addr_t
Response::get_m_cgi_client_addr() const
{
	return (this->m_cgi_client_addr);
}

std::string
Response::get_m_cgi_server_name() const
{
	return (this->m_cgi_server_name);
}
int
Response::get_m_cgi_port() const
{
	return (this->m_cgi_port);
}

std::vector<std::string>
Response::get_m_index_file() const
{
	return (this->m_index_file);
}

std::string
Response::get_m_root() const
{
	return (this->m_root);
}

std::vector<std::string>
Response::get_m_cgi_extension() const
{
	return (this->m_cgi_extension);
}

std::string
Response::get_m_cgi_path() const
{
	return (this->m_cgi_path);
}

/*============================================================================*/
/********************************  Setter  ************************************/
/*============================================================================*/

void
Response::set_m_status_code(int statusCode)
{
	this->m_status_code = statusCode;
	return ;
}

void
Response::set_m_status_description(std::string statusDescription)
{
	this->m_status_description = statusDescription;
	return ;
}

void
Response::set_m_err_page_path(std::string err_page_path)
{
	this->m_err_page_path = err_page_path;
	return ;
}

void
Response::set_m_cgi_client_addr(in_addr_t cgi_client_addr)
{
	this->m_cgi_client_addr = cgi_client_addr;
	return ;
}

void
Response::set_m_cgi_server_name(std::string cgi_server_name)
{
	this->m_cgi_server_name = cgi_server_name;
	return ;
}

void
Response::set_m_cgi_port(int cgi_port)
{
	this->m_cgi_port = cgi_port;
	return ;
}

void
Response::set_m_cgi_extension(std::vector<std::string> cgi_extension)
{
	this->m_cgi_extension = cgi_extension;
	return ;
}

void
Response::set_m_index_file(std::vector<std::string> index_file)
{
	this->m_index_file = index_file;
	return ;
}

void
Response::set_m_root(std::string root)
{
	this->m_root = root;
	return ;
}

void
Response::set_m_cgi_path(std::string cgi_path)
{
	this->m_cgi_path = cgi_path;
	return ;
}

/*============================================================================*/
/******************************  Exception  ***********************************/
/*============================================================================*/

/*============================================================================*/
/*********************************  Util  *************************************/
/*============================================================================*/

Response &
Response::setStatusCode(int statusCode)
{
	this->m_status_code = statusCode;
	return (*this);
}

void
Response::setTitleTag(std::string value)
{
	this->m_head += std::string("<title>")
		+ value
		+ std::string("</title>");
}

void
Response::setPTag(std::string value)
{
	this->m_body += std::string("<p>")
		+ value
		+ std::string("</p>");
}

void
Response::setDivTag(std::string value)
{
	this->m_body += std::string("<div>")
		+ value
		+ std::string("</div>");
	return ;
}

Response &
Response::setHtmlAttribute(htmlTag tag, std::string value)
{
	switch (tag)
	{
		case TITLE:
			setTitleTag(value);
			break;
		case P:
			setPTag(value);
			break;
		case DIV:
			setDivTag(value);
			break;
		default:
			perror("Undefined HTML Element");
			break;
	}
	return (*this);
}

std::string
Response::set404HtmlDocument()
{
	std::string body;

	body += std::string("<!DOCTYPE html>")
				+ std::string("<html lang=\"en\">")
					+ std::string("<head>")
						+ this->m_head
					+ std::string("</head>")
					+ std::string("<body>")
						+ std::string("<p>404 Not Found</p>")
						+ std::string("<p>- ftnix/1.0 -</p>")
					+ std::string("</body>")
				+ std::string("</html>");
	return (body);
}

Response &
Response::setBodyDocument(std::string body, std::string method)
{
	this->m_html_document = body;
	this->m_content_length = body.length();
	if (method == "HEAD")
		this->m_html_document = "";
	return (*this);
}

Response &
Response::setPublicFileDocument(std::string publicPath)
{
	std::string body;

	if (publicPath == "")
	{
		body = "";
		this->m_html_document = body;
		this->m_content_length = body.length();
		return (*this);
	}
	try
	{
		body = ft::publicFileToString(publicPath);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Err: setPublicFileDocument " << e.what() << '\n';
		body = "";
	}
	this->m_html_document = body;
	this->m_content_length = body.length();
	return (*this);
}

Response &
Response::setHtmlDocument()
{
	std::string body;

	if (this->m_status_code == 404)
	{
		body += set404HtmlDocument();
	}
	else
	{
		body += std::string("<!DOCTYPE html>")
					+ std::string("<html lang=\"en\">")
						+ std::string("<head>")
							+ this->m_head
						+ std::string("</head>")
						+ std::string("<body>")
							+ this->m_body
						+ std::string("</body>")
					+ std::string("</html>");
	}
	this->m_html_document = body;
	this->m_content_length = body.length();
	return (*this);
}

std::string
Response::httpResponseStartLine(std::string httpVersion, int statusCode)
{
	std::string startLine;

	startLine += httpVersion + " ";
	switch (statusCode)
	{
		case 100:
			startLine += std::to_string(statusCode) + std::string(" Continue\n");
			break;
		case 101:
			startLine += std::to_string(statusCode) + std::string(" Switching Protocols\n");
			break;
		case 200:
			startLine += std::to_string(statusCode) + std::string(" OK\n");
			break;
		case 201:
			startLine += std::to_string(statusCode) + std::string(" Created\n");
			break;
		case 202:
			startLine += std::to_string(statusCode) + std::string(" Accepted\n");
			break;
		case 203:
			startLine += std::to_string(statusCode) + std::string(" Non-Authoritative Information\n");
			break;
		case 204:
			startLine += std::to_string(statusCode) + std::string(" No Content\n");
			break;
		case 205:
			startLine += std::to_string(statusCode) + std::string(" Reset Content\n");
			break;
		case 206:
			startLine += std::to_string(statusCode) + std::string(" Partial Content\n");
			break;
		case 300:
			startLine += std::to_string(statusCode) + std::string(" Multiple Choices\n");
			break;
		case 301:
			startLine += std::to_string(statusCode) + std::string(" Moved Permanently\n");
			break;
		case 302:
			startLine += std::to_string(statusCode) + std::string(" Found\n");
			break;
		case 303:
			startLine += std::to_string(statusCode) + std::string(" See Other\n");
			break;
		case 304:
			startLine += std::to_string(statusCode) + std::string(" Not Modified\n");
			break;
		case 305:
			startLine += std::to_string(statusCode) + std::string(" Use Proxy\n");
			break;
		case 307:
			startLine += std::to_string(statusCode) + std::string(" Temporary Redirect\n");
			break;
		case 400:
			startLine += std::to_string(statusCode) + std::string(" Bad Request\n");
			break;
		case 401:
			startLine += std::to_string(statusCode) + std::string(" Unauthorized\n");
			break;
		case 402:
			startLine += std::to_string(statusCode) + std::string(" Payment Requied\n");
			break;
		case 403:
			startLine += std::to_string(statusCode) + std::string(" Forbidden\n");
			break;
		case 404:
			startLine += std::to_string(statusCode) + std::string(" Not Found\n");
			break;
		case 405:
			startLine += std::to_string(statusCode) + std::string(" Method Not Allowed\n");
			break;
		case 406:
			startLine += std::to_string(statusCode) + std::string(" Not Acceptable\n");
			break;
		case 407:
			startLine += std::to_string(statusCode) + std::string(" Proxy Authentication Required\n");
			break;
		case 408:
			startLine += std::to_string(statusCode) + std::string(" Request Timeout\n");
			break;
		case 409:
			startLine += std::to_string(statusCode) + std::string(" Conflict\n");
			break;
		case 410:
			startLine += std::to_string(statusCode) + std::string(" Gone\n");
			break;
		case 411:
			startLine += std::to_string(statusCode) + std::string(" Length Required\n");
			break;
		case 412:
			startLine += std::to_string(statusCode) + std::string(" Precondition Failed\n");
			break;
		case 413:
			startLine += std::to_string(statusCode) + std::string(" Payload Too Large\n");
			break;
		case 414:
			startLine += std::to_string(statusCode) + std::string(" URI Too Long\n");
			break;
		case 415:
			startLine += std::to_string(statusCode) + std::string(" Unsupported Media Type\n");
			break;
		case 416:
			startLine += std::to_string(statusCode) + std::string(" Range Not Satisfiable\n");
			break;
		case 417:
			startLine += std::to_string(statusCode) + std::string(" Expectation Failed\n");
			break;
		case 426:
			startLine += std::to_string(statusCode) + std::string(" Upgrade Required\n");
			break;
		case 500:
			startLine += std::to_string(statusCode) + std::string(" Internal Server Error\n");
			break;
		case 501:
			startLine += std::to_string(statusCode) + std::string(" Not Implemented\n");
			break;
		case 502:
			startLine += std::to_string(statusCode) + std::string(" Bad Gateway\n");
			break;
		case 503:
			startLine += std::to_string(statusCode) + std::string(" Service Unavailable\n");
			break;
		case 504:
			startLine += std::to_string(statusCode) + std::string(" Gateway Timeout\n");
			break;
		case 505:
			startLine += std::to_string(statusCode) + std::string(" HTTP Version Not Supported\n");
			break;
		default:
			perror("Undefined Status Code");
			break;
	}

	return (startLine);
}

std::string
Response::setCRLF()
{
	return (std::string("\r\n"));
}

std::string
Response::httpResponseHeader()
{
	std::string header;
	std::map<std::string, std::string>::iterator it;

	for (it = this->m_headers.begin(); it != this->m_headers.end(); ++it)
		header += std::string(it->first)
			+ ": "
			+ std::string(it->second)
			+ setCRLF();
	return (header);
}

Response &
Response::setHttpResponseHeader(std::string key, std::string value)
{
	this->m_headers.insert(std::make_pair(key, value));
	return (*this);
}

Response &
Response::setContentLanguage(std::string contentLanguage)
{
	this->m_content_language = contentLanguage;
	return (*this);
}

Response &
Response::setContentType(std::string contentType)
{
	this->m_content_type = contentType;
	return (*this);
}

Response &
Response::setServer(std::string server)
{
	this->m_server = server;
	return (*this);
}

Response &
Response::makeHttpResponseMessage()
{
	/* Concat HTTP Response  */
	this->m_response_message += httpResponseStartLine("HTTP/1.1", this->m_status_code)
		+ httpResponseHeader()
		+ setCRLF()
		+ this->m_html_document;

	/* Set Response Config */
	this->m_response_size = this->m_response_message.length();

	return (*this);
}

/*
**	gettimeofday()은 time(2)와 매우 비슷하지만 마이크로초 단위의 시간 까지 되돌려준다.
**	현재는 time(2)를 대신해서 쓰이고 있으며, 가능한 time(2)대신 이 함수를 사용하는 걸 권장한다.
**
**	#include <sys/time.h>
**	int gettimeofday(struct timeval *tv, struct timezone *tz);
**
**	<< tv >>
**	struct timeval
**	{
**		long tv_sec;		// 초
**		long tv_usec;		// 마이크로초
**	}
**
**	<< tz >>
**	struct timezone
**	{
**		int tz_minuteswest;	// 그리니치 서측분차
**		int tz_dsttime;		// DST 보정타입(일광 절약시간)
**	}
**	현재 timezone 구조체는 사용되지 않고 있으며, 앞으로도 지원되지 않을 것이다.
**	간혹 커널 소스등에서 이 필드가 사용되는 경우가 있는데, 모든 경우에 버그로 판단되어서 무시한다.
**	tz은 NULL을 사용하도록 한다.
**
**	return === 0: success
**	return === -1: fail
**
**	시간 구조체 함수 관계 참고: https://venture21.tistory.com/22
**
**	getDateTimestamp함수 Util.hpp 로 옮김.
*/

Response&
Response::setCurrentDate(int hour, int minute, int second)
{
	this->m_date = ft::getDateTimestamp(hour, minute, second);
	return (*this);
}
