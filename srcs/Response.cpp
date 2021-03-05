#include "Response.hpp"

/*============================================================================*/
/****************************  Static variables  ******************************/
/*============================================================================*/

/*============================================================================*/
/******************************  Constructor  *********************************/
/*============================================================================*/

Response::Response()
{
	this->m_body = "";
	this->m_head = "";
	this->m_status_code = 200;
	this->m_content_language = "ko, en";
	this->m_content_type = "text/html; charset=UTF-8";
	this->m_server = "ftnix/1.0 (MacOS)";
	this->m_status_description = "";
	this->m_content_length = 0;
}

Response::Response(Response const &other)
{
	*this = other;
}

Response &
Response::operator=(Response const &rhs)
{
	m_status_code = rhs.m_status_code;
	m_date = rhs.m_date;
	m_content_language = rhs.m_content_language;
	m_content_type = rhs.m_content_type;
	m_server = rhs.m_server;
	m_status_description = rhs.m_status_description;

	/* HTML document */
	m_headers = rhs.m_headers;
	m_html_document = rhs.m_html_document;
	m_body = rhs.m_body;
	m_head = rhs.m_head;
	m_content_length = rhs.m_content_length;

	/* response message */
	m_response_message = rhs.m_response_message;
	m_response_size = rhs.m_response_size;
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
		case 200:
			startLine += std::to_string(statusCode) + std::string(" OK\n");
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
		case 501:
			startLine += std::to_string(statusCode) + std::string(" Not Implemented\n");
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
			+ std::string("\n");
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
		+ setCRLF() + setCRLF()
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
*/

std::string
Response::getDate()
{
	struct timeval currentTime;
	struct tm *tm;
	char buf[64];

	gettimeofday(&currentTime, NULL);
	tm = localtime(&currentTime.tv_sec);
	strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", tm);
	free(tm);
	return (buf);
}

Response&
Response::setCurrentDate()
{
	this->m_date = getDate();
	return (*this);
}
