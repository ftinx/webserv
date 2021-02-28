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
	this->m_status_description = "";
}

Response::Response(Response const &other)
{
	*this = other;
}

Response&
Response::operator=(Response const &rhs)
{
	(void) rhs;
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

std::string
Response::get_m_html_document()
{
	return (this->m_html_document);
}

std::string 
Response::get_m_reponse_message()
{
	return (this->m_response_message);
}

int 
Response::get_m_response_size()
{
	return (this->m_response_size);
}

std::string 
Response::get_m_body()
{
	return (this->m_body);
}

/*============================================================================*/
/********************************  Setter  ************************************/
/*============================================================================*/

/*============================================================================*/
/******************************  Exception  ***********************************/
/*============================================================================*/

/*============================================================================*/
/*********************************  Util  *************************************/
/*============================================================================*/

void
Response::setStatusCode(int statusCode)
{
	this->m_status_code = statusCode;
	return ;
}

void
Response::setStatusDescription(std::string statusDescription)
{
	this->m_status_description = statusDescription;
	return ;
}

void
Response::addHeaders(std::string key, std::string value)
{
	this->m_headers.insert(make_pair(key, value));
	return ;
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
Response::setAttribute(htmlTag tag, std::string value)
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

Response &	
Response::setHtmlDocument()
{
	std::string body;

	body += std::string("<!DOCTYPE html>")
				+ std::string("<html lang=\"en\">")
					+ std::string("<head>")
						+ this->m_head
					+ std::string("</head>")
					+ std::string("<body>")
						+ this->m_body
					+ std::string("</body>")
				+ std::string("</html>");

	this->m_html_document = body;
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

void
Response::makeResponseMessage()
{
	std::string httpResponse;
	int contentLength;
	
	contentLength = this->m_html_document.length();
	/* Concat start line (http version, status code) */
	httpResponse += httpResponseStartLine("HTTP/1.1", this->m_status_code);

	/* Concat Header */
	this->m_headers.insert(std::make_pair("date", "Sat, 27 Feb 2021 12:01:27 GMT"));
	this->m_headers.insert(std::make_pair("content-length", std::to_string(contentLength)));
	this->m_headers.insert(std::make_pair("content-language", std::to_string(this->m_status_code)));
	this->m_headers.insert(std::make_pair("content-type", "text/html; charset=UTF-8"));
	this->m_headers.insert(std::make_pair("status", "200"));
	this->m_headers.insert(std::make_pair("server", "ftnix"));

	std::map<std::string, std::string>::iterator it;

	for(it=m_headers.begin(); it!=m_headers.end(); ++it)
		httpResponse += std::string(it->first)
			+ ": "
			+ std::string(it->second)
			+ std::string("\n");

	httpResponse += setCRLF() + setCRLF();
	/* Concat Body */
	httpResponse += this->m_html_document;

	/* Concat Message */
	this->m_response_message = httpResponse;
	this->m_response_size = httpResponse.length();

	return ;
}