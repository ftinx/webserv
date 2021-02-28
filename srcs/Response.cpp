#include "Response.hpp"

/*============================================================================*/
/****************************  Static variables  ******************************/
/*============================================================================*/

/*============================================================================*/
/******************************  Constructor  *********************************/
/*============================================================================*/

Response::Response()
{
	this->m_status_code = 200;
	this->m_status_description = "모두모두 파이팅입니다!! ㅎㅎ 항상 감사합니댜!";
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

void	Response::setHtmlDocument()
{
	std::string body;

	body += std::string("<!DOCTYPE html>")
				+ std::string("<html lang=\"en\">")
					+ std::string("<head>")
						+ std::string("<title> Webserv </title>")
					+ std::string("</head>")
					+ std::string("<body>")
						+ std::string("<p> Webserv by ftinx </p>")
						+ std::string("<div>") + this->m_status_description + std::string("</div>")
					+ std::string("</body>")
				+ std::string("</html>");

	this->m_html_document = body;
	return ;
}

#include <iostream>
#include <algorithm>

void
Response::makeResponseMessage()
{
	std::string httpResponse;
	/* Concat status code */
	httpResponse += std::string("HTTP/1.1 200 OK\n");

	/* Concat Header */
	this->m_headers.insert(std::make_pair("date", "Sat, 27 Feb 2021 12:01:27 GMT"));
	this->m_headers.insert(std::make_pair("content-length", "139"));
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

	httpResponse += std::string("\r\n\r\n");
	/* Concat Body */
	httpResponse += this->m_html_document;

	/* Concat Message */
	this->m_response_message = httpResponse;
	this->m_response_size = httpResponse.length();

	return ;
}