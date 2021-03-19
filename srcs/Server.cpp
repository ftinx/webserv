#include "Server.hpp"

/*============================================================================*/
/****************************  Static variables  ******************************/
/*============================================================================*/

/*============================================================================*/
/******************************  Constructor  *********************************/
/*============================================================================*/

Server::Server(){};

Server::Server(Server const &other)
{
	*this = other;
};

Server& Server::operator=(Server const &rhs)
{
	m_server_block = rhs.m_server_block;
	m_server_name = rhs.m_server_name;
	m_port = rhs.m_port;
	m_err_page_path = rhs.m_err_page_path;
	m_content_length = rhs.m_content_length;
	m_location_size = rhs.m_location_size;
	m_server_addr = rhs.m_server_addr;
	m_client_addr = rhs.m_client_addr;
	m_server_socket = rhs.m_server_socket;
	m_client_socket = rhs.m_client_socket;
	fd_num = rhs.fd_num;
	sockfd = rhs.sockfd;
	readn = rhs.readn;
	maxfd = rhs.maxfd;
	m_main_fds = rhs.m_main_fds;
	m_copy_fds = rhs.m_copy_fds;
	m_requests = rhs.m_requests;
	m_responses = rhs.m_responses;
	return (*this);
};

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

/*============================================================================*/
/******************************  Destructor  **********************************/
/*============================================================================*/

Server::~Server(){};

/*============================================================================*/
/*******************************  Overload  ***********************************/
/*============================================================================*/

/*============================================================================*/
/********************************  Getter  ************************************/
/*============================================================================*/

std::string
Server::get_m_server_name()
{
	return (this->m_server_name);
}

int
Server::get_m_port()
{
	return (this->m_port);
}

/*============================================================================*/
/********************************  Setter  ************************************/
/*============================================================================*/

/*============================================================================*/
/*****************************  Server Util  **********************************/
/*============================================================================*/

void
Server::noteCGILocation()
{
	Response response;
	std::vector<HttpConfigLocation> location_block = this->m_server_block.get_m_location_block();
	std::vector<HttpConfigLocation>::const_iterator i = location_block.begin();
	int j = 0;

	while (i != location_block.end())
	{
		printf("location %d\n", j++);
		printf("path:: %s\n", i->get_m_path().c_str());
		printf("root:: %s\n", i->get_m_root().c_str());
		printf("cgi::path %d\n", i->get_m_cgi_path() == "");
		{
			std::vector<Method> limit_except = i->get_m_limit_except();
			std::vector<Method>::const_iterator limit = limit_except.begin();
			while (limit != limit_except.end())
			{
				printf("%d", *limit);
				// std::cout << ' ' << *limit << std::endl;
				switch (*limit)
				{
					case HEAD:
					case GET:
					case POST:
					case PUT:
					case DELETE:
					case OPTIONS:
					case TRACE:
					default:
						break;
				}
				limit++;
			}
			printf("\n");
			// std::vector<std::string> index = i->get_m_index();
			// std::vector<std::string> get_m_cgi();
		}
		i++;
	}
	return ;
}

