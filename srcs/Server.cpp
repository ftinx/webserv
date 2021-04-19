#include "Server.hpp"

#include <bitset>
#include <iostream>

// int check_fd = open("/tmp/.check_fd", O_RDWR | O_CREAT | O_TRUNC, 0666);

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
/****************************  Static variables  ******************************/
/*============================================================================*/

/*============================================================================*/
/******************************  Constructor  *********************************/
/*============================================================================*/

Server::Server()
: m_auth_id(""), m_auth_pw(""), m_fd_table()
{}

Server::Server(Server const &other)
{
	*this = other;
}

Server& Server::operator=(Server const &rhs)
{
	/* Config */
	this->m_mime_types = rhs.m_mime_types;
	this->m_server_block = rhs.m_server_block;
	this->m_server_name = rhs.m_server_name;
	this->m_port = rhs.m_port;
	this->m_content_length = rhs.m_content_length;
	this->m_location_size = rhs.m_location_size;
	this->m_root = rhs.m_root;
	this->m_auth_id = rhs.m_auth_id;
	this->m_auth_pw = rhs.m_auth_pw;

	/* Parse */
	this->m_head_location = rhs.m_head_location;
	this->m_get_location = rhs.m_get_location;
	this->m_post_location = rhs.m_post_location;
	this->m_put_location = rhs.m_put_location;
	this->m_delete_location = rhs.m_delete_location;
	this->m_options_location = rhs.m_options_location;
	this->m_trace_location = rhs.m_trace_location;
	this->m_http_config_file_path_set = rhs.m_http_config_file_path_set;
	this->m_http_config_path_method = rhs.m_http_config_path_method;
	this->m_get_location_auto_index = rhs.m_get_location_auto_index;

	/* Socket */
	this->m_maxfd = rhs.m_maxfd;
	this->m_server_addr = rhs.m_server_addr;
	this->m_client_addr = rhs.m_client_addr;
	this->m_server_socket = rhs.m_server_socket;
	this->m_client_socket = rhs.m_client_socket;
	this->m_sockfd = rhs.m_sockfd;

	/* fd_sets */
	this->m_main_fds = rhs.m_main_fds;
	this->m_read_fds = rhs.m_read_fds;
	this->m_write_fds = rhs.m_write_fds;
	this->m_copy_write_fds = rhs.m_copy_write_fds;
	this->m_fd_table = rhs.m_fd_table;

	/* Request, Response */
	this->m_requests = rhs.m_requests;
	this->m_responses = rhs.m_responses;
	return (*this);
}

/*============================================================================*/
/******************************  Destructor  **********************************/
/*============================================================================*/

Server::~Server(){};

/*============================================================================*/
/*******************************  Overload  ***********************************/
/*============================================================================*/

const char* Server::CgiException::what() const throw()
{
	return ("Error occured in CGI");
}

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
Server::get_m_post_location()
{
	return (this->m_post_location);
}

int
Server::get_m_server_socket() const
{
	return (this->m_server_socket);
}

int
Server::get_m_client_socket() const
{
	return (this->m_client_socket);
}

/*============================================================================*/
/********************************  Setter  ************************************/
/*============================================================================*/

/*============================================================================*/
/********************************  Init  **************************************/
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
		std::string methods("");
		m_http_config_file_path_set.push_back(location_iter->get_m_path());
		while (limit != limit_except.end())
		{
			if (methods == "")
				methods = ft::getMethodString(*limit);
			else
				methods += ", " + ft::getMethodString(*limit);
			switch (*limit)
			{
				case HEAD:
					this->m_head_location.push_back(*location_iter);
					break;
				case GET:
					this->m_get_location.push_back(*location_iter);
					this->m_get_location_auto_index.insert(std::pair<std::string, bool>(location_iter->get_m_path(), location_iter->get_m_autoindex()));
					break;
				case POST:
					this->m_post_location.push_back(*location_iter);
					break;
				case PUT:
					this->m_put_location.push_back(*location_iter);
					break;
				case DELETE:
					this->m_delete_location.push_back(*location_iter);
					break;
				case OPTIONS:
					this->m_options_location.push_back(*location_iter);
					break;
				case TRACE:
					this->m_trace_location.push_back(*location_iter);
					break;
				default:
					std::cout << "Error: noteCGILocation method switch error " << std::endl;
					break;
			}
			limit++;
		}
		m_http_config_path_method.insert(make_pair(location_iter->get_m_path(), methods));
		location_iter++;
	}
	return ;
}

void
Server::init(HttpConfigServer server_block, std::string server_name, int port,
int content_length, size_t location_size, std::string root, std::map<std::string, std::string> mime_types, std::string default_type,
int *maxfd, fd_set *main_fds, fd_set *read_fds, fd_set *write_fds, fd_set *copy_write_fds)
{
	this->m_requests = std::vector<Request>(MAX_SOCK_NUM);
	this->m_responses = std::vector<Response>(MAX_SOCK_NUM);
	this->m_server_block = server_block;
	this->m_server_name = server_name;
	this->m_port = port;
	this->m_content_length = content_length;
	this->m_location_size = location_size;
	this->m_root = root;
	this->m_mime_types = mime_types;
	this->m_default_type = default_type;
	this->m_maxfd = maxfd;
	this->m_main_fds = main_fds;
	this->m_read_fds = read_fds;
	this->m_write_fds = write_fds;
	this->m_copy_write_fds = copy_write_fds;
	return ;
}

/*============================================================================*/
/********************************  Socket  ************************************/
/*============================================================================*/

void
Server::setServerAddr(int port)
{
	memset((void *)&this->m_server_addr, 0x00, sizeof(this->m_server_addr));
	this->m_server_addr.sin_family = AF_INET;
	this->m_server_addr.sin_port = htons(port);
	this->m_server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
}

