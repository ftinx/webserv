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
	/* Config */
	this->m_mime_types = rhs.m_mime_types;
	this->m_server_block = rhs.m_server_block;
	this->m_server_name = rhs.m_server_name;
	this->m_port = rhs.m_port;
	this->m_err_page_path = rhs.m_err_page_path;
	this->m_content_length = rhs.m_content_length;
	this->m_location_size = rhs.m_location_size;
	this->m_root = rhs.m_root;

	/* Parse */
	this->m_headLocation = rhs.m_headLocation;
	this->m_getLocation = rhs.m_getLocation;
	this->m_postLocation = rhs.m_postLocation;
	this->m_putLocation = rhs.m_putLocation;
	this->m_deleteLocation = rhs.m_deleteLocation;
	this->m_optionsLocation = rhs.m_optionsLocation;
	this->m_traceLocation = rhs.m_traceLocation;
	this->m_httpConfigFilePathSet = rhs.m_httpConfigFilePathSet;
	this->m_getLocationAutoIndex = rhs.m_getLocationAutoIndex;

	/* Socket */
	this->m_server_addr = rhs.m_server_addr;
	this->m_client_addr = rhs.m_client_addr;
	this->m_server_socket = rhs.m_server_socket;
	this->m_client_socket = rhs.m_client_socket;
	this->fd_num = rhs.fd_num;
	this->sockfd = rhs.sockfd;
	this->readn = rhs.readn;
	this->maxfd = rhs.maxfd;
	this->m_main_fds = rhs.m_main_fds;
	this->m_copy_fds = rhs.m_copy_fds;

	/* Request, Response */
	this->m_requests = rhs.m_requests;
	this->m_responses = rhs.m_responses;
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

std::vector<HttpConfigLocation>
Server::get_m_postLocation()
{
	return (this->m_postLocation);
}

fd_set
Server::get_m_write_fds()
{
	return (this->m_write_fds);
}

/*============================================================================*/
/********************************  Setter  ************************************/
/*============================================================================*/

/*============================================================================*/
/*****************************  Server Util  **********************************/
/*============================================================================*/

void
Server::noteHttpConfigLocation()
{
	std::vector<HttpConfigLocation> location_block = this->m_server_block.get_m_location_block();
	std::vector<HttpConfigLocation>::const_iterator location_iter = location_block.begin();

	while (location_iter != location_block.end())
	{
		std::vector<Method> limit_except = location_iter->get_m_limit_except();
		std::vector<Method>::const_iterator limit = limit_except.begin();
		m_httpConfigFilePathSet.push_back(location_iter->get_m_path());
		while (limit != limit_except.end())
		{
			switch (*limit)
			{
				case HEAD:
					this->m_headLocation.push_back(*location_iter);
					break;
				case GET:
					this->m_getLocation.push_back(*location_iter);
					this->m_getLocationAutoIndex.insert(std::pair<std::string, bool>(location_iter->get_m_path(), location_iter->get_m_autoindex()));
					break;
				case POST:
					this->m_postLocation.push_back(*location_iter);
					break;
				case PUT:
					this->m_putLocation.push_back(*location_iter);
					break;
				case DELETE:
					this->m_deleteLocation.push_back(*location_iter);
					break;
				case OPTIONS:
					this->m_optionsLocation.push_back(*location_iter);
					break;
				case TRACE:
					this->m_traceLocation.push_back(*location_iter);
					break;
				default:
					std::cout << "Error: noteCGILocation method switch error " << std::endl;
					break;
			}
			limit++;
		}
		location_iter++;
	}
	return ;
}

