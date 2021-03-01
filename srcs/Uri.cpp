#include "Uri.hpp"
#include "Utils.hpp"

/*============================================================================*/
/******************************  Constructor  *********************************/
/*============================================================================*/

Uri::Uri()
: m_uri(""), m_scheme(""), m_host(""), m_port("80"), m_path("")
{}

Uri::Uri(Uri const &other)
{
    *this = other;
}

Uri& Uri::operator=(Uri const &rhs)
{
    if (this == &rhs)
        return (*this);
    this->m_uri = rhs.get_m_uri();
    this->m_scheme = rhs.get_m_scheme();
    this->m_host = rhs.get_m_host();
    this->m_port = rhs.get_m_port();
    this->m_path = rhs.get_m_path();
    this->m_query = rhs.get_m_query();
    return (*this);
}


/*============================================================================*/
/******************************  Destructor  **********************************/
/*============================================================================*/

Uri::~Uri()
{}

/*============================================================================*/
/********************************  Setter  ************************************/
/*============================================================================*/

void
Uri::set_m_uri(std::string uri)
{
    this->m_uri = uri;
}

/*============================================================================*/
/********************************  Getter  ************************************/
/*============================================================================*/

std::string
Uri::get_m_uri() const
{
    return (this->m_uri);
}

std::string
Uri::get_m_scheme() const
{
    return (this->m_scheme);
}

std::string
Uri::get_m_host() const
{
    return (this->m_host);
}

std::string
Uri::get_m_port() const
{
    return (this->m_port);
}

std::string
Uri::get_m_path() const
{
    return (this->m_path);
}

std::map<std::string, std::string>
Uri::get_m_query() const
{
    return (this->m_query);
}

/*============================================================================*/
/*********************************  Util  *************************************/
/*============================================================================*/

int
Uri::parseUri()
{
    std::vector<std::string> pieces = ft::split(this->m_uri, "://");
    
    if (pieces.size() < 2 || pieces.size() > 2)
        return (400);
    this->m_scheme = pieces[0];
    return (this->parseHostPort(pieces[1]));
}

int
Uri::parseHostPort(std::string str)
{
    size_t colon_pos = str.find_first_of(':');
    size_t slash_pos = str.find_first_of('/');

    if (colon_pos == std::string::npos)
    {
        this->m_host = str.substr(0, slash_pos);
    }
    else
    {
        this->m_host = str.substr(0, colon_pos);
        this->m_port = str.substr(colon_pos + 1, slash_pos - colon_pos - 1);
    }
    if (slash_pos == std::string::npos)
        return (0);
    return (this->parsePath(str.substr(slash_pos + 1, std::string::npos)));
}

int
Uri::parsePath(std::string str)
{
    size_t question_pos = str.find_first_of('?');

    this->m_path = str.substr(0, question_pos);
    if (question_pos == std::string::npos)
        return (0);
    return (this->parseQuery(str.substr(question_pos + 1, std::string::npos)));
}

int
Uri::parseQuery(std::string str)
{
    std::vector<std::string> pieces = ft::split(str, "&");

    for (size_t i = 0; i < pieces.size(); i++)
    {
        std::vector<std::string> queries = ft::split(pieces[i], "=");
        if (queries.size() != 2)
            return (400);
        this->m_query.insert(make_pair(queries[0], queries[1]));
    }
    return (0);
}