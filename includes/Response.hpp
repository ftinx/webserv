#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <string>
#include <map>

class Response
{
    private:
        int m_status_code;
        std::string m_status_description;
        std::map<std::string, std::string> m_headers;
        std::string m_body;

    private:
        Response(Response const &other);
        Response& operator=(Response const &rhs);
    
    public: 
        Response();
        ~Response();

        bool    parseCgiResponse(std::string&);
        void    setStatusCode(int);
        void    setStatusDescription(std::string);
        void    addHeaders(std::string, std::string);
        void    setBody(std::string);
        char*   makeResponseMessage();
};

#endif