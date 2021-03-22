#include "Request.hpp"
#include "Utils.hpp"
#include <string>

/*============================================================================*/
/******************************  Constructor  *********************************/
/*============================================================================*/

Request::Request()
: m_message(""), m_http_version(""), m_cgi_version(""), m_check_cgi(false),
m_method(DEFAULT), m_uri(), m_headers(), m_body(""), m_error_code(0)
{
}

Request::Request(Request const &other)
{
	*this = other;
}

Request& Request::operator=(Request const &rhs)
{
	if (this == &rhs)
		return (*this);
	this->m_message = rhs.get_m_message();
	this->m_http_version = rhs.get_m_http_version();
	this->m_cgi_version = rhs.get_m_cgi_version();
	this->m_check_cgi = rhs.get_m_check_cgi();
	this->m_method = rhs.get_m_method();
	this->m_uri = rhs.get_m_uri();
	this->m_headers = rhs.get_m_headers();
	this->m_body = rhs.get_m_body();
	this->m_error_code = rhs.get_m_error_code();
	return (*this);
}



/*============================================================================*/
/******************************  Destructor  **********************************/
/*============================================================================*/

Request::~Request(){}


/*============================================================================*/
/********************************  Getter  ************************************/
/*============================================================================*/

std::string
Request::get_m_message() const
{
	return (this->m_message);
}


std::string
Request::get_m_http_version() const
{
	return (this->m_http_version);
}

std::string
Request::get_m_cgi_version() const
{
	return (this->m_cgi_version);
}


bool
Request::get_m_check_cgi() const
{
	return (this->m_check_cgi);
}

Method
Request::get_m_method() const
{
	return (this->m_method);
}

Uri
Request::get_m_uri() const
{
	return (this->m_uri);
}

std::map<std::string, std::string>
Request::get_m_headers() const
{
	return (this->m_headers);
}


std::string
Request::get_m_body() const
{
	return (this->m_body);
}

int
Request::get_m_error_code() const
{
	return (this->m_error_code);
}

/*============================================================================*/
/********************************  Setter  ************************************/
/*============================================================================*/

void
Request::set_m_http_version(std::string http_version)
{
	this->m_http_version = http_version;
}

void
Request::set_m_cgi_version(std::string cgi_version)
{
	this->m_cgi_version = cgi_version;
}

void
Request::set_m_check_cgi(bool flag)
{
	this->m_check_cgi = flag;
}

void
Request::set_m_method(Method method)
{
	this->m_method = method;
}

void
Request::set_m_error_code(int error_code)
{
	this->m_error_code = error_code;
}

/*============================================================================*/
/*********************************  Util  *************************************/
/*============================================================================*/

std::string
Request::getMethod()
{
	if (this->m_method == GET)
		return ("GET");
	else if (this->m_method == HEAD)
		return ("HEAD");
	else if (this->m_method == POST)
		return ("POST");
	else if (this->m_method == PUT)
		return ("PUT");
	else if (this->m_method == DELETE)
		return ("DELETE");
	else if (this->m_method == OPTIONS)
		return ("OPTIONS");
	else if (this->m_method == TRACE)
		return ("TRACE");
	return ("DEFAULT");
}

std::string
Request::getContentLength()
{
	std::map<std::string, std::string>::const_iterator it;

	it = this->m_headers.find("content-length");
	if (it == this->m_headers.end())
		return ("");
	return (it->second);
}

std::string
Request::getContentType()
{
	std::map<std::string, std::string>::const_iterator it;

	it = this->m_headers.find("content-type");
	if (it == this->m_headers.end())
		return ("text/html");
	return (it->second);
}

bool
Request::isBreakCondition(std::string str, bool *chunked, int body_bytes, int header_bytes)
{
	static int content_length = -1;
	size_t pos;
	std::string tmp;

	if (*chunked == true && str == "\r\n") //error의 소지가 있음
		return (true);
	if ((pos = this->m_message.find("content-length:")) != std::string::npos)
	{
		tmp = m_message.substr(pos + strlen("content-length:"), std::string::npos);
		if ((pos = tmp.find_first_of("\n")) != std::string::npos)
		{
			tmp = ft::trim(tmp.substr(0, pos), " \r");
			content_length = std::stoi(tmp);
		}
	}
	if (content_length >= 0 && content_length <= body_bytes)
	{
		this->m_message = this->m_message.substr(0, header_bytes + content_length);
		return (true);
	}
	else if ((pos = this->m_message.find("\r\n\r\n")) != std::string::npos)
	{
		this->m_message = this->m_message.substr(0, pos);
		return (true);
	}
	//td::cout << "isBreakCondition False" << std::endl;
	return (false);
}

