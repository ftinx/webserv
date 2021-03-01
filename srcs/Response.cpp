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
	if (this == &rhs)
        return (*this);
    *this = Response(rhs);
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
Response::get_m_status_code()
{
	return (this->m_status_code);
}

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

int
Response::get_m_content_length()
{
	return (this->m_content_length);
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
