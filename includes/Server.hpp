#ifndef SERVER_HPP
# define SERVER_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <map>
#include "Response.hpp"
#include "Request.hpp"

class Server
{
    private:
        struct sockaddr_in m_server_addr;
        int m_server_socket;
        std::vector<Request> m_request;
        std::vector<Response> m_response;
        std::map<std::string, std::string> m_chunked_message;

    private:
        Server(Server const &other);
        Server& operator=(Server const &ref);

    public:
        Server();
        ~Server();
        void setServerAddr(long host, int port);
        void setServerSocket();
        void runServer();
        void closeServer();
};

#endif