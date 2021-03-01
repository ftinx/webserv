#include "Request.hpp"
#include "Utils.hpp"

/*============================================================================*/
/******************************  Constructor  *********************************/
/*============================================================================*/

Request::Request()
: m_http_version(""), m_cgi_version(""), m_check_cgi(false),
m_method(""), m_body(""), m_error_code(0)
{
}

/*============================================================================*/
/******************************  Destructor  **********************************/
/*============================================================================*/

Request::~Request(){}


/*============================================================================*/
/********************************  Getter  ************************************/
/*============================================================================*/

std::string
Request::getHttpVersion() const
{
    return (this->m_http_version);
}

bool
Request::getCheckCgi() const
{
    return (this->m_check_cgi);
}

std::string
Request::getMethod() const
{
    return (this->m_method);
}

Uri
Request::getUri() const
{
    return (this->m_uri);
}

void
Request::printHeaders() const
{
    std::map<std::string, std::string> m;
    std::map<std::string, std::string>::iterator i;
    std::vector<std::string> ret;

    for (i = m.begin(); i != m.end(); i++)
    {
        std::cout << i->first << ": " << i->second << std::endl;
    }
}

std::string
Request::getBody() const
{
    return (this->m_body);
}

int
Request::getErrorCode() const
{
    return (this->m_error_code);
}

/*============================================================================*/
/********************************  Setter  ************************************/
/*============================================================================*/

void
Request::setHttpVersion(std::string http_version)
{
    this->m_http_version = http_version;
}

void
Request::setCgiVersion(std::string cgi_version)
{
    this->m_cgi_version = cgi_version;
}

void
Request::setCheckCgi(bool flag)
{
    this->m_check_cgi = flag;
}

void
Request::setMethod(std::string method)
{
    this->m_method = method;
}

void
Request::setErrorCode(int error_code)
{
    this->m_error_code = error_code;
}

/*============================================================================*/
/*********************************  Util  *************************************/
/*============================================================================*/

bool
Request::parseMessage(std::string message)
{
    size_t i;
    std::vector<std::string> lines = ft::split(message, '\n');

    if (parseRequestLine(lines[0]) == false)
        return (false);
    for (i = 1; i < lines.size(); i++)
    {
        if (checkBlankLine(lines[i]))
            break;
        if (parseHeader(ft::rtrim(lines[i], "\r\n")) == false)
            return (false);
    }
    if (i < lines.size())
    {
        while (i < lines.size())
        {
            if (parseBody(lines[i]) == false)
                return (false);
            i++;
        }
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
    this->m_method = pieces[0];
    this->m_uri.setUri(pieces[1]);
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
    std::vector<std::string> key_value = ft::split(line, ":");
    
    if (key_value.size() != 2)
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
    this->m_headers.insert(make_pair(key_value[0], ft::trim(key_value[1], " ")));
    return (true);
}

bool
Request::parseBody(std::string line)
{
    std::string newline; 

    newline = line + "\n";
    this->m_body += newline;
    return (true);
}

bool
Request::checkMethod()
{
    if (this->m_method.compare("GET") ||
    this->m_method.compare("HEAD") ||
    this->m_method.compare("POST") ||
    this->m_method.compare("PUT") ||
    this->m_method.compare("DELETE") ||
    this->m_method.compare("OPTIONS") ||
    this->m_method.compare("TRACE"))
        return (true);
    return (false);
}

void
Request::checkCGI()
{
    int p;

    p = this->m_uri.getPath().find("cgi-bin/");
    if (p == 0)
        this->m_check_cgi = true;
    else
        this->m_check_cgi = false;
}

bool
Request::checkBlankLine(std::string str)
{
    if (str.empty())
    {
        return (true);
    }
    return (false);
}

std::ostream& operator<<(std::ostream &os, Request const& ref)
{
    Uri uri;

    uri = ref.getUri();
    return (os << "error code: " << ref.getErrorCode() << std::endl
    << "http version: "<< ref.getHttpVersion() << std::endl
    << "method: " << ref.getMethod() << std::endl
    << "uri: " << uri.getUri() << std::endl
    << "> scheme: " << uri.getScheme() << std::endl
    << "> host: " << uri.getHost() << std::endl
    << "> port: " << uri.getPort() << std::endl
    << "> path: " << uri.getPath() << std::endl
    << "body: " << ref.getBody() << std::endl);
}