void
Server::init(HttpConfigServer server_block, std::string server_name, int port, std::string err_page_path, int content_length, size_t location_size, std::string root, std::map<std::string, std::string> mime_types)
{
	this->m_requests = std::vector<Request>(MAX_SOCK_NUM);
	this->m_responses = std::vector<Response>(MAX_SOCK_NUM);
	this->m_server_block = server_block;
	this->m_server_name = server_name;
	this->m_port = port;
	if (err_page_path != "")
		this->m_err_page_path = err_page_path;
	else
		this->m_err_page_path = "./www/errors/default_error.html";
	this->m_content_length = content_length;
	this->m_location_size = location_size;
	this->m_root = root;
	this->m_mime_types = mime_types;
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

void
Server::runServer()
{
	// rfc보고 정의
	struct timeval timeout;
	timeout.tv_sec = 4;
	timeout.tv_usec = 2;

	FD_ZERO(&this->m_main_fds);
	FD_ZERO(&this->m_write_fds);
	FD_SET(this->m_server_socket, &this->m_main_fds);

	this->maxfd = 0;
	this->maxfd = this->m_server_socket;

	while (42)
	{
		printf("---Select Wait %d---\n", this->maxfd);
		this->m_read_fds = this->m_main_fds;
		this->m_copy_write_fds = this->m_write_fds;

		this->fd_num = select(
			this->maxfd + 1 ,
			&this->m_read_fds,
			&this->m_copy_write_fds,
			reinterpret_cast<fd_set *>(0),
			&timeout
		);

		switch (this->fd_num)
		{
			case -1:
				std::cerr << "select error" << std::endl;
				return ;
			case 0:
				std::cout << "---Timeout Reset---" << std::endl;
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

	for (int i = 0; i <= this->maxfd; i++)
	{
		int sockfd = i;
		if (ft::fdIsSet(sockfd, &this->m_copy_write_fds))
		{
			sendResponse(sockfd);
			FD_CLR(sockfd, &this->m_write_fds);
		}
	}
	if (ft::fdIsSet(this->m_server_socket, &this->m_read_fds))
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
			std::cerr << "accept error" << std::endl;

		ft::fdSet(this->m_client_socket, &this->m_main_fds);
		if (this->m_client_socket > this->maxfd)
			this->maxfd = this->m_client_socket;
		printf("Accept OK\n");
		return ;
	}

	for (int i = 0; i <= this->maxfd; i++)
	{
		this->sockfd = i;
		if (ft::fdIsSet(this->sockfd, &this->m_read_fds))
		{
			/*
			** Request 부분 시작, false시 에러 받아줘야
			*/
			this->m_requests[this->sockfd] = Request();
			this->m_requests[this->sockfd].getMessage(this->sockfd);

			FD_SET(this->sockfd, &this->m_write_fds);
			//FD_CLR(this->sockfd, &this->m_main_fds);
			//FD_CLR(this->sockfd, &this->m_read_fds);
			if (--this->fd_num <= 0)
				break;
		}
	}
	return ;
}

/*============================================================================*/
/*******************************  Response  ***********************************/
/*============================================================================*/

/*============================================================================*/
/*********************************  HEAD  *************************************/
/*============================================================================*/
std::string
Server::getMimeType(std::string extension)
{
	std::map<std::string, std::string>::const_iterator it;
	it = this->m_mime_types.find(extension);
	if (it == m_mime_types.end())
		return "none";
	return (it->second);
}

Response
Server::methodHEAD(int clientfd)
{
	return (methodGET(clientfd, "HEAD"));
}
/*============================================================================*/
/**********************************  GET  *************************************/
/*============================================================================*/
Response
Server::getTest(Request req, Response res)
{
	(void)req;
	return (res);
}

std::string
Server::makeAutoindexPage(std::string path)
{
	std::string page;
	std::string dirName(path);
	struct dirent *entry;
	DIR *dirptr = opendir(path.c_str());

	// if ((dirptr = opendir(path.c_str())) != NULL)
	// {
	// 	closedir(dirptr);
	// 	return ("");
	// }
	page += std::string("<!DOCTYPE html>")
			+ std::string("<html lang=\"en\">")
			+ std::string("<head>")
			+ std::string("<title>")
			+ std::string(dirName)
			+ std::string("</title>")
			+ std::string("</head>")
			+ std::string("<body>")
			+ std::string("<p>");
	if (dirName[0] != '/')
	{
		dirName = std::string("/") + dirName;
		// std::cout << dirName << std::endl;
	}
	for (entry = readdir(dirptr) ; entry ; entry = readdir(dirptr))
	{
		page += std::string("<p><a href=\"")
				// + std::string(dirName)
				// + std::string("/")
				+ std::string(entry->d_name)
				+ std::string("\">")
				+ std::string(entry->d_name)
				+ std::string("</a></p>\n");
	}
	page += std::string("</p>")
			+ std::string("</body>")
			+ std::string("</html>");
	closedir(dirptr);
	return (page);
}

bool
Server::checkHttpConfigFilePathHead(std::string path)
{
	int path_length = path.length();
	std::vector<HttpConfigLocation>::const_iterator headlocation_iter = this->m_headLocation.begin();
	while (headlocation_iter != this->m_headLocation.end())
	{
		int headlocation_length = headlocation_iter->get_m_path().length();
		if (path == headlocation_iter->get_m_path())
			return (true);
		if (path_length > headlocation_length
		&& headlocation_length > 1
		&& path.substr(0, headlocation_length) == headlocation_iter->get_m_path())
			return (true);
		headlocation_iter++;
	}
	return (false);
}

bool
Server::checkHttpConfigFilePath(std::string path, std::string method)
{
	int path_length = path.length();
	if (method == "HEAD")
	{
		if (checkHttpConfigFilePathHead(path))
			return (false);
		return (true);
	}
	std::vector<std::string>::const_iterator path_iter = this->m_httpConfigFilePathSet.begin();
	while (path_iter != this->m_httpConfigFilePathSet.end())
	{
		int path_iter_length = (*path_iter).length();
		if (path == *path_iter)
			return (false);
		if (path_length > path_iter_length
		&& path_iter_length > 1
		&& path.substr(0, path_iter_length) == *path_iter)
			return (false);
		path_iter++;
	}
	return (true);
}

Response
Server::methodGET(int clientfd, std::string method)
{
	Response response;
	std::string absolute_path;// = this->m_requests[clientfd].get_m_reset_path();
	HttpConfigLocation location_block;// = this->m_requests[clientfd].get_m_location_path();
	std::string type;
	std::string extension;

	if (ft::isValidDirPath(absolute_path)) // 폴더라면
	{
		if (absolute_path.find("/", absolute_path.length() - 1) == std::string::npos)
			absolute_path = absolute_path + std::string("/");
		std::vector<std::string> index = location_block.get_m_index();
		for (std::vector<std::string>::const_iterator index_it = index.begin() ; index_it != index.end() ; ++index_it) // index 벡터 순회
		{
			if (ft::isValidFilePath(absolute_path + std::string("/") + *index_it) == 0) // 만약 유효한 파일이면
			{
				extension = (*index_it).substr((*index_it).find_last_of(".") + 1, std::string::npos);
				type = getMimeType(extension);
				return (Server::makeResponseMessage(200, absolute_path + std::string("/") + *index_it, method, type)); // 200 응답과 반환
			}
			else // index 순회했는데 일치하는게 없으면 오토인덱스 체크
			{
				std::map<std::string, bool>::const_iterator autoindex_it;
				for (autoindex_it = this->m_getLocationAutoIndex.begin() ; autoindex_it != this->m_getLocationAutoIndex.end() ; ++autoindex_it)
				{
					if (location_block.get_m_path().compare((*autoindex_it).first) == 0 && (*autoindex_it).second == true)
						return (Server::makeResponseBodyMessage(200, makeAutoindexPage(absolute_path)));
				}
			}
		}
	}
	else // 폴더가 아니라면
	{
		if (ft::isValidFilePath(absolute_path))
		{
			extension = absolute_path.substr(absolute_path.find_last_of(".") + 1, std::string::npos);
			type = getMimeType(extension);
			return (Server::makeResponseMessage(200, absolute_path, method, type));
		}
	}
	return (Server::page404(this->m_err_page_path));
}

// Response
// Server::methodGET(int clientfd, std::string method)
// {
// 	/* map < path, autoindex 여부 > 예시 */
// 	std::map<std::string, bool>::iterator iter;
// 	for(iter = this->m_getLocationAutoIndex.begin(); iter != this->m_getLocationAutoIndex.end(); iter++){
// 		std::cout << "[\"" << iter->first << "\", " << iter->second << "]" << std::endl;
// 	}

// 	// (void) clientfd;
// 	// return (Server::makeResponseMessage(200, "./www/index.html"));
// 	Response response;
// 	std::string content_type;
// 	std::string path;
// 	// get_cnt++;
// 	// std::cout << "---------------------get_cnt : "<< get_cnt << std::endl;
// 	path = this->m_requests[clientfd].get_m_uri().get_m_path();
// 	// if(checkHttpConfigFilePath(path, method))
// 	// 	return (Server::makeResponseMessage(405, ""));
// 	if (ft::isValidFilePath(this->m_root + path))
// 	{
// 		content_type = getMimeType(path.substr(path.find_last_of(".") + 1, std::string::npos));
// 		// std::cout << "----1------ entension : " << path.substr(path.find_last_of(".") + 1, std::string::npos) << std::endl;
// 		// std::cout << "----1------ contenttype : " << content_type << std::endl;
// 		return (Server::makeResponseMessage(200, this->m_root + path, method, content_type));
// 	}
// 	else if (ft::isValidDirPath(this->m_root + path))
// 	{
// 		std::string block;
// 		if (path.compare("/") == 0 || path.compare("") == 0)
// 			block = "/";
// 		else
// 			block = path.substr(0, path.find_last_of("/"));
// 		std::vector<HttpConfigLocation> location = this->m_server_block.get_m_location_block();
// 		for (std::vector<HttpConfigLocation>::const_iterator location_it = location.begin() ; location_it != location.end() ; ++location_it)
// 		{
// 			if (block.compare(location_it->get_m_path()) == 0)
// 			{
// 				if (location_it->get_m_autoindex() && location_it->get_m_index().empty() && ft::isValidDirPath(location_it->get_m_root() + block))
// 					return (Server::makeResponseBodyMessage(200, makeAutoindexPage(location_it->get_m_root()), method));
// 				std::vector<std::string> v2 = location_it->get_m_index();
// 				for (std::vector<std::string>::const_iterator index_it = v2.begin() ; index_it != v2.end() ; ++index_it)
// 				{
// 					if (ft::isValidFilePath(location_it->get_m_root() + block + *index_it))
// 					{
// 						path = location_it->get_m_root() + block + *index_it;
// 						content_type = getMimeType(path.substr(path.find_last_of(".") + 1, std::string::npos));
// 						// std::cout << "---2------- entension : " << path.substr(path.find_last_of(".") + 1, std::string::npos) << std::endl;
// 						// std::cout << "---2------- contenttype : " << content_type << std::endl;
// 						return (Server::makeResponseMessage(200, location_it->get_m_root() + block + *index_it, method, content_type));
// 					}
// 					// else if (ft::isValidDirPath(location_it->get_m_root() + block)) // autoindex 파트 조건 수정 필요
// 					// 	return (Server::makeResponseBodyMessage(200, makeAutoindexPage(location_it->get_m_root() + block)));
// 				}
// 			}
// 		}
// 	}
// 	return (Server::page404(this->m_err_page_path));
// }

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

std::map<std::string, std::string>
Server::makeCgiEnvpMap(Request req, Response res)
{
	std::map<std::string, std::string> map;
	Uri uri = req.get_m_uri();

	/*
	** auth 관련 AUTH_TYPE REMOTE_USER REMOTE_IDENT
	*/
	map["SERVER_SOFTWARE"] = std::string("ftinx/1.0");
	map["SERVER_NAME"] = res.get_m_cgi_server_name();
	map["GATEWAY_INTERFACE"] = "Cgi/1.1";
	map["SERVER_PROTOCOL"] = req.get_m_http_version();
	map["SERVER_PORT"] = std::to_string(res.get_m_cgi_port());
	map["REQUEST_METHOD"] = req.getMethod();
	//map["PATH_INFO"] = this->parseCgiPathInfo(req);
	// map["PATH_INFO"] = "/cgi-bin/cgi_tester";
	map["PATH_TRANSLATED"] = uri.get_m_path();
	map["SCRIPT_NAME"] = uri.get_m_path();
	map["QUERY_STRING"] = uri.get_m_query_string();
	map["REMOTE_ADDR"] = ft::iNetNtoA(res.get_m_cgi_client_addr());
	map["CONTENT_TYPE"] = req.getContentType();
	map["CONTENT_LENGTH"] = req.getContentLength();
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
Server::executeCgi(Request req, Response res, fd_set *write_fds)
{
	pid_t pid;
	// Request &request = req;
	Response &response = res;
	char** envp = Server::makeCgiEnvp(req, res);
	int pipe1[2];
	int pipe2[2];
	int ret;

	std::cout << "Execute Cgi >0<" << std::endl;
	if (pipe(pipe1) < 0 || pipe(pipe2) < 0)
		return (Server::page404(this->m_err_page_path));

	int parent_stdout = pipe1[1];
	int parent_stdin = pipe2[0];
	int cgi_stdout = pipe2[1];
	int cgi_stdin = pipe1[0];

	close(pipe1[0]);
	close(pipe2[1]);

	fcntl(cgi_stdout, F_SETFL, O_NONBLOCK);
	fcntl(cgi_stdin, F_SETFL, O_NONBLOCK);
	fcntl(parent_stdout, F_SETFL, O_NONBLOCK);
	fcntl(parent_stdin, F_SETFL, O_NONBLOCK);

	if ((pid = fork()) < 0)
		return (Server::page404(this->m_err_page_path));
	if (pid == 0)
	{
		close(parent_stdout);
		close(parent_stdin);
		// dup2 실패에 대한 에러처리를 어떤 방식으로 해줘야 할지?
		dup2(cgi_stdout, STDOUT_FILENO);
		dup2(cgi_stdin, STDIN_FILENO);
		// 일반화 해줘야
		ret = execve("./cgi-bin/cgi_tester", 0, envp);
		exit(ret);
	}
	else
	{
		char buff[1025];
		close(cgi_stdout);
		close(cgi_stdin);
		read(parent_stdout, buff, 1024);
		buff[1024] = '\0';
		std::cout << "\n" << buff << std::endl;
		response
			.setStatusCode(200)
			.setCurrentDate()
			.setBodyDocument(std::string(buff))
			.setContentLanguage("ko")
			.setContentType("text/html; charset=UTF-8")
			.setServer("hihih")
			.setHttpResponseHeader("date", response.get_m_date())
			.setHttpResponseHeader("content-length", std::to_string(response.get_m_content_length()))
			.setHttpResponseHeader("content-language", response.get_m_content_language())
			.setHttpResponseHeader("content-type", response.get_m_content_type())
			.setHttpResponseHeader("status", std::to_string(response.get_m_status_code()))
			.setHttpResponseHeader("server", response.get_m_server())
			.makeHttpResponseMessage();
		ft::fdSet(parent_stdout, write_fds);
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
				return (Server::makeResponseMessage(200, "./www/srcs/login.html"));
			else
				return (Server::makeResponseMessage(200, "./www/index.html"));
		}
	}
	return (res);
}

Response
Server::HttpConfigPost(Request req, Response res)
{
	/* Request에 대한 요청 처리*/
	(void) req;
	(void) res;

	/* Response */
	return (
		Server::makeResponseMessage(200)
	);
}

Response
Server::methodPOST(int clientfd)
{
	(void) clientfd;
	// Response response;

	// /* Response Setting */
	// response.set_m_err_page_path(this->m_err_page_path);

	// /* CGI Response Setting */
	// response.set_m_cgi_client_addr(this->m_client_addr.sin_addr.s_addr);
	// response.set_m_cgi_port(this->get_m_port());
	// response.set_m_cgi_server_name(this->get_m_server_name());
	// response = Server::page404(response.get_m_err_page_path());

	/* Route */
	// if (this->m_requests[clientfd].get_m_uri().get_m_path() == "/cgi-bin/cgi_tester")
	// 	response = executeCgi(this->m_requests[clientfd], response, &this->m_write_fds);
	// response = post("/auth", this->m_requests[clientfd], response, Server::postAuth);
	// response = post("/cgi-bin/cgi_tester", this->m_requests[clientfd], response, &this->m_write_fds, Server::executeCgi);

	/* Config File Route */
	// if (this->m_postLocation.size() == 0)
	// 	return (response);
	// std::vector<HttpConfigLocation>::const_iterator location_iter = this->m_postLocation.begin();
	// while (location_iter != this->m_postLocation.end())
	// {
	// 	/* HttpConfig path Response Setting */
	// 	response.set_m_cgi_extension(location_iter->get_m_cgi());
	// 	response.set_m_index_file(location_iter->get_m_index());
	// 	response.set_m_root(location_iter->get_m_root());
	// 	// response.set_m_cgi_path(location_iter->get_m_cgi_path());

	// 	/* CGI */
	// 	if (location_iter->get_m_cgi_path() != "")
	// 		response = post(location_iter->get_m_path(), this->m_requests[clientfd], response, &this->m_write_fds, Server::executeCgi);
	// 	// else
	// 	// 	response = post(location_iter->get_m_path(), this->m_requests[clientfd], response, Server::HttpConfigPost);
	// 	location_iter++;
	// }
	if(checkHttpConfigFilePath(this->m_requests[clientfd].get_m_uri().get_m_path()))
		return (Server::makeResponseMessage(405, ""));
	return (Server::makeResponseMessage(405, ""));
}

/*============================================================================*/
/**********************************  PUT  *************************************/
/*============================================================================*/

Response
Server::methodPUT(int clientfd)
{
	Request req = this->m_requests[clientfd];
	std::string path = req.get_m_uri().get_m_path();
	int fd;
	const char *body;
	int status_code = 0;

	/* cgi part should be added */
	std::cout << this->m_root + path << std::endl;
	if (ft::isValidFilePath(this->m_root + path) == false)
	{
		if ((fd = open((this->m_root + path).c_str(), O_RDWR | O_CREAT, 0666)) < 0)
 			return (Server::page404(this->m_err_page_path));
		status_code = 201;
	}
	else
	{
		if ((fd = open((this->m_root + path).c_str(), O_RDWR | O_TRUNC, 0666)) < 0)
			return (Server::page404(this->m_err_page_path));
		status_code = 200;
	}
	/* 405 check */
	body = req.get_m_body().c_str();
	if (write(fd, req.get_m_body().c_str(), ft::strlen(req.get_m_body().c_str())) < 0)
	{
		close(fd);
		return (Server::page404(this->m_err_page_path));
	}
	else
	{
		close(fd);
		return (Server::makeResponseBodyMessage(status_code, ""));
	}
	return (Server::page404(this->m_err_page_path));
}

/*============================================================================*/
/*********************************  DELETE  ***********************************/
/*============================================================================*/

Response
Server::methodDELETE(int clientfd)
{
	Uri uri = this->m_requests[clientfd].get_m_uri();
	std::string path = uri.get_m_path();

	/* 405 check */
	if (ft::isValidFilePath(this->m_root + path))
	{
		if (unlink(path.c_str()) == 0)
			return (Server::page200());
	}
	return (Server::page404(this->m_err_page_path));
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

	/* 405 check --  should be changed */
	size_t pos = path.find_first_of("/", 1);
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

Response
Server::makeResponseMessage(
	int statusCode, std::string path, std::string method, std::string contentType,
	int dateHour, int dateMinute, int dateSecond,
	std::string contentLanguage, std::string server
)
{
	Response response = Response();

	return (
		response
			.setStatusCode(statusCode)
			.setCurrentDate(dateHour, dateMinute, dateSecond)
			.setPublicFileDocument(path)
			.setContentLanguage(contentLanguage)
			.setContentType(contentType)
			.setServer(server)
			.setHttpResponseHeader("date", response.get_m_date())
			.setHttpResponseHeader("content-length", std::to_string(response.get_m_content_length()))
			.setHttpResponseHeader("content-language", response.get_m_content_language())
			.setHttpResponseHeader("content-type", response.get_m_content_type())
			.setHttpResponseHeader("status", std::to_string(response.get_m_status_code()))
			.setHttpResponseHeader("server", response.get_m_server())
			.makeHttpResponseMessage(method)
	);
}

Response
Server::makeResponseBodyMessage(
	int statusCode, std::string body, std::string method, std::string contentType,
	int dateHour, int dateMinute, int dateSecond,
	std::string contentLanguage, std::string server
)
{
	Response response = Response();

	return (
		response
			.setStatusCode(statusCode)
			.setCurrentDate(dateHour, dateMinute, dateSecond)
			.setContentLanguage(contentLanguage)
			.setContentType(contentType)
			.setServer(server)
			.setBodyDocument(body)
			.setHttpResponseHeader("date", response.get_m_date())
			.setHttpResponseHeader("content-length", std::to_string(response.get_m_content_length()))
			.setHttpResponseHeader("content-language", response.get_m_content_language())
			.setHttpResponseHeader("content-type", response.get_m_content_type())
			.setHttpResponseHeader("status", std::to_string(response.get_m_status_code()))
			.setHttpResponseHeader("server", response.get_m_server())
			.makeHttpResponseMessage(method)
	);
}

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
		return (page404(this->m_err_page_path));
}

Response
Server::getDirectory(Request req, Response res)
{
	Response response = Response();
	(void) req;
	(void) res;
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
Server::post(std::string path, Request req, Response res, fd_set *write_fds, Response (*func)(Request req, Response res, fd_set *write_fds))
{
	if (path == req.get_m_uri().get_m_path())
		return (func(req, res, write_fds));
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
	switch(method)
	{
	case GET:
		response = this->methodGET(clientfd);
		break;
	case HEAD:
		response = this->methodHEAD(clientfd);
		break;
	case POST:
		response = this->methodPOST(clientfd);
		break;
	case PUT:
		response = this->methodPUT(clientfd);
		break;
	case DELETE:
		response = this->methodDELETE(clientfd);
		break;
	case TRACE:
		response = this->methodTRACE(clientfd);
		break;
	case OPTIONS:
		response = this->methodOPTIONS(clientfd);
		break;
	default:
		response = methodNotAllow_405();
		break;
	}
	/* config Method */
	// response = get("/hi", this->m_requests[clientfd], response, Server::getDirectory);

	/* 전체 Response Message 확인 할 수 있음 */
	// printf("%s\n", response.get_m_reponse_message().c_str());
	std::cout << "\033[47:30m**** response message ****\033[0m" << std::endl;;
	std::cout << response.get_m_reponse_message() << std::endl;

	write(clientfd, response.get_m_reponse_message().c_str(), response.get_m_response_size());
	return ;
}