void
Server::init(HttpConfigServer server_block, std::string server_name, int port, std::string err_page_path, int content_length, size_t location_size)
{
	this->m_requests = std::vector<Request>(MAX_SOCK_NUM);
	this->m_responses = std::vector<Response>(MAX_SOCK_NUM);
	this->m_server_block = server_block;
	this->m_server_name = server_name;
	this->m_port = port;
	if (err_page_path != "")
		this->m_err_page_path = err_page_path.substr(6, err_page_path.size()-6);
	else
		this->m_err_page_path = "errors/default_error.html";
	this->m_content_length = content_length;
	this->m_location_size = location_size;
	return ;
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
			reinterpret_cast<struct sockaddr *>(&this->m_server_addr),
			static_cast<socklen_t>(sizeof(this->m_server_addr))
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
**	len 바이트 만큼 읽으라고 요청하였지만 읽은 데이터가 없다면, read() 함수는 읽은 바이트가 생길 때 까지 블록된다.
**
**	return > 0: 수신한 바이트 수
**	return === 0: EOF
**	return === -1: 오류
*/
#include <bitset>
#include <iostream>

// void
// serverFDLog(int num)
// {
// 	for (int i=0; i<1; i++) {
// 		std::cout << "-----" << num << "-----" << std::endl;
// 		std::cout << std::bitset<32>(this->m_copy_fds.fds_bits[i]) << std::endl;
// 	}
// 	return ;
// }

void
Server::runServer()
{
	// rfc보고 정의
	struct timeval timeout;
	timeout.tv_sec = 4;
	timeout.tv_usec = 2;

	FD_ZERO(&this->m_main_fds);
	FD_SET(this->m_server_socket, &this->m_main_fds);

	this->maxfd = 0;
	this->maxfd = this->m_server_socket;

	while (42)
	{
		this->m_copy_fds = this->m_main_fds;
		printf("Select Wait %d\n", this->maxfd);
		this->fd_num = select(
			this->maxfd + 1 ,
			&this->m_copy_fds,
			reinterpret_cast<fd_set *>(0),
			reinterpret_cast<fd_set *>(0),
			&timeout
		);

		switch (this->fd_num)
		{
			case -1:
				perror("select error");
				return ;
			case 0:
				perror("timeout error");
				return ;
			default:
				getRequest();
		}
	}
	return ;
}

void
Server::closeServer()
{
	return ;
}

/*============================================================================*/
/*******************************  Request  ************************************/
/*============================================================================*/

#include <bitset>
#include <iostream>

void
Server::getRequest()
{
	if (ft::fdIsSet(this->m_server_socket, &this->m_copy_fds))
	{
		socklen_t addrlen;

		addrlen = sizeof(this->m_client_addr);
		if ((
			this->m_client_socket = accept(
				this->m_server_socket,
				reinterpret_cast<struct sockaddr *>(&this->m_client_addr),
				reinterpret_cast<socklen_t *>(&addrlen)
			)
		) == -1)
			std::cerr<<"accept error"<<std::endl;

		ft::fdSet(this->m_client_socket, &this->m_main_fds);

		if (this->m_client_socket > this->maxfd)
			this->maxfd = this->m_client_socket;
		printf("Accept OK\n");
		return ;
	}

	for (int i = 0; i <= this->maxfd; i++)
	{
		this->sockfd = i;
		if (ft::fdIsSet(this->sockfd, &this->m_copy_fds))
		{
			/*
			** Request 부분 시작, false시 에러 받아줘야
			*/
			this->m_requests[this->sockfd].getMessage(this->sockfd);
			std::cout << this->m_requests[this->sockfd] << std::endl;
            this->m_requests[this->sockfd].printHeaders();
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
	// FD_ZERO(&this->m_main_fds);
	return ;
}

/*============================================================================*/
/*******************************  Response  ***********************************/
/*============================================================================*/

/*============================================================================*/
/*********************************  HEAD  *************************************/
/*============================================================================*/

Response
Server::methodHEAD(int clientfd)
{
	Response response;
	clientfd=0;
	return (response);
}

/*============================================================================*/
/**********************************  GET  *************************************/
/*============================================================================*/

Response
Server::methodGET(int clientfd)
{
	Response response;
	std::string path = this->m_requests[clientfd].get_m_uri().get_m_path();

	if (ft::isValidFilePath(path) == false)
		return (Server::page404("errors/default_error.html"));
	else
	{
		return (
			response
				.setPublicFileDocument(path)
				.setHttpResponseHeader("date", response.get_m_date())
				.setHttpResponseHeader("content-length", std::to_string(response.get_m_content_length()))
				.setHttpResponseHeader("content-language", response.get_m_content_language())
				.setHttpResponseHeader("content-type", response.get_m_content_type())
				.setHttpResponseHeader("status", std::to_string(response.get_m_status_code()))
				.setHttpResponseHeader("server", response.get_m_server())
				.makeHttpResponseMessage()
		);
}
}

/*============================================================================*/
/**********************************  POST  ************************************/
/*============================================================================*/

std::map<std::string, std::string>
Server::parseQuery(std::string str)
{
	std::map<std::string, std::string> m_query;
	std::vector<std::string> pieces = ft::split(str, "&");

	for (size_t i = 0; i < pieces.size(); i++)
	{
		std::vector<std::string> queries = ft::split(pieces[i], "=");
		m_query.insert(make_pair(ft::trim(queries[0], " \n\t\v\f\r"), ft::trim(queries[1], " \n\t\v\f\r")));
	}
	return (m_query);
}

Response
Server::postLoginSuccess()
{
	Response response = Response();

	return (
		response
			.setStatusCode(200)
			.setCurrentDate()
			.setContentLanguage("ko, en")
			.setContentType("text/html; charset=UTF-8")
			.setServer("ftnix/1.0 (MacOS)")
			.setPublicFileDocument("srcs/login.html")
			.setHttpResponseHeader("date", response.get_m_date())
			.setHttpResponseHeader("content-length", std::to_string(response.get_m_content_length()))
			.setHttpResponseHeader("content-language", response.get_m_content_language())
			.setHttpResponseHeader("content-type", response.get_m_content_type())
			.setHttpResponseHeader("status", std::to_string(response.get_m_status_code()))
			.setHttpResponseHeader("server", response.get_m_server())
			.makeHttpResponseMessage()
	);
}

std::map<std::string, std::string>
Server::makeCgiEnvpMap(Request req, Response res)
{
	std::map<std::string, std::string> map;
	Request &request = req;
	Uri uri = request.get_m_uri();

	/*
	** auth 관련 AUTH_TYPE REMOTE_USER REMOTE_IDENT
	*/
	map["SERVER_SOFTWARE"] = std::string("ftinx/1.0");
	map["SERVER_NAME"] = res.get_m_cgi_server_name();
	map["GATEWAY_INTERFACE"] = "Cgi/1.1";
	map["SERVER_PROTOCOL"] = request.get_m_http_version();
	map["SERVER_PORT"] = std::to_string(res.get_m_cgi_port());
	map["REQUEST_METHOD"] = request.getMethod();
	map["PATH_INFO"] = uri.get_m_path();
	map["PATH_TRANSLATED"] = uri.get_m_path();
	map["SCRIPT_NAME"] = uri.get_m_path();
	map["QUERY_STRING"] = uri.get_m_query_string();
	map["REMOTE_ADDR"] = ft::iNetNtoA(res.get_m_cgi_client_addr());
	map["CONTENT_TYPE"] = request.getContentType();
	map["CONTENT_LENGTH"] = request.getContentLength();
	return (map);
}

char **
Server::makeCgiEnvp(Request req, Response res)
{
	std::map<std::string, std::string> env_map = makeCgiEnvpMap(req, res);
	std::map<std::string, std::string>::const_iterator i = env_map.begin();
	char **env = new char*[env_map.size() + 1];
	int j;

	j = 0;
	while(i != env_map.end())
	{
		env[j] = ft::strdup(i->first + "=" + i->second);
		i++;
		j++;
	}
	env[j] = NULL;
	return (env);
}

Response
Server::executeCgi(Request req, Response res)
{
	pid_t pid;
	Request &request = req;
	Response &response = res;
	char** envp = Server::makeCgiEnvp(req, res);

	if ((pid = fork()) < 0)
		return (Server::page404("errors/default_error.html"));
	if (pid == 0)
	{
		execve((request.get_m_uri().get_m_path()).c_str(), 0, envp);
	}
	else
	{
		/* code */
	}
	return (response);
}

Response
Server::postAuth(Request req, Response res)
{
	std::string path = req.get_m_uri().get_m_path();

	if (path == "/auth")
	{
		std::map<std::string, std::string> m_query = Server::parseQuery(req.get_m_body());
		std::string username;
		std::string password;

		if(m_query.find("formType") != m_query.end()
		&& m_query.find("formType")->second == "login")
		{
			username = m_query.find("username")->second;
			password = m_query.find("password")->second;

			printf("::%s:: ::%s::", username.c_str(), password.c_str());
			printf("::%d:: ::%d::", username == "42seoul", password == "42seoul");

			if (username == "42seoul" && password == "42seoul")
				return (Server::postLoginSuccess());
			else
				return (Server::page200());
		}
	}
	return (res);
}

Response
Server::methodPOST(int clientfd)
{
	Response response;
	response.set_m_err_page_path(this->m_err_page_path);

	/* CGI Setting */
	response.set_m_cgi_client_addr(this->m_client_addr.sin_addr.s_addr);
	response.set_m_cgi_port(this->get_m_port());
	response.set_m_cgi_server_name(this->get_m_server_name());
	response = Server::page404(response.get_m_err_page_path());

	response = post("/auth", this->m_requests[clientfd], response, Server::postAuth);
	response = post("/auth.cgi", this->m_requests[clientfd], response, Server::executeCgi);
	return (response);
}

/*============================================================================*/
/**********************************  PUT  *************************************/
/*============================================================================*/

Response
Server::methodPUT(int clientfd)
{
	(void) clientfd;
	return (Server::page404("errors/default_error.html"));
}

/*============================================================================*/
/*********************************  DELETE  ***********************************/
/*============================================================================*/

Response
Server::methodDELETE(int clientfd)
{
	Uri uri = this->m_requests[clientfd].get_m_uri();
	std::string path = uri.get_m_path();

	if (ft::isValidFilePath(path))
	{
		if (unlink(path.c_str()) == 0)
			return (Server::page200());
	}
	return (Server::page404("errors/default_error.html"));
}

/*============================================================================*/
/*********************************  OPTIONS  **********************************/
/*============================================================================*/

Response
Server::options_405(std::string allow_method)
{
	Response response;

	return (
		response
			.setStatusCode(405)
			.setCurrentDate()
			.setContentLanguage("ko, en")
			.setContentType("text/html; charset=UTF-8")
			.setServer("ftnix/1.0 (MacOS)")
			.setHttpResponseHeader("date", response.get_m_date())
			.setHttpResponseHeader("Allow", allow_method)
			.makeHttpResponseMessage()
	);
}

Response
Server::options_204(std::string allow_method)
{
	Response response;

	return (
		response
			.setStatusCode(204)
			.setCurrentDate()
			.setContentLanguage("ko, en")
			.setContentType("text/html; charset=UTF-8")
			.setServer("ftnix/1.0 (MacOS)")
			.setHttpResponseHeader("date", response.get_m_date())
			.setHttpResponseHeader("Allow", allow_method)
			.makeHttpResponseMessage()
	);
}


std::string
Server::makeAllowMethod(std::vector<Method> v, bool *options_allowed)
{
	std::string ret("");

	for(std::vector<Method>::const_iterator i = v.begin(); i != v.end(); ++i)
	{
		if (*i == OPTIONS)
			*options_allowed = true;
		ret += ft::getMethodString(*i) + ", ";
	}
	ret = ret.substr(0, ret.size() - 2);
	return (ret);
}

Response
Server::methodOPTIONS(int clientfd)
{
	std::string path = m_requests[clientfd].get_m_uri().get_m_path();
	bool options_allowed = false;
	std::string allow_method("");
	const std::vector<HttpConfigLocation> locations = m_server_block.get_m_location_block();
	std::vector<HttpConfigLocation>::const_iterator i = locations.begin();

	size_t pos = path.find_first_of("/", 1);
	std::cout << "PATHAPHTAHPT " << path << std::endl;
	if (pos != std::string::npos)
		path = path.substr(0, pos);

	while (i != locations.end())
	{
		if (path == i->get_m_path())
		{
			allow_method = makeAllowMethod(i->get_m_limit_except(), &options_allowed);
			break;
		}
		i++;
	}
	if (i == locations.end())
		return (Server::page404(this->m_err_page_path));
	else if (options_allowed == false)
		return (Server::options_405(allow_method));
	return (Server::options_204(allow_method));
}

/*============================================================================*/
/**********************************  TRACE  ***********************************/
/*============================================================================*/

/*
**	<< TRACE >>
**	Request has body: No
**	Successful response has body: No
**	Safe: No
**	Idempotent: No
**	Cacheable: No
**	Allowed in HTML forms: No
*/
Response
Server::methodTRACE(int clientfd)
{
	Response response = Response();

	return (
		response
			.setStatusCode(200)
			.setCurrentDate()
			.setServer("ftnix/1.0 (MacOS)")
			.setBodyDocument(this->m_requests[clientfd].get_m_message())
			.setHttpResponseHeader("date", response.get_m_date())
			.setHttpResponseHeader("content-length", std::to_string(response.get_m_content_length()))
			.setHttpResponseHeader("content-type", "message/http")
			.setHttpResponseHeader("connection", "close")
			.setHttpResponseHeader("status", std::to_string(response.get_m_status_code()))
			.setHttpResponseHeader("server", response.get_m_server())
			.makeHttpResponseMessage()
	);
}

/*============================================================================*/
/*******************************  STATUS CODE *********************************/
/*============================================================================*/

/*============================================================================*/
/**********************************  1XX  *************************************/
/*============================================================================*/

/*============================================================================*/
/**********************************  2XX  *************************************/
/*============================================================================*/

Response
Server::page200()
{
	Response response = Response();

	return (
		response
			.setStatusCode(200)
			.setCurrentDate()
			.setContentLanguage("ko, en")
			.setContentType("text/html; charset=UTF-8")
			.setServer("ftnix/1.0 (MacOS)")
			.setPublicFileDocument("index.html")
			.setHttpResponseHeader("date", response.get_m_date())
			.setHttpResponseHeader("content-length", std::to_string(response.get_m_content_length()))
			.setHttpResponseHeader("content-language", response.get_m_content_language())
			.setHttpResponseHeader("content-type", response.get_m_content_type())
			.setHttpResponseHeader("status", std::to_string(response.get_m_status_code()))
			.setHttpResponseHeader("server", response.get_m_server())
			.makeHttpResponseMessage()
	);
}

/*============================================================================*/
/**********************************  3XX  *************************************/
/*============================================================================*/


/*============================================================================*/
/**********************************  4XX  *************************************/
/*============================================================================*/

Response
Server::badRequest_400()
{
	Response response = Response();

	return (
		response
			.setStatusCode(400)
			.setCurrentDate()
			.setServer("ftnix/1.0 (MacOS)")
			.setHttpResponseHeader("date", response.get_m_date())
			.setHttpResponseHeader("content-length", std::to_string(response.get_m_content_length()))
			.setHttpResponseHeader("status", std::to_string(response.get_m_status_code()))
			.setHttpResponseHeader("server", response.get_m_server())
			.makeHttpResponseMessage()
	);
}

Response
Server::page404(std::string path)
{
	Response response = Response();

	return (
		response
			.setStatusCode(404)
			.setCurrentDate()
			.setContentLanguage("ko, en")
			.setContentType("text/html; charset=UTF-8")
			.setServer("ftnix/1.0 (MacOS)")
			.setPublicFileDocument(path)
			.setHttpResponseHeader("date", response.get_m_date())
			.setHttpResponseHeader("content-length", std::to_string(response.get_m_content_length()))
			.setHttpResponseHeader("content-language", response.get_m_content_language())
			.setHttpResponseHeader("content-type", response.get_m_content_type())
			.setHttpResponseHeader("status", std::to_string(response.get_m_status_code()))
			.setHttpResponseHeader("server", response.get_m_server())
			.makeHttpResponseMessage()
	);
}

Response
Server::methodNotAllow_405()
{
	Response response = Response();

	return (
		response
			.setStatusCode(405)
			.setCurrentDate()
			.setServer("ftnix/1.0 (MacOS)")
			.setHttpResponseHeader("date", response.get_m_date())
			.setHttpResponseHeader("content-length", std::to_string(response.get_m_content_length()))
			.setHttpResponseHeader("status", std::to_string(response.get_m_status_code()))
			.setHttpResponseHeader("server", response.get_m_server())
			.makeHttpResponseMessage()
	);
}

/*============================================================================*/
/**********************************  5XX  *************************************/
/*============================================================================*/

Response
Server::methodNotImplemented_501()
{
	Response response = Response();

	return (
		response
			.setStatusCode(501)
			.setCurrentDate()
			.setServer("ftnix/1.0 (MacOS)")
			.setHttpResponseHeader("date", response.get_m_date())
			.setHttpResponseHeader("content-length", std::to_string(response.get_m_content_length()))
			.setHttpResponseHeader("status", std::to_string(response.get_m_status_code()))
			.setHttpResponseHeader("server", response.get_m_server())
			.makeHttpResponseMessage()
	);
}

/*============================================================================*/
/***************************  SERVER METHOD UTIL  *****************************/
/*============================================================================*/

Response
Server::parseErrorResponse(int clientfd)
{
	if (this->m_requests[clientfd].get_m_error_code() == 400)
		return (badRequest_400());
	else if (this->m_requests[clientfd].get_m_error_code() == 501)
		return (methodNotImplemented_501());
	else
		return (page404("errors/default_error.html"));
}

Response
Server::getDirectory()
{
	Response response = Response();

	return (
		response
			.setStatusCode(200)
			.setCurrentDate()
			.setContentLanguage("ko, en")
			.setContentType("text/html; charset=UTF-8")
			.setServer("ftnix/1.0 (MacOS)")
			.setPublicFileDocument("index.html")
			.setHttpResponseHeader("date", response.get_m_date())
			.setHttpResponseHeader("content-length", std::to_string(response.get_m_content_length()))
			.setHttpResponseHeader("content-language", response.get_m_content_language())
			.setHttpResponseHeader("content-type", response.get_m_content_type())
			.setHttpResponseHeader("status", std::to_string(response.get_m_status_code()))
			.setHttpResponseHeader("server", response.get_m_server())
			.makeHttpResponseMessage()
	);
}

Response
Server::get(std::string path, Request req, Response res, Response (*func)(Request req, Response res))
{
	if (path == req.get_m_uri().get_m_path())
		return (func(req, res));
	return (res);
}

Response
Server::post(std::string path, Request req, Response res, Response (*func)(Request req, Response res))
{
	if (path == req.get_m_uri().get_m_path())
		return (func(req, res));
	return (res);
}

Response
Server::put(std::string path, Request req, Response res, Response (*func)(Request req, Response res))
{
	if (path == req.get_m_uri().get_m_path())
		return (func(req, res));
	return (res);
}

Response
Server::del(std::string path, Request req, Response res, Response (*func)(Request req, Response res))
{
	if (path == req.get_m_uri().get_m_path())
		return (func(req, res));
	return (res);
}

Response
Server::options(std::string path, Request req, Response res, Response (*func)(Request req, Response res))
{
	if (path == req.get_m_uri().get_m_path())
		return (func(req, res));
	return (res);
}

Response
Server::trace(std::string path, Request req, Response res, Response (*func)(Request req, Response res))
{
	if (path == req.get_m_uri().get_m_path())
		return (func(req, res));
	return (res);
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
	Response response = Response();
	Method method = this->m_requests[clientfd].get_m_method();

	/* make Response for Parse Error */
	if (this->m_requests[clientfd].get_m_error_code())
		response = this->parseErrorResponse(clientfd);

	/* make Response for Method */
	if (method == GET)
		response = this->methodGET(clientfd);
	else if (method == HEAD)
		response = this->methodHEAD(clientfd);
	else if (method == POST)
		response = this->methodPOST(clientfd);
	else if (method == PUT)
		response = this->methodPUT(clientfd);
	else if (method == DELETE)
		response = this->methodDELETE(clientfd);
	else if (method == TRACE)
		response = this->methodTRACE(clientfd);
	else if (method == OPTIONS)
		response = this->methodOPTIONS(clientfd);
	else
		response = methodNotAllow_405();

	/* config Method */
	// response = get("/hi", this->m_requests[clientfd], response, Server::getDirectory);
	// {
	// 	Response response;
	// 	std::vector<HttpConfigLocation> location_block = this->m_server_block.get_m_location_block();
	// 	std::vector<HttpConfigLocation>::const_iterator i = location_block.begin();
	// 	int j = 0;

	// 	while (i != location_block.end())
	// 	{
	// 		printf("location %d\n", j++);
	// 		printf("path:: %s\n", i->get_m_path().c_str());
	// 		printf("root:: %s\n", i->get_m_root().c_str());
	// 		printf("cgi::path %d\n", i->get_m_cgi_path() == "");
	// 		{
	// 			std::vector<Method> limit_except = i->get_m_limit_except();
	// 			std::vector<Method>::const_iterator limit = limit_except.begin();
	// 			while (limit != limit_except.end())
	// 			{
	// 				printf("%d", *limit);
	// 				// std::cout << ' ' << *limit << std::endl;
	// 				switch (*limit)
	// 				{
	// 					case HEAD:
	// 					case GET:
	// 					case POST:
	// 					case PUT:
	// 					case DELETE:
	// 					case OPTIONS:
	// 					case TRACE:
	// 					default:
	// 						break;
	// 				}
	// 				limit++;
	// 			}
	// 			printf("\n");
	// 			// std::vector<std::string> index = i->get_m_index();
	// 			// std::vector<std::string> get_m_cgi();
	// 		}
	// 		i++;
	// 	}
	// }

	/* 전체 Response Message 확인 할 수 있음 */
	// printf("%s\n", response.get_m_reponse_message().c_str());

	write(clientfd, response.get_m_reponse_message().c_str(), response.get_m_response_size());
	return ;
}
