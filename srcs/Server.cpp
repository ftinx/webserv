#include "Server.hpp"

/*
** TODO: utils 함수 적용하기
*/


Server::Server(){};
Server::~Server(){};

char *bin2hex(const unsigned char *input, size_t len)
{
    char *result;
    std::string hexits = "0123456789ABCDEF";

    if (input == NULL || len <= 0)
        return (NULL);

    // (2 hexits+space)/chr + NULL
    int resultlength = (len*3)+1;

    result = (char*)malloc(resultlength);
    bzero(result, resultlength);

    for (size_t i=0; i<len; i++)
    {
        result[i*3] = hexits[input[i] >> 4];
        result[(i*3)+1] = hexits[input[i] & 0x0F];
        result[(i*3)+2] = ' '; //for readability
    }

    return result;
}

/*
**	struct sockaddr_in {
**		short sin_family;			// 주소 체계: 항상 AF_INET
**		u_short sin_port;			// 16 비트 포트 번호 (0~65535), network byte order (Big Endian)
**		struct in_addr sin_addr;	// 32 비트 IP 주소
**		char sin_zero[8];			// 전체 크기를 16 비트로 맞추기 위한 dummy, 반드시 모두 0으로 채워져야 한다.
**	};
**
**	struct in_addr {
**		u_long s_addr; // 32비트 IP 주소를 저장 할 구조체, network byte order (Big Endian)
**	};
**	- - -
**	htonl(): long intger 데이터(일반적으로 4byte)를 network byte order로 변경
**	htons(): short intger 데이터(일반적으로 2byte)를 network byte order로 변경
**	ntohl(): long intger 데이터를 host byte order로 변경
**	ntohs(): short intger 데이터를 host byte order로 변경
*/
void
Server::setServerAddr(int port)
{
	memset((void *)&this->m_server_addr, 0x00, sizeof(this->m_server_addr));
	this->m_server_addr.sin_family = AF_INET;
	this->m_server_addr.sin_port = htons(port);
	this->m_server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
}

