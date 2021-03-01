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

    public:
    Uri();
    Uri(Uri const &other);
    Uri& operator=(Uri const &rhs);
    ~Uri();
    void setUri(std::string);
    std::string getUri() const;
    std::string getScheme() const;
    std::string getHost() const;
    std::string getPort() const;
    std::string getPath() const;
    std::map<std::string, std::string> getQuery() const;
    int parseUri();
    int parseHostPort(std::string);
    int parsePath(std::string);
    int parseQuery(std::string);
};

#endif