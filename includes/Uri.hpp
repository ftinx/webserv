#ifndef URI_HPP
#define URI_HPP

/*
** rfc 7230 2.7
*/

#include <string>
#include <vector>
#include <map>


class Uri
{
    private:
    std::string m_uri;
    std::string m_scheme;
    std::string m_host;
    std::string m_port;
    std::string m_path;
    std::map<std::string, std::string> m_query;

    private:
    Uri& operator=(Uri const &rhs);
    Uri(Uri const &other);

    public:
    Uri();
    ~Uri();
    void setUri(std::string);
    std::string getUri();
    std::string getScheme();
    std::string getHost();
    std::string getPort();
    std::string getPath();
    std::map<std::string, std::string> getQuery();
    int parseUri();
    int parseHostPort(std::string);
    int parsePath(std::string);
    int parseQuery(std::string);
};

#endif