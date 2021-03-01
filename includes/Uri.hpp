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
    void set_m_uri(std::string);
    std::string get_m_uri() const;
    std::string get_m_scheme() const;
    std::string get_m_host() const;
    std::string get_m_port() const;
    std::string get_m_path() const;
    std::map<std::string, std::string> get_m_query() const;
    int parseUri();
    int parseHostPort(std::string);
    int parsePath(std::string);
    int parseQuery(std::string);
};

#endif