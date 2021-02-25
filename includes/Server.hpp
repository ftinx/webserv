#ifndef SERVER_HPP
# define SERVER_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>

#include <unistd.h>
#include <string>

#include <vector>
#include <map>

// #include "Response.hpp"
// #include "Request.hpp"
// #include "utils.hpp"

/* MAXLINE과 SOCK_SETSIZE size 설정 이유 찾기 */
#define MAXLINE 1024 
#define SOCK_SETSIZE 1021
/* config 파일에서 지정될 변수들 추후 삭제 예정 */
#define PORT 3601

class Server
{
    private:
        /* Socket */
        struct sockaddr_in m_server_addr;
        struct sockaddr_in m_client_addr;
		socklen_t addrlen;
        int m_server_socket;
        int m_client_socket;
		int fd_num;
		int sockfd;
		int readn;
		int maxfd;
		uint8_t buff[MAXLINE+1];
		uint8_t recvline[MAXLINE+1];
    	fd_set readfds, allfds;

        /* Request, Response */
        // std::vector<Request> m_request;
        // std::vector<Response> m_response;
        // std::map<std::string, std::string> m_chunked_message;

    /* 우선 실험을 위해 private에서 public으로 변경 */
    public:
        Server(Server const &other);
        Server& operator=(Server const &ref);

    public:
        Server();
        ~Server();
        void setServerAddr();
        bool setServerSocket();
        void runServer();
        void closeServer();
};

#endif