bool
Server::setServerSocket()
{
	int option;

	option = true;
	if ((this->m_server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
	{
		std::cout << "socket error" << std::endl;
		return (false);
	}
    setsockopt(this->m_server_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(int));
	if (bind(
			this->m_server_socket,
			reinterpret_cast<struct sockaddr *>(&this->m_server_addr),
			static_cast<socklen_t>(sizeof(this->m_server_addr))
		) == -1)
	{
		std::cout << "bind error" << std::endl;
		return (false);
	}
	if (listen(this->m_server_socket, 1000) == -1)
	{
		std::cout << "listen error" << std::endl;
		return (false);
	}
	return (true);
}

void
Server::closeServer()
{
	return ;
}

/*============================================================================*/
/*******************************  Request  ************************************/
/*============================================================================*/

int
Server::findMaxFd()
{
	std::vector<FDT>::const_iterator fd_iter;
	int ret = 0;

	for (fd_iter = m_fd_table.begin() ; fd_iter != m_fd_table.end() ; ++fd_iter)
	{
		ret = std::max(ret, fd_iter->sockfd);
	}
	return (ret);
}

void
Server::acceptSocket()
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
	{
		std::cerr << "accept error" << std::endl;
		return ;
	}
	ft::fdSet(this->m_client_socket, this->m_main_fds);
	fcntl(m_client_socket, F_SETFL, O_NONBLOCK);
	m_fd_table.push_back(ft::makeFDT(C_SOCKET, this->m_client_socket, 0));
	if (this->m_client_socket > *this->m_maxfd)
		*(this->m_maxfd) = this->m_client_socket;
	std::cerr << "Accept OK" << std::endl;
	return ;
}

void
Server::handleRequest(int clientfd)
{
	Request &request = this->m_requests[clientfd];
	Method method = request.get_m_method();
	ft::console_log("HANDLE REQUEST | fd: "+ std::to_string(clientfd));
	ft::console_log("HANDLE REQUEST | method : " + std::to_string(method));
	ft::console_log("HANDLE REQUEST | uri: "+ this->m_requests[clientfd].get_m_reset_path());

	this->m_responses[clientfd] = Response();
	if (this->m_requests[clientfd].get_m_error_code())
		this->m_responses[clientfd] = this->parseErrorResponse(clientfd);
	else
	{
		/* make Response for Method */
		switch(method)
		{
		case GET:
			this->m_responses[clientfd] = this->methodGET(clientfd);
			break;
		case HEAD:
			this->m_responses[clientfd] = this->methodHEAD(clientfd);
			break;
		case POST:
			this->m_responses[clientfd] = this->methodPOST(clientfd);
			break;
		case PUT:
			this->m_responses[clientfd] = this->methodPUT(clientfd);
			break;
		case DELETE:
			this->m_responses[clientfd] = this->methodDELETE(clientfd);
			break;
		case TRACE:
			this->m_responses[clientfd] = this->methodTRACE(clientfd);
			break;
		case OPTIONS:
			this->m_responses[clientfd] = this->methodOPTIONS(clientfd);
			break;
		default:
			std::map<std::string, std::string>::const_iterator it;
			std::string allow_method("");
			it = m_http_config_path_method.find(this->m_requests[clientfd].get_m_uri().get_m_path());
			if (it != m_http_config_path_method.end())
				allow_method = it->second;
			this->m_responses[clientfd] = Server::makeResponseBodyMessage(405, this->m_server_name, "", "", this->m_requests[clientfd].getAcceptLanguage(),
				ft::getMethodString(method), getMimeType("html"), this->m_requests[clientfd].getReferer(), 0, 0, 0, allow_method);
			break;
		}
		this->m_responses[clientfd].setMultipleResponses(request.get_m_count_message());
		request.set_m_count_message(0);
	}
	return ;
}


bool
Server::readProcess()
{
	std::vector<FDT>::const_iterator fd_iter;

	for (fd_iter = m_fd_table.begin() ; fd_iter != m_fd_table.end() ; ++fd_iter)
	{
		int sockfd = fd_iter->sockfd;
		if (ft::fdIsSet(sockfd, this->m_read_fds))
		{
			int ret;
			int status_code = 0;

			if (fd_iter->type == CGI_PIPE)
			{
				Request &request = this->m_requests[fd_iter->clientfd];
				Response &response = this->m_responses[fd_iter->clientfd];
				ft::console_log(":::::::::::::::::::::::::::::::::::::::::::::::3::");
				int status;
				// char buff[SOCK_BUFF];
				char *buff =  (char*)malloc(sizeof(char) * SOCK_BUFF);
				//kill(request.get_m_cgi_pid(), SIGTERM);
				//waitpid(request.get_m_cgi_pid(), &status, 0);
				ft::memset(buff, 0, SOCK_BUFF);
				// if (status == 0)
				// {
				waitpid(request.get_m_cgi_pid(), &status, 0);
				if( 0 < (ret = read(sockfd, buff, SOCK_BUFF - 1)))
				{
					ft::console_log("++++++ READ PROCESS(pipe): " + std::to_string(ret));
					response.setCgiResponse(buff);
					try
					{
						if (status_code == 0 &&
							(status_code = response.findCgiStatusCodeHeader()))
						{
							response.set_m_status_code(status_code);
							response.set_m_has_cgi_response(true);
						}
					}
					catch (std::exception &e)
					{
						std::cout << e.what() << std::endl;
						return (true);
					}
					// ft::fdSet(fd_iter->clientfd, m_write_fds);
					return (false);
				}
				if (response.get_m_cgi_chunked_read_end() == true && ret == 0)
				{
					std::cout << "RET IS 0" << std::endl;
					ft::console_log("::3:: RET 0 body size: " + std::to_string(response.get_m_body().size()));
					close(fd_iter->sockfd);
					ft::fdClr(fd_iter->sockfd, m_main_fds);
					ft::fdSet(fd_iter->clientfd, m_write_fds);
					response.set_m_cgi_chunked_read_end(true);
					this->m_fd_table.erase(fd_iter);
					*m_maxfd = findMaxFd();
					return (true);
				}
			}
			else if(fd_iter->type == C_SOCKET)
			{
				Request &request = this->m_requests[sockfd];
				Response &response = this->m_responses[sockfd];
				int header_status = request.getHeader(sockfd);
				int body_status = -1;
				ft::console_log(":::::::::::::::::::::::::::::::::::::::::::::::1::");
				if (header_status == SUCCESS)
				{
					resetRequest(&request);
					if (request.get_m_method() == POST && request.get_m_check_cgi() == true)
					{
						/* 최적화할때 checkCGI 없애야함. 중복 CGI 검사. */
						if (request.checkCGI() == true)
							executeCgi(request, response, sockfd);
					}
					if (request.get_m_content_length() == -1 && request.get_m_chunked() == false) // 헤더만 들어온 메세지 처리
					{
						ft::console_log("ONLY HEADER | method: "+ std::to_string(request.get_m_method()));
						ft::console_log("ONLY HEADER | uri: "+ request.get_m_reset_path());
						ft::console_log("ONLY HEADER | fd: "+ std::to_string(sockfd));
						handleRequest(sockfd);
						if (this->m_responses[sockfd].get_m_status_code() != 0)
						{
							ft::console_log("STATUS CODE: " + std::to_string(this->m_responses[sockfd].get_m_status_code()));
							ft::fdSet(sockfd, m_write_fds);
						}
					}
				}
				else if (header_status == FAIL)
				{
					ft::console_log("GET HEADER | FAIL");
					close(sockfd);
					ft::fdClr(sockfd, m_main_fds);
					m_fd_table.erase(fd_iter);
					*m_maxfd = findMaxFd();
					return (false);
				}
				else if (header_status == CONTINUE && request.get_m_raw_header() != "") //body 읽어야 함
				{
					body_status = request.getBody(sockfd);
					if (body_status == FAIL)
					{
						ft::console_log("GET BODY | FAIL");
						close(sockfd);
						ft::fdClr(sockfd, m_main_fds);
						m_fd_table.erase(fd_iter);
						*m_maxfd = findMaxFd();
						return (false);
					}
					if (body_status == SUCCESS && request.get_m_check_cgi() == true)
					{
						ft::console_log("----- set cgi fds -----");
						// ft::fdSet(request.get_m_cgi_stdin(), m_main_fds);
						ft::fdSet(request.get_m_cgi_stdout(), m_write_fds);
						// m_fd_table.push_back(ft::makeFDT(CGI_PIPE, request.get_m_cgi_stdin(), sockfd));
						// m_fd_table.push_back(ft::makeFDT(CGI_PIPE, request.get_m_cgi_stdout(), sockfd));
						// *m_maxfd = findMaxFd();
					}
					else if (body_status == SUCCESS)
					{
						ft::console_log("----- handle request -----");
						handleRequest(sockfd);
						if (this->m_responses[sockfd].get_m_status_code() != 0)
						{
							std::cout << "STATUS CODE: " << this->m_responses[sockfd].get_m_status_code() << std::endl;;
							std::cout << "SET WRITE FD :D" << std::endl;
							ft::fdSet(sockfd, m_write_fds);
						}
					}
					ft::console_log("m_check_cgi " + std::to_string(request.get_m_check_cgi()));
				}
			}
			return (false);
		}
	}
	return (false);
}

bool
Server::writeProcess()
{
	std::vector<FDT>::const_iterator fd_iter;

	for (fd_iter = m_fd_table.begin() ; fd_iter != m_fd_table.end() ; ++fd_iter)
	{
		int sockfd = fd_iter->sockfd;
		if (ft::fdIsSet(sockfd, this->m_copy_write_fds))
		{
			if (fd_iter->type == CGI_PIPE)
			{
				ft::console_log(":::::::::::::::::::::::::::::::::::::::::::::::2::");

				Request &request = this->m_requests[fd_iter->clientfd];
				const std::string &body = request.get_m_body();
				// int content_length = body.size();
				// int written_bytes = request.get_m_written_bytes();
				// int buffsize = std::min(SOCK_BUFF, content_length - written_bytes);
				int buffsize = std::min(SOCK_BUFF, static_cast<int>(body.size())); // body size가 int 넘어갈 경우 위험
				size_t ret = 0;
				static int tmp = 0;

				if (buffsize > 0 && (ret = write(sockfd, body.c_str(), buffsize)) > 0) //buffsize 0으로 write 하면 ret이 size_t max
				{
					tmp += ret;
					ft::console_log("++++++ WRITE PROCESS(pipe): " + std::to_string(tmp));
					ft::console_log("CGI body length 1: " + std::to_string(request.get_m_body().length()));
					if (ret == body.length())
					{
						ft::console_log("here clear body ==================");
						request.clearBody();
					}
					else
						request.set_m_body(body.substr(ret, std::string::npos));
					ft::console_log("CGI body length 2: " + std::to_string(request.get_m_body().length()));
					// ft::fdSet(request.get_m_cgi_stdin(), m_main_fds);
				}
				// if ((ret = write(sockfd, &body.c_str()[written_bytes], buffsize)) > 0)
				// {
				// 	written_bytes += ret;
				// 	request.set_m_written_bytes(written_bytes);
				// 	std::cout << "WRITE PROCESS) WRITTEN BYTES: " << written_bytes << std::endl;
				// 	buffsize = std::min(SOCK_BUFF, content_length - written_bytes);
				// 	ft::fdClr(sockfd, m_write_fds);
				// 	return (false);
				// }
				else if (ret < 0)
				{
					std::cout << "ERRNO IS " << errno << std::endl;
					std::cout << "A" << EAGAIN << std::endl;
					std::cout << "B" << EWOULDBLOCK << std::endl;
					std::cout << "C" << EBADF << std::endl;
					std::cout << "D" << EDESTADDRREQ << std::endl;
					std::cout << "E" << EDQUOT << std::endl;
					std::cout << "F" << EFAULT << std::endl;
					std::cout << "G" << EFBIG << std::endl;
					std::cout << "H" << EINTR << std::endl;
					std::cout << "I" << EINVAL << std::endl;
					std::cout << "J" << EIO << std::endl;
					std::cout << "K" << ENOSPC << std::endl;
					std::cout << "L" << EPERM  << std::endl;
					std::cout << "M" << EPIPE << std::endl;
				}
				if (buffsize == 0 && ret == 0 && (request.get_m_read_end() == true))
				{
					ft::console_log("RET IS 0");
					ft::console_log("RET IS 0 BODY _____ ");
					write(request.get_m_check_fd(), "end", 3);
					ft::fdSet(request.get_m_cgi_stdin(), m_main_fds);
					m_responses[fd_iter->clientfd].set_m_cgi_chunked_read_end(true);
					ft::fdClr(sockfd, this->m_write_fds);
					close(sockfd);
					this->m_fd_table.erase(fd_iter);
					*m_maxfd = findMaxFd();
					return (true);
				}
				else if (ret < 0)
				{
					ft::fdClr(sockfd, this->m_write_fds);
					close(sockfd);
					this->m_fd_table.erase(fd_iter);
					*m_maxfd = findMaxFd();
					return (false);
				}


			}
			else if(fd_iter->type == C_SOCKET)
			{
				ft::console_log(":::::::::::::::::::::::::::::::::::::::::::::::4::");
				int ret = 0;
				int buffsize;
				// Request &request = m_requests[sockfd];
				Response &response = m_responses[sockfd];
				int pos = response.get_m_pos();

				if (response.get_m_has_cgi_response() == true)
				{
					std::string &header = response.get_m_cgi_header();
					if (header != "")
					{
						ft::console_log("---- write cgi response header ----");
						ret = write(sockfd, header.c_str(), header.size());
						ft::console_log("finish header: " + header);
						header = "";
						if (ret > 0)
							return (false);
					}
					else
					{
						const std::string &body = response.get_m_cgi_response();
						int content_length = body.size();
						// buffsize = std::min(content_length - pos, 32768); //chunked writingd
						buffsize = std::min(content_length - pos, GAEBOKCHI); // mess writig
						if (buffsize == 0 && response.get_m_cgi_chunked_read_end() == false)
						{
							ft::fdClr(sockfd, this->m_write_fds);
							return(false);
						}
						std::string num;
						ft::itoa(buffsize, num, 16);
						std::string buff = (num + "\r\n"+ body.substr(pos, buffsize) + "\r\n");
						ret = write(sockfd, buff.c_str(), buff.size());
						ft::console_log("++++++ WRITE PROCESS(sock) | num : " + num);
						ft::console_log("++++++ WRITE PROCESS(sock) | pos : " + std::to_string(pos));
						ft::console_log("++++++ WRITE PROCESS(sock) | buf : " + std::to_string(buffsize));
						response.set_m_pos(pos + buffsize);
						if (buffsize == 0)
						{
							ft::console_log("------ END ");
							close(m_requests[sockfd].get_m_cgi_stdout());
							unlink("/tmp/.tmptext1");
							unlink("/tmp/.tmptext2");
							this->m_requests[sockfd] = Request();
							this->m_responses[sockfd] = Response();
							ft::fdClr(sockfd, this->m_write_fds);

							return (true);
						}
						ft::console_log("finish body: \n");
					}
				}
				else
				{
					const std::string &body = response.get_m_reponse_message();
					int content_length = body.size();
					buffsize = std::min(content_length - pos, SOCK_BUFF);
					if ((ret = write(sockfd, &(body.c_str()[pos]), buffsize)) > 0)
					{
						// this->m_requests[sockfd] = Request(); // 안해줘도 되지 않나
						response.set_m_pos(pos + ret);
						ft::console_log("------ OK " + std::to_string(pos));
						return (false);
					}
				}
				if (ret < 0)
				{
					std::cout << "------ FAIL" << pos << std::endl;
					std::cout << "ERRNO IS " << errno << std::endl;
					std::cout << "X" << EACCES << std::endl;
					std::cout << "A" << EAGAIN << std::endl;
					std::cout << "B" << EALREADY << std::endl;
					std::cout << "C" << EBADF << std::endl;
					std::cout << "D" << ECONNRESET << std::endl;
					std::cout << "E" << EFAULT  << std::endl;
					std::cout << "F" << EINTR << std::endl;
					std::cout << "G" << EINVAL << std::endl;
					std::cout << "H" << EISCONN << std::endl;
					std::cout << "I" << EMSGSIZE << std::endl;
					std::cout << "J" << ENOBUFS << std::endl;
					std::cout << "K" << ENOMEM << std::endl;
					std::cout << "L" << ENOTCONN << std::endl;
					std::cout << "M" << ENOTSOCK << std::endl;
					std::cout << "N" << EOPNOTSUPP << std::endl;
					std::cout << "O" << EPIPE << std::endl;
					std::cout << "P" << EDESTADDRREQ << std::endl;
				}


				if (ret <= 0)
				{
					static int youpiget = 0;
					ft::console_log("------ END " + std::to_string(pos) + ":=================::" + std::to_string(youpiget), 1);
					youpiget++;
					this->m_requests[sockfd] = Request();
					this->m_responses[sockfd] = Response();
					ft::fdClr(sockfd, m_write_fds);
				}
				writeLog("response", m_responses[sockfd], Request(), WRITE_LOG);
			}
			return (false);
		}
	}
	return (false);
}

void
Server::getRequest(fd_set *main_fds, fd_set *read_fds, fd_set *copy_write_fds, fd_set *write_fds, int *maxfd)
{
	this->m_main_fds = main_fds;
	this->m_read_fds = read_fds;
	this->m_copy_write_fds = copy_write_fds;
	this->m_write_fds = write_fds;
	this->m_maxfd = maxfd;

	if (writeProcess() == true)
		return;
	if (ft::fdIsSet(this->m_server_socket, this->m_read_fds))
	{
		acceptSocket();
		return ;
	}
	if (readProcess() == true)
		return;
	return ;
}

/*============================================================================*/
/*******************************  Server Util  ********************************/
/*============================================================================*/

std::vector<HttpConfigLocation>
Server::getMethodLocation(Method method)
{
	switch(method)
	{
		case GET:
			return (m_get_location);
		case HEAD:
			return (m_head_location);
		case POST:
			return (m_post_location);
		case PUT:
			return (m_put_location);
		case DELETE:
			return (m_delete_location);
		case OPTIONS:
			return (m_options_location);
		case TRACE:
			return (m_trace_location);
		default:
			return (std::vector<HttpConfigLocation>());
	}
	return (std::vector<HttpConfigLocation>());
}

bool
Server::isMatchingLocation(std::string location, std::string path_in, std::string *rest)
{
	try
	{
		if (path_in.compare(0, location.size(), location) != 0)
			return (false);
		*rest = path_in.substr(location.size(), std::string::npos);
		if (location == "/")
			*rest = "/" + *rest;
		if ((*rest).size() && (*rest)[0] != '/')
			return (false);
	}
	catch(const std::exception& e)
	{
		return (false);
	}
	return (true);
}

std::string
Server::checkHttpConfigFilePath(std::string path_in)
{
	std::vector<std::string>::const_iterator it;
	std::string rest;
	std::string ret("");

	for(it = m_http_config_file_path_set.begin(); it != m_http_config_file_path_set.end(); it++)
	{
		if (isMatchingLocation(*it, path_in, &rest) == true)
		{
			if (ret.size() < (*it).size())
				ret = *it;
		}
	}
	return (ret);
}

void
Server::resetRequest(Request *req)
{
	std::vector<HttpConfigLocation> loc_block = getMethodLocation(req->get_m_method());
	std::vector<HttpConfigLocation>::const_iterator it;
	HttpConfigLocation block;
	std::string path_in = req->get_m_uri().get_m_path();
	std::string path_loc("");
	std::string path_out("");
	std::string tmp;
	std::string rest;

	if (loc_block.empty())
	{
		req->set_m_error_code(405);
		return;
	}
	for (it = loc_block.begin(); it != loc_block.end(); ++it)
	{
		tmp = it->get_m_path();
		if (isMatchingLocation(tmp, path_in, &rest) == false)
			continue;
		if (path_loc.size() < tmp.size())
		{
			path_loc = tmp;
			path_out = it->get_m_root() + rest;
			block = *it;
		}
	}
	if (checkHttpConfigFilePath(path_in) != path_loc)
	{
		req->set_m_error_code(405);
		return;
	}
	if (path_out.find("/.") != std::string::npos)
		req->set_m_error_code(404);
	req->set_m_reset_path(path_out);
	req->set_m_location_block(block);
	if (block.get_m_limit_body_size() < req->get_m_content_length())
	{
		req->set_m_error_code(413);
		return ;
	}
	req->set_m_check_cgi(ft::checkValidFileExtension(path_out, block.get_m_cgi()));


	/* 인증파트 */
	if (block.get_m_auth_basic().empty() == false) // 인증이 필요한 블럭일 때
	{
		std::map<std::string, std::string> headers = req->get_m_headers();
		std::map<std::string, std::string>::const_iterator header_it = headers.find("authorization");
		if (header_it == headers.end()) // 인증 헤더가 없으면 401 에러
		{
			req->set_m_error_code(401);
			return ;
		}
		std::vector<std::string> auth_value = ft::split((*header_it).second, ' ');
		if (checkAuth(auth_value.back(), block.get_m_auth_basic(), block.get_m_root() + std::string("/") + block.get_m_auth_basic_user_file()) == false) // 인증 실패 시 403 에러
		{
			std::cout << "--=-==---=-=-="<<block.get_m_root() + std::string("/") + block.get_m_auth_basic_user_file()<<std::endl;
			req->set_m_error_code(403);
			return ;
		}
	}
	/* 인증파트 끝 */
	writeLog("request", Response(), *req, WRITE_LOG);
}

void
Server::writeLog(std::string type, Response res, Request req, bool work)
{
	if (!work)
		return ;
	int fd;
	static int start = 1;
	static int cnt_request = 0;
	static int cnt_response = 0;
	std::string path;

	path += this->m_root
		+ std::string("/debug_")
		+ this->m_server_name.substr(11)
		+ std::string(".log");
	if ((fd = open(path.c_str(), O_CREAT|O_RDWR|O_APPEND, S_IRWXU)) < 0)
		throw std::exception();
	if (start)
	{
		ft::putstr_fd("SERVER NAME : ", fd);
		ft::putstr_fd(this->m_server_name.c_str(), fd);
		ft::putstr_fd("(port : ", fd);
		ft::putnbr_fd(this->m_port, fd);
		ft::putendl_fd(")", fd);
		ft::putstr_fd("SERVER START : ", fd);
		ft::putendl_fd(ft::getDateTimestamp(0, 0, 0).c_str(), fd);
		start--;
	}
	if (type.compare("request") == 0)
	{
		ft::putstr_fd("\n---------- REQUEST MESSAGE [", fd);
		ft::putnbr_fd(++cnt_request, fd);
		ft::putstr_fd("] [ ", fd);
		ft::putstr_fd(ft::getDateTimestamp(0, 0, 0).c_str(), fd);
		ft::putendl_fd(" ] ----------", fd);
		ft::putendl_fd(req.get_m_message().c_str(), fd);
		ft::putendl_fd("---------------------------------------------------------------------------",fd);
	}
	else if (type.compare("response") == 0)
	{
		ft::putstr_fd("\n########## RESPONSE MESSAGE [", fd);
		ft::putnbr_fd(++cnt_response, fd);
		ft::putstr_fd("] [ ", fd);
		ft::putstr_fd(ft::getDateTimestamp(0, 0, 0).c_str(), fd);
		ft::putendl_fd("] ##########", fd);
		ft::putendl_fd(res.get_m_reponse_message().c_str(), fd);
		ft::putendl_fd("###########################################################################",fd);
	}
	close(fd);
}

bool
Server::checkAuth(std::string auth_value, std::vector<std::string> auth_basic, std::string auth_file_path)
{
	// client가 보낸 auth 값 유효성 체크, id/pwd로 나눠놓기
	std::string auth_value_decode = ft::decode(auth_value);
	if (auth_value_decode.find(":") == std::string::npos)
		return (false);
	std::vector<std::string> idpwd_client = ft::split(auth_value_decode, ':');
	std::string id_client = idpwd_client.front();
	idpwd_client.erase(idpwd_client.begin());
	std::string pwd_client = idpwd_client.front();

	// client가 보낸 아이디가 config파일의 auth_basic과 일치하지 않으면 에러
	bool is_auth_basic_match = false;
	std::vector<std::string>::const_iterator auth_basic_it;
	for (auth_basic_it = auth_basic.begin() ; auth_basic_it != auth_basic.end() ; ++auth_basic_it)
	{
		if ((*auth_basic_it).compare(id_client) == 0)
		{
			is_auth_basic_match = true;
			break ;
		}
	}
	if (is_auth_basic_match == false)
		return (false);
	// 인증파일 존재하지 않으면 에러
	if (ft::isValidFilePath(auth_file_path) == false)
		return (false);

	// .htpasswd 파일 순회하며 client id, pwd와 비교, 일치하면 인증성공
	std::vector<std::string> idpwds = ft::split(ft::fileToString(auth_file_path), '\n');
	std::vector<std::string>::const_iterator idpwds_it;
	for (idpwds_it = idpwds.begin() ; idpwds_it != idpwds.end() ; ++idpwds_it)
	{
		std::string decode = ft::decode(*idpwds_it);
		if (decode.find(":") == std::string::npos)
			return (false);
		std::vector<std::string> idpwd_server = ft::split(decode, ':');
		std::string id_server = idpwd_server.front();
		idpwd_server.erase(idpwd_server.begin());
		std::string pwd_server = idpwd_server.front();
		if ((id_server.compare(id_client) == 0) && (pwd_server.compare(pwd_client) == 0))
		{
			std::cout << "=-=-=-=-=-=-=-=-=-"<<this->m_auth_id << this->m_auth_pw << std::endl;
			this->m_auth_id = id_client;
			this->m_auth_pw = pwd_client;
			return (true);
		}
	}
	return (false);
}

/*============================================================================*/
/*********************************  HEAD  *************************************/
/*============================================================================*/

Response
Server::methodHEAD(int clientfd, std::string method)
{
	return (methodGET(clientfd, method));
}

/*============================================================================*/
/**********************************  GET  *************************************/
/*============================================================================*/

std::string
Server::getMimeType(std::string extension)
{
	std::map<std::string, std::string>::const_iterator it;
	it = this->m_mime_types.find(extension);
	if (it == m_mime_types.end())
		return (this->m_default_type);
	if ((it->first.compare("css") == 0) || (it->first.compare("js") == 0))
		return (it->second + std::string("; charset=utf-8"));
	return (it->second);
}

std::string
Server::makeAutoindexPage(std::string root, std::string path)
{
	std::string page;
	std::string dir_name(path.substr(root.length() + 1, std::string::npos));
	struct dirent *entry;
	std::vector<std::string> entry_dir;
	std::vector<std::string> entry_file;
	DIR *dirptr = opendir(path.c_str());
	struct stat buffer;
	struct tm *tm;
	char timestamp[64];
	std::string bytes;

	for (entry = readdir(dirptr) ; entry ; entry = readdir(dirptr))
	{
		if (std::string(entry->d_name).compare("..") == 0)
		 	entry_dir.push_back(std::string(entry->d_name));
		if (std::string(entry->d_name).substr(0, 1) == ".")
			continue ;
		if (ft::isValidDirPath(path + std::string(entry->d_name)))
			entry_dir.push_back(std::string(entry->d_name));
		if (ft::isValidFilePath(path + std::string(entry->d_name)))
			entry_file.push_back(std::string(entry->d_name));
	}
	std::sort(entry_dir.begin(), entry_dir.end());
	std::sort(entry_file.begin(), entry_file.end());
	page += std::string("<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n")
			+ std::string("<style>\ntable{border-collapse:collapse;width:100%;font-family:Monaco;}\nth,td{text-align:left;padding:4px;}\ntr:not(:first-child):hover{background-color:#f5f5f5;}\n</style>\n")
			+ std::string("<title>Index of /")
			+ std::string(dir_name)
			+ std::string("</title>\n</head>\n<body>\n<h1>Directory Listing</h1>\n<h2>Index of /")
			+ std::string(dir_name)
			+ std::string("</h2>\n<hr>\n");
	if (dir_name[0] != '/')
		dir_name = std::string("/") + dir_name;
	page += std::string("<table>\n<tr><th width=\"50%\">name</th><th>last modified</th><th style=\"text-align:right;\">size</th></tr>\n");
	for (std::vector<std::string>::const_iterator it = entry_dir.begin() ; it != entry_dir.end() ; ++it)
	{
		std::string dash("-");
		stat((root + dir_name + *it).c_str(), &buffer);
		tm = localtime(&buffer.st_atimespec.tv_sec);
		ft::memset(reinterpret_cast<void *>(timestamp), 0, 64);
		strftime(timestamp, sizeof(timestamp), "%d-%b-%Y %H:%M", tm);
		if ((*it).compare("..") == 0)
		{
			dash = "";
			ft::memset(reinterpret_cast<void *>(timestamp), 0, 64);
		}
		page += std::string("<tr><td><a href=\"")
				+ std::string(dir_name + *it)
				+ std::string("\">")
				+ std::string(*it)
				+ std::string("/</a></td><td>")
				+ std::string(timestamp)
				+ std::string("</td><td style=\"text-align:right;\">")
				+ dash
				+ std::string("</td></tr>\n");
	}
	for (std::vector<std::string>::const_iterator it = entry_file.begin() ; it != entry_file.end() ; ++it)
	{
		stat((root + dir_name + *it).c_str(), &buffer);
		tm = localtime(&buffer.st_mtimespec.tv_sec);
		ft::memset(reinterpret_cast<void *>(timestamp), 0, 64);
		strftime(timestamp, sizeof(timestamp), "%d-%b-%Y %H:%M", tm);
		bytes = std::to_string(buffer.st_size);
		if (bytes.size() >= 5)
			bytes = std::to_string(buffer.st_size / 1024) + std::string("K");
		page += std::string("<tr><td><a href=\"")
				+ std::string(dir_name + *it)
				+ std::string("\">")
				+ std::string(*it)
				+ std::string("</a></td><td>")
				+ std::string(timestamp)
				+ std::string("</td><td style=\"text-align:right;\">")
				+ bytes
				+ std::string("</td></tr>\n");
	}
	page += std::string("</table>\n<hr>\n<i>")
			+ std::string(this->m_server_name)
			+ std::string(" (port ")
			+ std::string(std::to_string(this->m_port))
			+ std::string(")</i>\n</body>\n</html>\n");
	closedir(dirptr);
	return (page);
}

// bool
// Server::checkHttpConfigFilePathHead(std::string path)
// {
// 	int path_length = path.length();
// 	std::vector<HttpConfigLocation>::const_iterator headlocation_iter = this->m_head_location.begin();
// 	while (headlocation_iter != this->m_head_location.end())
// 	{
// 		int headlocation_length = headlocation_iter->get_m_path().length();
// 		if (path == headlocation_iter->get_m_path())
// 			return (true);
// 		if (path_length > headlocation_length
// 		&& headlocation_length > 1
// 		&& path.substr(0, headlocation_length) == headlocation_iter->get_m_path())
// 			return (true);
// 		headlocation_iter++;
// 	}
// 	return (false);
// }

// bool
// Server::checkHttpConfigFilePath(std::string path, std::string method)
// {
// 	int path_length = path.length();
// 	if (method == "HEAD")
// 	{
// 		if (checkHttpConfigFilePathHead(path))
// 			return (false);
// 		return (true);
// 	}
// 	std::vector<std::string>::const_iterator path_iter = this->m_http_config_file_path_set.begin();
// 	while (path_iter != this->m_http_config_file_path_set.end())
// 	{
// 		int path_iter_length = (*path_iter).length();
// 		if (path == *path_iter)
// 			return (false);
// 		if (path_length > path_iter_length
// 		&& path_iter_length > 1
// 		&& path.substr(0, path_iter_length) == *path_iter)
// 			return (false);
// 		path_iter++;
// 	}
// 	return (true);
// }

Response
Server::methodGET(int clientfd, std::string method)
{
	Request &req(this->m_requests[clientfd]);
	HttpConfigLocation location_block(req.get_m_location_block());
	std::string abs_path(req.get_m_reset_path());
	std::string final_path;
	std::string extension;

	if (ft::isValidDirPath(abs_path)) // 폴더라면
	{
		if (abs_path.find("/", abs_path.length() - 1) == std::string::npos)
			abs_path = abs_path + std::string("/");
		if (location_block.get_m_index().empty() == false) // index 벡터가 존재하면
		{
			std::vector<std::string> index = location_block.get_m_index();
			std::vector<std::string>::const_iterator index_it;
			for (index_it = index.begin() ; index_it != index.end() ; ++index_it) // index 벡터 순회
			{
				if (ft::isValidFilePath(abs_path + *index_it)) // 만약 유효한 파일이면
				{
					extension = (*index_it).substr((*index_it).find_last_of(".") + 1, std::string::npos);
					if ((req.getAcceptLanguage().compare("en") == 0) || (req.getAcceptLanguage().compare("en-US") == 0)) // 영문 페이지 요청의 경우 path 수정
						final_path = abs_path + std::string("en/") + *index_it;
					else
						final_path = abs_path + *index_it;
					return (Server::makeResponseMessage(200, this->m_server_name, final_path, "", req.getAcceptLanguage(), method, getMimeType(extension), req.getReferer())); // 200 응답과 반환
				}
			}
		}
		std::map<std::string, bool>::const_iterator autoindex_it;
		for (autoindex_it = this->m_get_location_auto_index.begin() ; autoindex_it != this->m_get_location_auto_index.end() ; ++autoindex_it) // index 를 못찾은 경우
		{
			if (location_block.get_m_path().compare((*autoindex_it).first) == 0 && (*autoindex_it).second == true) // location 블록의 autoindex 값이 on인 경우 페이지 만들어서 200 응답과 반환
				return (Server::makeResponseBodyMessage(200, this->m_server_name, makeAutoindexPage(location_block.get_m_root(), abs_path), "", req.getAcceptLanguage(), method, getMimeType("html"), req.getReferer()));
		}
	}
	else // 폴더가 아니라면
	{
		if (ft::isValidFilePath(abs_path)) // 유효한 파일이면
		{
			extension = abs_path.substr(abs_path.find_last_of(".") + 1, std::string::npos);
			if ((req.getAcceptLanguage().compare("en") == 0) || (req.getAcceptLanguage().compare("en-US") == 0)) // 영문 페이지 요청의 경우 path 수정
			{
				int pos_last_slash = abs_path.find_last_of("/");
				final_path += abs_path.substr(0, pos_last_slash + 1)
							+ std::string("en/")
							+ abs_path.substr(pos_last_slash + 1, std::string::npos);
			}
			else
				final_path = abs_path;
			return (Server::makeResponseMessage(200, this->m_server_name, final_path, "", req.getAcceptLanguage(), method, getMimeType(extension), req.getReferer()));
		}
	}
	return (Server::makeResponseBodyMessage(404, this->m_server_name, makeErrorPage(404), "", req.getAcceptLanguage(), method, getMimeType("html"), req.getReferer()));
}

/*============================================================================*/
/**********************************  POST  ************************************/
/*============================================================================*/

std::map<std::string, std::string>
Server::makeCgiEnvpMap(Request req, Response res)
{
	(void) res;
	std::map<std::string, std::string> map;
	Uri uri = req.get_m_uri();

	map["AUTH_TYPE"] = "BASIC";
	map["CONTENT_LENGTH"] = std::to_string(req.get_m_content_length());
	map["CONTENT_TYPE"] = getMimeType("html");
	map["GATEWAY_INTERFACE"] = "CGI/1.1";
	map["PATH_INFO"] = req.get_m_uri().get_m_path();
	map["PATH_TRANSLATED"] = req.get_m_path_translated();
	map["QUERY_STRING"] = req.get_m_uri().get_m_query_string();
	map["REMOTE_ADDR"] = req.get_m_uri().get_m_host();
	map["REMOTE_IDENT"] = this->m_auth_pw;
	map["REMOTE_USER"] = this->m_auth_id;
	map["REQUEST_METHOD"] = req.getMethod();
	map["REQUEST_URI"] = req.get_m_uri().get_m_path();
	map["SCRIPT_NAME"] = req.get_m_reset_path();
	map["SERVER_NAME"] = this->m_server_name.substr(11);
	map["SERVER_PORT"] = std::to_string(this->m_port);
	map["SERVER_PROTOCOL"] = req.get_m_http_version();
	map["SERVER_SOFTWARE"] = this->m_server_name;
	map["HTTP_X_SECRET_HEADER_FOR_TEST"] = '1';
	map["REDIRECT_STATUS"] = "200";
	map["SCRIPT_FILENAME"] =  req.get_m_reset_path();
	return (map);
}

char **
Server::makeCgiEnvp(Request req, Response res)
{
	std::map<std::string, std::string> env_map = makeCgiEnvpMap(req, res);
	std::map<std::string, std::string>::const_iterator i = env_map.begin();
	char **env = (char **)malloc(sizeof(char *) * (env_map.size() + 1));
	int j;

	if (env == NULL)
		return (NULL);
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

char **
Server::makeCgiArgv(Request req)
{
	char **argv = (char **)malloc(sizeof(char *) * 2);

	if (argv == NULL)
		return (NULL);
	argv[0] = ft::strdup((char *)req.get_m_script_name().c_str());
	argv[1] = (char *)0;
	return (argv);
}

Response
Server::executeCgi(Request &req, Response &res, int clientfd)
{
	/* Setting execve parameters */
	char** envp = Server::makeCgiEnvp(req, res);
	char** argv = Server::makeCgiArgv(req);

	Response response(res);

	int fds2[2];
	pid_t pid;

	if (envp == NULL)
		throw (Server::CgiException());
	else if (argv == NULL)
	{
		ft::doubleFree(envp);
		throw (Server::CgiException());
	}
	else if (pipe(fds2) < 0)
		throw (Server::CgiException());

	/* 최적화 open close 해줘야함. */
	// int parent_write = fds2[1];
	// int cgi_stdin = fds2[0];
	int parent_write = open("/tmp/.tmptext2", O_WRONLY | O_CREAT | O_TRUNC, 0666);
	int cgi_stdin = open("/tmp/.tmptext2", O_RDONLY);
	int parent_read = open("/tmp/.tmptext", O_RDONLY);
	int cgi_stdout = open("/tmp/.tmptext", O_WRONLY | O_CREAT | O_TRUNC, 0666);


	/* set fd nonblock */
	fcntl(cgi_stdin, F_SETFL, O_NONBLOCK);
	fcntl(cgi_stdout, F_SETFL, O_NONBLOCK);
	fcntl(parent_read, F_SETFL, O_NONBLOCK);
	fcntl(parent_write, F_SETFL, O_NONBLOCK);

	std::string extension = req.get_m_reset_path().substr(req.get_m_reset_path().find_last_of(".") + 1, std::string::npos);
	std::cout << "Execute Cgi === "<< req.get_m_path_translated().c_str() << std::endl;
	req.set_m_check_fd(open("/tmp/.check_fd", O_RDWR | O_CREAT | O_TRUNC, 0666));
	pid = fork();

	if (pid == 0) // child process
	{
		struct stat buff;

		close(parent_write);
		while (42)
		{
			int ha;
			ha = stat("/tmp/.check_fd", &buff);
			printf("---- %d %lld\n", ha, buff.st_size);
			sleep(1);
			if (buff.st_size > 0)
			{
				close(req.get_m_check_fd());
				unlink("/tmp/.check_fd");
				break ;
			}
		}
		if (dup2(cgi_stdout, STDOUT_FILENO) < 0)
			throw(Server::CgiException());
		if (dup2(cgi_stdin, STDIN_FILENO) < 0)
			throw (Server::CgiException());

		if (extension.compare("php") == 0)
		{
			char path[1024];
			getcwd(path, 1024);
			std::string php_cgi_path = std::string(path) + std::string("/php-mac/bin/php-cgi");
			if (execve(php_cgi_path.c_str(), argv, envp) < 0)
				throw (Server::CgiException());
		}
		else
		{
			if (execve(req.get_m_path_translated().c_str(), argv, envp) < 0)
				throw (Server::CgiException());
		}
		close(cgi_stdout);
		ft::doubleFree(argv);
		ft::doubleFree(envp);
		exit(EXIT_SUCCESS);
	}
	else  // parent process
	{
		close(cgi_stdin);
		req.set_m_cgi_pid(pid);
		req.set_m_cgi_stdin(parent_read);
		req.set_m_cgi_stdout(parent_write);
		ft::doubleFree(argv);
		ft::doubleFree(envp);
		// ft::fdSet(parent_write, m_write_fds);
		// ft::fdSet(parent_read, m_main_fds);
		m_fd_table.push_back(ft::makeFDT(CGI_PIPE, parent_write, clientfd));
		m_fd_table.push_back(ft::makeFDT(CGI_PIPE, parent_read, clientfd));
		*m_maxfd = findMaxFd();
		// response.get_m_cgi_response().reserve(RESV_SIZE);
	}
	(void)clientfd;
	return (response);
}

Response
Server::postBody(Request req, Response res)
{
	std::string path = req.get_m_uri().get_m_path();

	if (path == "/post_body")
	{
		res = makeResponseBodyMessage(200, this->m_server_name, req.get_m_body());
	}
	return (res);
}

Response
Server::methodPOST(int clientfd, std::string method)
{
	(void)method;
	Response response;
	Request request(m_requests[clientfd]);

	// if (request.checkCGI() == true)
	// {
	// 	try
	// 	{
	// 		response = executeCgi(request, response, clientfd);
	// 	}
	// 	catch(const std::exception& e)
	// 	{
	// 		std::cerr << e.what() << std::endl;
	// 	}
	// }
	// else
	// {
		response = postBody(request, response);
	// }
	return (response);
}

/*============================================================================*/
/**********************************  PUT  *************************************/
/*============================================================================*/

Response
Server::methodPUT(int clientfd, std::string method)
{
	Request &req(this->m_requests[clientfd]);
	std::string path = req.get_m_reset_path();

	int fd;
	const char *body;
	int status_code = 0;

	if (ft::isValidFilePath(path) == false)
	{
		if ((fd = open((path).c_str(), O_RDWR | O_CREAT, 0666)) < 0)
 			return (Server::makeResponseBodyMessage(404, this->m_server_name, "", "", req.getAcceptLanguage(), method, getMimeType("html"), req.getReferer()));
		status_code = 201;
	}
	else
	{
		if ((fd = open((path).c_str(), O_RDWR | O_TRUNC, 0666)) < 0)
			return (Server::makeResponseBodyMessage(404, this->m_server_name, "", "", req.getAcceptLanguage(), method, getMimeType("html"), req.getReferer()));
		status_code = 200;
	}
	body = req.get_m_body().c_str();
	if (write(fd, req.get_m_body().c_str(), ft::strlen(req.get_m_body().c_str())) < 0)
	{
		//close(fd);
		return (Server::makeResponseBodyMessage(404, this->m_server_name, "", "", req.getAcceptLanguage(), method, getMimeType("html"), req.getReferer()));
	}
	else
	{
		//close(fd);
		return (Server::makeResponseMessage(status_code, this->m_server_name, "", "", req.getAcceptLanguage(), method, "", req.getReferer(), 0, 0, 0, "", req.getReferer()));
	}
	return (Server::makeResponseBodyMessage(404, this->m_server_name, "", "", req.getAcceptLanguage(), method, getMimeType("html"), req.getReferer()));
}

/*============================================================================*/
/*********************************  DELETE  ***********************************/
/*============================================================================*/

Response
Server::methodDELETE(int clientfd, std::string method)
{
	Request &req(this->m_requests[clientfd]);
	std::string path = req.get_m_reset_path();

	if (ft::isValidFilePath(path))
	{
		if (unlink(path.c_str()) == 0)
			return (Server::makeResponseMessage(200, this->m_server_name, "./www/index.html", "", this->m_requests[clientfd].getAcceptLanguage(), method, this->m_requests[clientfd].getReferer()));
	}
	return (Server::makeResponseBodyMessage(404, this->m_server_name, makeErrorPage(404), "", this->m_requests[clientfd].getAcceptLanguage(), method, getMimeType("html"), this->m_requests[clientfd].getReferer()));
}

/*============================================================================*/
/*********************************  OPTIONS  **********************************/
/*============================================================================*/

std::string
Server::makeAllowMethod(std::vector<Method> v)
{
	std::string ret("");
	std::vector<Method>::const_iterator i = v.begin();

	while (i != v.end())
	{
		ret += ft::getMethodString(*i) + ", ";
		i++;
	}
	if (i == v.end())
		ret = ret.substr(0, ret.size() - 2);
	return (ret);
}

Response
Server::methodOPTIONS(int clientfd, std::string method)
{
	Response response = Response();
	std::string allow_method("");
	HttpConfigLocation location = m_requests[clientfd].get_m_location_block();

	if (location.get_m_path() == "") // 초기화된 상태 그대로, 맞는 로케이션 블록 못찾았을 때 값
		return (Server::makeResponseBodyMessage(404, this->m_server_name, makeErrorPage(404), "", this->m_requests[clientfd].getAcceptLanguage(), method, getMimeType("html"), this->m_requests[clientfd].getReferer()));
	allow_method = makeAllowMethod(location.get_m_limit_except());
	return (Server::makeResponseBodyMessage(204, this->m_server_name, "",  "", this->m_requests[clientfd].getAcceptLanguage(), method, getMimeType("html"), this->m_requests[clientfd].getReferer(), 0, 0, 0, allow_method));
}


/*============================================================================*/
/**********************************  TRACE  ***********************************/
/*============================================================================*/

Response
Server::methodTRACE(int clientfd, std::string method)
{
	Response response = Response();

	return (
		Server::makeResponseBodyMessage(200, this->m_requests[clientfd].get_m_message(), "", "", method, "message/http", this->m_requests[clientfd].getReferer())
	);
}

/*============================================================================*/
/*****************************  RESPONSE MESSAGE ******************************/
/*============================================================================*/

Response
Server::makeResponseMessage(
	int status_code, std::string server,
	std::string path, std::string transfer_encoding,
	std::string content_language,
	std::string method, std::string content_type,
	std::string referer, int date_hour, int date_minute, int date_second,
	std::string allow_method, std::string content_location, std::string location
)
{
	(void) transfer_encoding;
	Response response = Response();

	if (status_code == 301 || status_code == 503)
		response.setHttpResponseHeader("retry-after", "10");
	if (status_code == 401)
		response.setHttpResponseHeader("WWW-Authenticate", "Basic realm=\"simple\"");
	if ((300 <= status_code && status_code < 400) || status_code == 201)
		response.setHttpResponseHeader("location", location);
	if (allow_method != "")
		response.setHttpResponseHeader("allow", allow_method);
	if (referer != ""
		&& (content_type != "image/gif" || content_type != "image/jpeg"
		|| content_type != "image/png" || content_type != "image/x-icon"))
		response.setHttpResponseHeader("referer", referer);
	if (method == "HEAD")
		response
			.setContentType(content_type)
			.setHttpResponseHeader("content-type", response.get_m_content_type());
	else if (method == "GET")
		response
			.setContentType(content_type)
			.setHttpResponseHeader("last-modified", ft::getDateTimestamp(0, 0, 0))
			.setHttpResponseHeader("content-type", response.get_m_content_type());
	else if (method == "POST")
		response
			.setContentType(content_type)
			.setHttpResponseHeader("content-location", content_location)
			.setHttpResponseHeader("content-type", response.get_m_content_type());
	else if (method == "PUT")
		response.setHttpResponseHeader("content-location", content_location);
	else if (method == "OPTIONS")
		response.setHttpResponseHeader("allow", allow_method);
	else if (method == "TRACE")
		response
			.setContentType(content_type)
			.setHttpResponseHeader("content-type", response.get_m_content_type());
	return (
		response
			.setStatusCode(status_code)
			.setFileDocument(path)
			.setContentLanguage(content_language)
			.setServer(server)
			.setCurrentDate(date_hour, date_minute, date_second)
			.setHttpResponseHeader("date", response.get_m_date())
			.setHttpResponseHeader("content-length", std::to_string(response.get_m_content_length()))
			.setHttpResponseHeader("content-language", response.get_m_content_language())
			.setHttpResponseHeader("status", std::to_string(response.get_m_status_code()))
			.setHttpResponseHeader("server", response.get_m_server())
			.makeHttpResponseMessage(method)
	);
}

Response
Server::makeResponseBodyMessage(
	int status_code, std::string server,
	std::string body, std::string transfer_encoding,
	std::string content_language,
	std::string method, std::string content_type,
	std::string referer, int date_hour, int date_minute, int date_second,
	std::string allow_method, std::string content_location, std::string location
)
{
	(void) transfer_encoding;
	Response response = Response();

	if (status_code == 301 || status_code == 503)
		response.setHttpResponseHeader("retry-after", "10");
	if (status_code == 401)
		response.setHttpResponseHeader("WWW-Authenticate", "Basic realm=\"simple\"");
	if ((300 <= status_code && status_code < 400) || status_code == 201)
		response.setHttpResponseHeader("location", location);
	if (allow_method != "")
		response.setHttpResponseHeader("allow", allow_method);
	if (referer != ""
		&& (content_type != "image/gif" || content_type != "image/jpeg"
		|| content_type != "image/png" || content_type != "image/x-icon"))
		response.setHttpResponseHeader("referer", referer);
	if (method == "HEAD")
		response
			.setContentType(content_type)
			.setHttpResponseHeader("content-type", response.get_m_content_type());
	else if (method == "GET")
		response
			.setContentType(content_type)
			.setHttpResponseHeader("last-modified", ft::getDateTimestamp(0, 0, 0))
			.setHttpResponseHeader("content-type", response.get_m_content_type());
	else if (method == "POST")
		response
			.setContentType(content_type)
			.setHttpResponseHeader("content-location", content_location)
			.setHttpResponseHeader("content-type", response.get_m_content_type());
	else if (method == "PUT")
		response.setHttpResponseHeader("content-location", content_location);
	else if (method == "OPTIONS")
		response.setHttpResponseHeader("allow", allow_method);
	else if (method == "TRACE")
		response
			.setContentType(content_type)
			.setHttpResponseHeader("content-type", response.get_m_content_type());
	return (
		response
			.setStatusCode(status_code)
			.setContentLanguage(content_language)
			.setServer(server)
			.setBodyDocument(body)
			.setCurrentDate(date_hour, date_minute, date_second)
			.setHttpResponseHeader("date", response.get_m_date())
			.setHttpResponseHeader("content-length", std::to_string(response.get_m_content_length()))
			.setHttpResponseHeader("content-language", response.get_m_content_language())
			.setHttpResponseHeader("status", std::to_string(response.get_m_status_code()))
			.setHttpResponseHeader("server", response.get_m_server())
			.makeHttpResponseMessage(method)
	);
}

std::string
Server::makeErrorPage(int status_code)
{
	std::string page;

	if (this->m_server_block.get_m_default_error_page().empty() == false)
	{
		std::string path(this->m_root + this->m_server_block.get_m_default_error_page());
		if (ft::isValidFilePath(path))
			return (ft::fileToString(path));
	}
	page += std::string("<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n<title>")
			+ std::string(std::to_string(status_code))
			+ std::string(" Error</title>\n</head>\n<body>\n<center>\n<h1>")
			+ std::string(std::to_string(status_code))
			+ std::string("</h1>\n<h3>")
			+ ft::getErrorMessage(status_code)
			+ std::string("</h3>\n<p>The server encountered an unexpected condition that prevented it from fulfilling the request.<br>\n")
			+ std::string("We are sorry for the inconvenience.</p>\n<hr>\n<i>")
			+ std::string(m_server_name)
			+ std::string(" (port ")
			+ std::string(std::to_string(m_port))
			+ std::string(")</i>\n</center>\n</body>\n</html>\n");
	return (page);
}

/*============================================================================*/
/***************************  SERVER METHOD UTIL  *****************************/
/*============================================================================*/

Response
Server::parseErrorResponse(int clientfd)
{
	Request &request = this->m_requests[clientfd];
	int status_code(request.get_m_error_code());
	if (status_code == 405)
	{
		std::map<std::string, std::string>::const_iterator it;
		std::string allow_method("");
		it = m_http_config_path_method.find(request.get_m_uri().get_m_path());
		if (it != m_http_config_path_method.end())
			allow_method = it->second;
		return (Server::makeResponseBodyMessage(status_code, this->m_server_name, "", "", request.getAcceptLanguage(),
				ft::getMethodString(request.get_m_method()), getMimeType("html"), request.getReferer(), 0, 0, 0, allow_method));
	}
	return (
		Server::makeResponseBodyMessage(
			status_code, this->m_server_name, "", "", request.getAcceptLanguage(),
			ft::getMethodString(request.get_m_method()), getMimeType("html"), request.getReferer()
			)
	);
}

Response
Server::checkValidRequestHeader(int clientfd)
{
	/* If request has no Host Header */
	if (this->m_requests[clientfd].get_m_uri().get_m_host() == "")
		return (Server::makeResponseBodyMessage(400, this->m_server_name, makeErrorPage(400), "", this->m_requests[clientfd].getAcceptLanguage(), "GET", getMimeType("html"), this->m_requests[clientfd].getReferer()));
	return (
		Server::makeResponseBodyMessage(400, this->m_server_name, makeErrorPage(400), "", this->m_requests[clientfd].getAcceptLanguage(), "GET", getMimeType("html"), this->m_requests[clientfd].getReferer())
	);
}

Response
Server::post(std::string path, Request req, Response res, Response (*func)(Request req, Response res))
{
	if (path == req.get_m_uri().get_m_path())
		return (func(req, res));
	return (res);
}

/*============================================================================*/
/***************************  Response  ***************************************/
/*============================================================================*/

bool
Server::sendResponse(int clientfd)
{
	(void) clientfd;
	return (true);
}
