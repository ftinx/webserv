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

#define MAXLINE 1024 
#define SOCK_SETSIZE 1021
#define PORT 3601

/*
**  ** TODO
**
**  1. MAXLINE과 SOCK_SETSIZE size 설정 이유 찾기
**  2. perror() => message 함수로 교체하기
**  3. char *bin2hex 개조
**
**
**  ** Config file 에서 반영해야하는 것들
**
**  1. listen(this->m_server_socket, 5) 의 연결 대기열 개수 => 임의로 10개 지정
**  2. this->m_server_addr.sin_port = htons(PORT);의 PORT => 임의로 #define PORT 3601로 지정
*/

class Server
{
    private:
        /* Socket */
        struct sockaddr_in m_server_addr;
        struct sockaddr_in m_client_addr;
		// socklen_t addrlen;
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
        void setServerAddr(long host, int port);
        bool setServerSocket();
        void runServer();
        void closeServer();
};

#endif