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
    bool m_check_abs_uri;
    std::string m_scheme;
    std::string m_host;
    std::string m_port;
    std::string m_path;
    std::map<std::string, std::string> m_query;
    std::string m_query_string;

    public:
    Uri();
    Uri(Uri const &other);
    Uri& operator=(Uri const &rhs);
    ~Uri();
    void set_m_uri(std::string);
    void set_m_host(std::string);
    void set_m_port(std::string);
    void set_m_path(std::string);
    std::string get_m_uri() const;
    bool get_m_check_abs_uri() const;
    std::string get_m_scheme() const;
    std::string get_m_host() const;
    std::string get_m_port() const;
    std::string get_m_path() const;
    std::map<std::string, std::string> get_m_query() const;
    std::string get_m_query_string() const;
    int parseUri();
    int parseHostPort(std::string);
    int parsePath(std::string);
    int parseQuery(std::string);
};

#endif