/*
**	socket (
**		AF_INET || AF_INET6 || AF_UNIX || AF_LOCAL || AF_LINK || AF_PACKET,
**		SOCK_STREAM(TCP) || SOCK_DGRAM(UDP) || SOCK_RAW,
**		0(Default Value) || IPPROTO_TCP(TCP) || IPPROTO_UDP(UDP)
**	)
** 	 - - -
**	int getsockopt(int  s, int level, int optname, void *optval, socklen_t *optlen);
**	int setsockopt(int s, int  level,  int  optname,  const  void  *optval, socklen_t optlen);
**
**	setsockopt(
**		socket,
**		SOL_SOCKET || IPPROTO_TCP,
**		SO_BROADCAST || SO_DEBUG || SO_DONTLINGER || SO_KEEPALIVE || SO_REUSEADDR (..etc),
**		socketlen
**	)
**
**	s : 소켓지정번호
**	level : 소켓의 레벨로 어떤 레벨의 소켓정보를 가져오거나 변경할 것인지를 명시한다. 보통 SOL_SOCKET와 IPPROTO_TCP 중 하나를 사용한다.
**	optname : 설정을 위한 소켓옵션의 번호
**	optval : 설정값을 저장하기 위한 버퍼의 포인터 (void * 로 넘기는 이유는 설정하고자 하는 소켓옵션에 따라서 다양한 크기의 데이터 형이 사용되기 떄문이다.)
**	optlen : optval 버퍼의 크기
**
**  SO_REUSEADDR의 상태를 TRUE(1)로 변경하게 되면 TIME_WAIT 상태에 있는 소켓에 할당된 IP주소와 포트를 새로 시작하는 소켓에 할당 해 줄 수 있게 된다.
**	On success, a file descriptor for the new socket is returned.
**	- - -
**	int bind (int sockfd, const struct sockaddr * addr, socklen_t addrlen);
**	int listen(int sockfd, int backlog);
*/
bool
Server::setServerSocket()
{
	int option;

	option = true;
	if ((this->m_server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
	{
		perror("socket error");
		return (false);
	}
    setsockopt(this->m_server_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(int));
	if (bind(
			this->m_server_socket,
			(struct sockaddr *)&this->m_server_addr,
			sizeof(this->m_server_addr)
		) == -1)
	{
		perror("bind error");
		return (false);
	}
	if (listen(this->m_server_socket, 10) == -1)
	{
		perror("listen error");
		return (false);
	}
	return (true);
}

/*
**	int new_socket = accept (int sockfd, struct sockaddr * addr, socklen_t * addrlen);
**	int select(int mafdl, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
**	- - -
**	struct timeval {
**		long tv_sec;  // 초
**		long tv_usec; // 마이크로 초
**	};
**
**	select (
**		maxfd(점검할 fd 의 개수),
**		*reafds(자료 읽기가 준비되었는지 확인할 fd 들),
**		*writefds(쓰기 연산을 끝낼 수 있는지 확인할 fd 들),
**		*exceptfds(예외가 발생했는지 확인할 fd 들),
**		NULL || 0 || int
**	)
**
**	maxfd에 1을 더하는 이유는 fd 번호가 0 부터 시작하기 때문이다.
**	#include<sys/select.h> 에 가장 큰 fd 번호로 FD_SETSIZE 가 정의되어 있다.
**	보통 1024가 정의되어 있는데 이 값은 너무 크므로 최대 fd 번호에 1을 더한 값을 넘겨주는 것이 좋다. (성능 저하)
**
**	timeout === NULL: 무한정 기다린다. fd 중 하나가 준비되거나 신호가 잡힐 대까지 차단된다.
**	timeout === 0: 전혀 기다리지 않는다. 차단 없이 fd 상태만 확인할 경우 쓰인다.
**	timeout !== 0: 지정된 sec 나 usec 만큼 기다린다. fd 중 하나가 준비되거나 시간이 만료되면 반환된다.
**
**	return > 0: 준비된 fd 개수
**	return === 0: 시간 만료, 이 경우 fd 세 집합 비트들은 모두 0이 되버린다.
**	return === -1: 오류, 이 경우 fd 세 집합의 비트들은 모두 수정되지 않는다.
**	- - -
**	FD_ZERO : 데이터가 변경된 파일의 개수 즉 fd_set에서 비트 값이 1인 필드의 개수를 반환
**	FD_SET : 집합의 특정 비트를 켤 때 사용
**	FD_CLR : 집합의 특정 비트를 끌 때 사용
**	FD_ISSET : 특정 비트가 켜져 있는지 확인 할 경우 사용
**	- - -
**	int accept(int s, struct sockaddr *addr, socklen_t *addrlen);
**
**	return > 0: 받아들인 소켓을 위한 파일지정번호 반환
**	return === -1: 오류
**	- - -
**	ssize_t read(int fd, void *buf, size_t nbytes);
**
**	return > 0: 수신한 바이트 수
**	return === -1: 오류
*/
void
Server::runServer()
{
	FD_ZERO(&this->readfds);
	FD_SET(this->m_server_socket, &this->readfds);

	this->maxfd = 0;
	this->maxfd = this->m_server_socket;
	memset(this->recvline, 0, MAXLINE);

	while(1)
	{
		this->allfds = this->readfds;
		printf("Select Wait %d\n", this->maxfd);
		this->fd_num = select(this->maxfd + 1 , &this->allfds, (fd_set *)0, (fd_set *)0, NULL);
		if (this->fd_num == -1)
		{
			perror("select error");
			return ;
		}
		else if (this->fd_num == 0)
		{
			return ;
			perror("time out");
		}

		if (FD_ISSET(this->m_server_socket, &this->allfds))
		{
			socklen_t addrlen;

			addrlen = sizeof(this->m_client_addr);
			this->m_client_socket = accept(
				this->m_server_socket,
				(struct sockaddr *)&this->m_client_addr,
				&addrlen
			);

			FD_SET(this->m_client_socket, &this->readfds);

			if (this->m_client_socket > this->maxfd)
				this->maxfd = this->m_client_socket;
			printf("Accept OK\n");
			continue;
		}

		for (int i = 0; i <= this->maxfd; i++)
		{
			this->sockfd = i;
			if (FD_ISSET(this->sockfd, &this->allfds))
			{
                while ((this->readn = read(this->sockfd, this->recvline, MAXLINE-1)) > 0)
                {
					/*
					**	Request parsing 부분 시작
					*/
                    fprintf(stdout, "\n%s\n\n%s", bin2hex(this->recvline, readn), this->recvline);

                    //hacky way to detect the end of the message.
                    if (this->recvline[readn-1] == '\n')
                    {
                        break;
                    }
                    memset(this->recvline, 0, MAXLINE);
					/*
					**	Request parsing 부분 끝
					*/
                }
				/*
				**	Response 부분 시작
				*/
				sendResponse(this->sockfd);
				/*
				**	Response 부분 끝
				*/
				close(this->sockfd);
				if (--this->fd_num <= 0)
					break;
			}
		}
	}

}

void
Server::closeServer()
{

}

/*
**	ssize_t write(int fd, const void *buf, size_t count);
**
**	return > 0: the number of bytes written is returned.
**	return === -1: error
*/
void 
Server::sendResponse(int clientfd)
{
	(void) clientfd;
	// uint8_t buff[MAXLINE+1];

	// snprintf((char *)buff, sizeof(buff), "HTTP/1.0 200 OK\r\n\r\nHello");
	// write(clientfd, buff, strlen((char *)buff));

	Response response = Response();
	response.setHtmlDocument();
	response.makeResponseMessage();
	// printf("%s", response.makeResponseMessage().c_str());
	write(this->sockfd, response.get_m_reponse_message().c_str(), response.get_m_response_size());
	return ;
}