bool
Request::getMessage(int fd)
{
	bool found_break_line = false;
	bool chunked = false;
	int ret;
	int header_bytes = 0;
	int body_bytes = 0;
	char recvline[MAXLINE + 1];


	while ((ret = read(fd, recvline, MAXLINE - 1)) > 0)
	{
		recvline[ret] = '\0';
		std::string str(recvline);

		this->m_message.append(str);
		if (this->m_message.find("\r\n\r\n") >= 0)
		{
			if (found_break_line == false)
			{
				found_break_line = true;
				body_bytes = this->m_message.size() - (this->m_message.find("\r\n\r\n") + 3);
				header_bytes = this->m_message.find("\r\n\r\n") + 4;
			}
			else
				body_bytes += ret;
		}
		if (isBreakCondition(str, &chunked, body_bytes, header_bytes))
			break;
		memset(recvline, 0, MAXLINE);
	}
	std::cout << "\033[43;31m**** request m_message out of loop *****\033[0m" << std::endl;
	std::cout << m_message << std::endl;
	if (this->parseMessage() == false)
		return (false);
	return (true);
}

bool
Request::parseMessage()
{
	size_t i;
	std::vector<std::string> lines = ft::split(this->m_message, "\n");

	if (lines.size() == 0)
		return (false);
	if (parseRequestLine(ft::rtrim(lines[0], "\r")) == false)
	{
		return (false);
	}
	for (i = 1; i < lines.size(); i++)
	{
		if (checkBlankLine(ft::rtrim(lines[i], "\r")))
			break;
		if (parseHeader(ft::rtrim(lines[i], "\r")) == false)
			return (false);
	}
	i++;
	while (i < lines.size())
	{
		if (parseBody(lines[i], i, lines.size()) == false)
			return (false);
		i++;
	}
	return (true);
}

bool
Request::parseRequestLine(std::string request_line)
{
	int error_code;
	std::vector<std::string> pieces = ft::split(request_line, ' ');

	if (pieces.size() != 3)
	{
		this->m_error_code = 400;
		return (false);
	}
	this->m_method = ft::getMethodType(pieces[0]);
	this->m_uri.set_m_uri(pieces[1]);
	this->m_http_version = pieces[2];

	if (checkMethod() == false)
	{
		this->m_error_code = 501;
		return (false);
	}
	if ((error_code = m_uri.parseUri()))
	{
		this->m_error_code = error_code;
		return(false);
	}
	checkCGI();
	return (true);
}

bool
Request::parseHeader(std::string line)
{
	std::vector<std::string> key_value = ft::split(line, ':');

	if (key_value.size() < 2)
	{
		this->m_error_code = 400;
		return (false);
	}

	int key_len = key_value[0].length();
	if (key_len == 0 || key_value[0][key_len - 1] == ' ')
	{
		this->m_error_code = 400;
		return (false);
	}
	if (key_value[0] == "Host")
		this->m_uri.set_m_host(ft::trim(key_value[1], " "));
	else if (key_value[0] == "Port")
		this->m_uri.set_m_port(ft::trim(key_value[1], " "));
	else
		this->m_headers.insert(make_pair(key_value[0], ft::trim(key_value[1], " ")));
	return (true);
}

bool
Request::parseBody(std::string line, int i, int size)
{
	std::string newline;

	newline = line;
	if (i != size - 1)
		newline += "\n";
	this->m_body += newline;
	return (true);
}

bool
Request::checkMethod()
{
	if (this->m_method == DEFAULT)
		return (false);
	return (true);
}

bool
Request::checkCGI()
{
	size_t pos;
	std::string path = std::string(".") + this->m_uri.get_m_path();

	pos = path.find("cgi-bin");
	if (pos != 0)
		return (this->m_check_cgi = false);
	if (ft::isValidFilePath(path))
		return (this->m_check_cgi = true);
	else
		return (this->m_check_cgi = false);
}

bool
Request::checkBlankLine(std::string str)
{
	if (str[0] == '\r')
		return (true);
	return (false);
}

void
Request::printHeaders() const
{
	std::map<std::string, std::string> m = this->m_headers;
	std::map<std::string, std::string>::iterator i;
	std::vector<std::string> ret;

	std::cout << "#### print headers #### " << std::endl;
	for (i = m.begin(); i != m.end(); i++)
	{
		std::cout << i->first << ": " << i->second << std::endl;
	}
}

std::ostream& operator<<(std::ostream &os, Request const& ref)
{
	Uri uri;

	uri = ref.get_m_uri();
	return (os << "error code: " << ref.get_m_error_code() << std::endl
	<< "http version: "<< ref.get_m_http_version() << std::endl
	<< "method: " << ref.get_m_method() << std::endl
	<< "uri: " << uri.get_m_uri() << std::endl
	<< "> scheme: " << uri.get_m_scheme() << std::endl
	<< "> host: " << uri.get_m_host() << std::endl
	<< "> port: " << uri.get_m_port() << std::endl
	<< "> path: " << uri.get_m_path() << std::endl
	<< "> body: " << ref.get_m_body() << std::endl);
}
