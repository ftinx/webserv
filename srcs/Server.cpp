#include "Server.hpp"

#include <bitset>
#include <iostream>

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
: m_fd_table()
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
int content_length, size_t location_size, std::string root, std::map<std::string, std::string> mime_types,
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
	m_fd_table.push_back(ft::makeFDT(C_SOCKET, this->m_client_socket, 0));
	ft::fdSet(this->m_client_socket, this->m_main_fds);
	if (this->m_client_socket > *this->m_maxfd)
		*(this->m_maxfd) = this->m_client_socket;
	std::cerr << "Accept OK" << std::endl;
	return ;
}

void
Server::handleRequest(int clientfd)
{
	Method method = this->m_requests[clientfd].get_m_method();

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
			this->m_responses[clientfd] = Server::makeResponseBodyMessage(405, this->m_server_name, makeErrorPage(405), "", this->m_requests[clientfd].getAcceptLanguage(), "GET", getMimeType("html"), this->m_requests[clientfd].getReferer(), 0, 0, 0, allow_method);
			break;
		}
	}
	if (this->m_responses[clientfd].get_m_status_code() != 0)
	{
		std::cout << "STATUS CODE: " << this->m_responses[clientfd].get_m_status_code() << std::endl;;
		std::cout << "SET WRITE FD :D" << std::endl;
		ft::fdSet(clientfd, m_write_fds);
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
			if (fd_iter->type == CGI_PIPE)
			{
				std::cout << "::3::"<<std::endl;
				// int status;
				int ret;
				int status_code = 0;
				char buff[CGI_BUFF];
				static std::string body("");

				std::cout << "BEFORE WAITING" << std::endl;
				//kill(this->m_requests[fd_iter->clientfd].get_m_cgi_pid(), SIGTERM);
				//waitpid(this->m_requests[fd_iter->clientfd].get_m_cgi_pid(), &status, 0);
				std::cout << "AFTER WAITING" << std::endl;
				ft::memset(buff, 0, CGI_BUFF);
				// if (status == 0)
				// {
					if( 0 < (ret = read(fd_iter->sockfd, buff, CGI_BUFF - 1)))
					{
						std::cout << "RET IS " << ret << std::endl;
						buff[ret] = '\0';
						this->m_responses[fd_iter->clientfd].setCgiResponse(std::string(buff));
						try
						{
						if (status_code == 0 &&
							(status_code = this->m_responses[fd_iter->clientfd].findCgiStatusCode()))
						{
							this->m_responses[fd_iter->clientfd].set_m_status_code(status_code);
						}
						}
						catch (std::exception &e)
						{
							std::cout << e.what() << std::endl;
							return (true);
						}
					}
					std::cout << "READ PROCESS) BODY SIZE: " << this->m_responses[fd_iter->clientfd].get_m_cgi_response().size() << std::endl;
					if (ret  == 0)
					{
						std::cout << "RET IS 0" << std::endl;
						this->m_responses[fd_iter->clientfd]
								.setCgiContentLength()
								.makeCgiHttpResponseMessage();
						close(fd_iter->sockfd);
						ft::fdClr(fd_iter->sockfd, m_main_fds);
						ft::fdSet(fd_iter->clientfd, m_write_fds);
						this->m_fd_table.erase(fd_iter);
						*m_maxfd = findMaxFd();
						body = "";
						return (true);
					}
				// }
			}
			else if(fd_iter->type == C_SOCKET)
			{
				int ret;

				std::cout << "::1:: " << sockfd << " ::" <<std::endl;
				if ((ret = this->m_requests[sockfd].getMessage(sockfd)) == SUCCESS)
				{
					std::cout << "GET MESSAGE SUCCESS " << std::endl;
					resetRequest(&this->m_requests[sockfd]);
					handleRequest(sockfd);
					return (true);
				}
				else if (ret == FAIL)
				{
					close(sockfd);
					ft::fdClr(sockfd, this->m_main_fds);
					this->m_fd_table.erase(fd_iter);
					*m_maxfd = findMaxFd();
					if (this->m_fd_table.size() <= 0)
						return (false);
					return (true);
				}
				return (true);
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
				std::cout << "::2::"<<std::endl;

				Request &request = this->m_requests[fd_iter->clientfd];
				const std::string &body = request.get_m_body();
				int content_length = request.get_m_content_length();
				int written_bytes = request.get_m_written_bytes();
				int buffsize = std::min(CGI_BUFF, content_length - written_bytes);
				int ret;

				if ((ret = write(sockfd, &body.c_str()[written_bytes], buffsize)) > 0)
				{
					written_bytes += ret;
					request.set_m_written_bytes(written_bytes);
					std::cout << "WRITE PROCESS) BUFF SIZE: " << buffsize << std::endl;
					buffsize = std::min(CGI_BUFF, content_length - written_bytes);
					return (false);
				}
				std::cout << "WRITE PROCESS) ret: " << ret << std::endl;
				if (ret < 0)
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
					return (false);
				}
				std::cout << "WRITE PROCESS) WRITTEN BYTES SIZE: " << written_bytes << std::endl;
				if (ret == 0)
				{
					ft::fdClr(sockfd, this->m_write_fds);
					close(sockfd);
					this->m_fd_table.erase(fd_iter);
					*m_maxfd = findMaxFd();
					return (true);
				}
			}
			else if(fd_iter->type == C_SOCKET)
			{
				std::cout << "::4::"<<std::endl;
				int ret;
				int buffsize;
				static int pos = 0;
				const std::string &body = m_responses[sockfd].get_m_reponse_message();
				int content_length = body.size();

				buffsize = std::min(content_length - pos, SOCK_BUFF);
				if ((ret = write(sockfd, &(body.c_str()[pos]), buffsize)) > 0)
				{
					pos += ret;
					std::cout << "OOOOOOO OK " << pos << std::endl;
					return (false);
				}
				if (ret < 0)
				{
					std::cout << "XXXXXXX FAIL" << pos << std::endl;
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
					std::cout << "OOOOOOO END " << pos << std::endl;
					this->m_requests[sockfd] = Request();
					ft::fdClr(sockfd, this->m_write_fds);
					pos = 0;
				}
				writeLog("response", m_responses[sockfd], Request());
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
	readProcess();
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
	req->set_m_reset_path(path_out);
	req->set_m_location_block(block);
	int pos = block.get_m_limit_body_size();
	req->set_m_body(req->get_m_body().substr(0, pos));
	std::cout << "RESETREQUEST) BODY SIZE: " << req->get_m_body().size() << std::endl;
	writeLog("request", Response(), *req);
}

void
Server::writeLog(std::string type, Response res, Request req)
{
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
			return (true);
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
		return ("none");
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
	std::string absolute_path(req.get_m_reset_path());
	std::string final_path;
	std::string extension;

	if (location_block.get_m_auth_basic().empty() == false) // 인증이 필요한 블럭일 때
	{
		std::map<std::string, std::string> headers = req.get_m_headers();
		std::map<std::string, std::string>::const_iterator header_it = headers.find("Authorization");
		if (header_it == headers.end()) // 인증 헤더가 없으면 401 에러
			return (Server::makeResponseBodyMessage(401, this->m_server_name, "", "", req.getAcceptLanguage(), method, getMimeType("html"), req.getReferer()));
		std::vector<std::string> auth_value = ft::split((*header_it).second, ' ');
		if (checkAuth(auth_value.back(), location_block.get_m_auth_basic(), location_block.get_m_root() + std::string("/") + location_block.get_m_auth_basic_user_file()) == false) // 인증 실패 시 403 에러
			return (Server::makeResponseBodyMessage(403, this->m_server_name, makeErrorPage(403), "", req.getAcceptLanguage(), method, getMimeType("html"), req.getReferer()));
	}
	if (ft::isValidDirPath(absolute_path)) // 폴더라면
	{
		if (absolute_path.find("/", absolute_path.length() - 1) == std::string::npos)
			absolute_path = absolute_path + std::string("/");
		if (location_block.get_m_index().empty() == false) // index 벡터가 존재하면
		{
			std::vector<std::string> index = location_block.get_m_index();
			std::vector<std::string>::const_iterator index_it;
			for (index_it = index.begin() ; index_it != index.end() ; ++index_it) // index 벡터 순회
			{
				if (ft::isValidFilePath(absolute_path + *index_it)) // 만약 유효한 파일이면
				{
					extension = (*index_it).substr((*index_it).find_last_of(".") + 1, std::string::npos);
					if ((req.getAcceptLanguage().compare("en") == 0) || (req.getAcceptLanguage().compare("en-US") == 0)) // 영문 페이지 요청의 경우 path 수정
						final_path = absolute_path + std::string("en/") + *index_it;
					else
						final_path = absolute_path + *index_it;
					return (Server::makeResponseMessage(200, this->m_server_name, final_path, "", req.getAcceptLanguage(), method, getMimeType(extension), req.getReferer())); // 200 응답과 반환
				}
			}
		}
		std::map<std::string, bool>::const_iterator autoindex_it;
		for (autoindex_it = this->m_get_location_auto_index.begin() ; autoindex_it != this->m_get_location_auto_index.end() ; ++autoindex_it) // index 를 못찾은 경우
		{
			if (location_block.get_m_path().compare((*autoindex_it).first) == 0 && (*autoindex_it).second == true) // location 블록의 autoindex 값이 on인 경우 페이지 만들어서 200 응답과 반환
				return (Server::makeResponseBodyMessage(200, this->m_server_name, makeAutoindexPage(location_block.get_m_root(), absolute_path), "", req.getAcceptLanguage(), method, req.getReferer()));
		}
	}
	else // 폴더가 아니라면
	{
		if (ft::isValidFilePath(absolute_path)) // 유효한 파일이면
		{
			if (absolute_path.find("/.", absolute_path.find_last_of("/") - 1, 2) != std::string::npos) // 숨김파일을 요청하면 404 에러
				return (Server::makeResponseBodyMessage(404, this->m_server_name, makeErrorPage(404), "", req.getAcceptLanguage(), method, getMimeType("html"), req.getReferer()));
			extension = absolute_path.substr(absolute_path.find_last_of(".") + 1, std::string::npos);
			if ((req.getAcceptLanguage().compare("en") == 0) || (req.getAcceptLanguage().compare("en-US") == 0)) // 영문 페이지 요청의 경우 path 수정
			{
				int pos_last_slash = absolute_path.find_last_of("/");
				final_path += absolute_path.substr(0, pos_last_slash + 1)
							+ std::string("en/")
							+ absolute_path.substr(pos_last_slash + 1, std::string::npos);
			}
			else
				final_path = absolute_path;
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

	/*
	** auth 관련 AUTH_TYPE REMOTE_USER REMOTE_IDENT
	*/
	map["REQUEST_METHOD"] = req.getMethod();
	map["SERVER_PROTOCOL"] = req.get_m_http_version();
	map["PATH_INFO"] = req.get_m_path_info();
	map["PATH_TRANSLATED"] = req.get_m_path_translated();

	// map["SCRIPT_NAME"] = req.get_m_script_name();

	// map["SERVER_SOFTWARE"] = std::string("ftinx/1.0");
	// map["SERVER_NAME"] = res.get_m_cgi_server_name();
	// map["GATEWAY_INTERFACE"] = "Cgi/1.1";
	// map["SERVER_PORT"] = std::to_string(res.get_m_cgi_port());

	// map["REDIRECT_STATUS"] = "";
	// map["SCRIPT_FILENAME"] = "";
	// map["QUERY_STRING"] = uri.get_m_query_string();
	// map["REMOTE_ADDR"] = ft::iNetNtoA(res.get_m_cgi_client_addr());
	// map["CONTENT_TYPE"] = req.getContentType();
	// map["CONTENT_LENGTH"] = req.getContentLength();
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
		std::cout << env[j] << std::endl;
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
Server::executeCgi(Request req, Response res, int clientfd)
{
	/* Setting execve parameters */
	char** envp = Server::makeCgiEnvp(req, res);
	char** argv = Server::makeCgiArgv(req);

	Response response(res);

	int fds1[2], fds2[2];
	pid_t pid;

	if (envp == NULL)
		throw (Server::CgiException());
	else if (argv == NULL)
	{
		ft::doubleFree(envp);
		throw (Server::CgiException());
	}
	else if (pipe(fds1) < 0 || pipe(fds2) < 0)
		throw (Server::CgiException());

	int parent_write = fds2[1];
	int parent_read = fds1[0];
	int cgi_stdin = fds2[0];
	int cgi_stdout = fds1[1];

	/* set fd nonblock */
	fcntl(cgi_stdin, F_SETFL, O_NONBLOCK);
	fcntl(cgi_stdout, F_SETFL, O_NONBLOCK);
	fcntl(parent_read, F_SETFL, O_NONBLOCK);
	fcntl(parent_write, F_SETFL, O_NONBLOCK);

	printf("\n=========== cgi argv ============\n");
	printf("%s\n", argv[0]);
	printf("\n=========== cgi envp ============\n");
	for (int i = 0; i<3; i++)
	{
		printf("%s\n", envp[i]);
	}
	printf("===================================\n\n");

	std::cout << "Execute Cgi >0<"<< req.get_m_path_translated().c_str() << std::endl;
	pid = fork();

	if (pid == 0) // child process
	{
		req.set_m_cgi_pid(pid);
		close(parent_write);
		close(parent_read);
		if (dup2(cgi_stdin, STDIN_FILENO) < 0 || dup2(cgi_stdout, STDOUT_FILENO) < 0)
			throw (Server::CgiException());
		if (execve(req.get_m_path_translated().c_str(), argv, envp) < 0)
			throw (Server::CgiException());
		exit(EXIT_SUCCESS);
	}
	else  // parent process
	{
		close(cgi_stdout);
		close(cgi_stdin);
		ft::doubleFree(argv);
		ft::doubleFree(envp);
		std::cout << "PARENT WRITE FD " << parent_write << std::endl;
		std::cout << "PARENT READ FD " << parent_read << std::endl;
		m_fd_table.push_back(ft::makeFDT(CGI_PIPE, parent_write, clientfd));
		m_fd_table.push_back(ft::makeFDT(CGI_PIPE, parent_read, clientfd));
		ft::fdSet(parent_write, m_write_fds);
		ft::fdSet(parent_read, m_main_fds);
		*m_maxfd = findMaxFd();
		std::cout << "MAX FD: " << *m_maxfd << std::endl;
	}
	(void)clientfd;
	return (response);
}

// std::map<std::string, std::string>
// Server::parseQuery(std::string str)
// {
// 	std::map<std::string, std::string> m_query;
// 	std::vector<std::string> pieces = ft::split(str, "&");

// 	for (size_t i = 0; i < pieces.size(); i++)
// 	{
// 		std::vector<std::string> queries = ft::split(pieces[i], "=");
// 		m_query.insert(make_pair(ft::trim(queries[0], " \n\t\v\f\r"), ft::trim(queries[1], " \n\t\v\f\r")));
// 	}
// 	return (m_query);
// }

// Response
// Server::postAuth(Request req, Response res)
// {
// 	std::string path = req.get_m_uri().get_m_path();

// 	if (path == "/auth")
// 	{
// 		std::map<std::string, std::string> m_query = Server::parseQuery(req.get_m_body());
// 		std::string username;
// 		std::string password;

// 		if(m_query.find("formType") != m_query.end()
// 		&& m_query.find("formType")->second == "login")
// 		{
// 			username = m_query.find("username")->second;
// 			password = m_query.find("password")->second;

// 			printf("::%s:: ::%s::", username.c_str(), password.c_str());
// 			printf("::%d:: ::%d::", username == "42seoul", password == "42seoul");

// 			if (username == "42seoul" && password == "42seoul")
// 				return (Server::makeResponseMessage(200, "./www/srcs/login.html", ""));
// 			else
// 				return (Server::makeResponseMessage(200, "./www/index.html", ""));
// 		}
// 	}
// 	return (res);
// }

Response
Server::methodPOST(int clientfd, std::string method)
{
	(void)method;
	Response response;
	Request request(m_requests[clientfd]);

	if (request.checkCGI() == true)
	{
		try
		{
			response = executeCgi(request, response, clientfd);
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
	return (response);
}

// Response
// Server::methodPOST(int clientfd, std::string method)
// {
// 	(void) clientfd;
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
		// response = executeCgi(this->m_requests[clientfd], response, method);
	// response = post("/auth", this->m_requests[clientfd], response, Server::postAuth);
	// response = post("/cgi-bin/cgi_tester", this->m_requests[clientfd], response, &this->m_write_fds, Server::executeCgi);

	/* Config File Route */
	// if (this->m_post_location.size() == 0)
	// 	return (response);
	// std::vector<HttpConfigLocation>::const_iterator location_iter = this->m_post_location.begin();
	// while (location_iter != this->m_post_location.end())
	// {
	// 	/* HttpConfig path Response Setting */
	// 	response.set_m_cgi_extension(location_iter->get_m_cgi());
	// 	response.set_m_index_file(location_iter->get_m_index());
	// 	response.set_m_root(location_iter->get_m_root());
	// 	// response.set_m_cgi_path(location_iter->get_m_cgi_path());

	// 	/* CGI */
	// 	if (location_iter->get_m_cgi_path() != "")
	// 		response = post(location_iter->get_m_path(), this->m_requests[clientfd], response, &this->m_write_fds, Server::executeCgi);
	// 	location_iter++;
	// }
// 	return (Server::makeResponseBodyMessage(405, makeErrorPage(405), method));
// }

/*============================================================================*/
/**********************************  PUT  *************************************/
/*============================================================================*/

Response
Server::methodPUT(int clientfd, std::string method)
{
	Request req = this->m_requests[clientfd];
	std::string path = req.get_m_reset_path();

	int fd;
	const char *body;
	int status_code = 0;

	if (ft::isValidFilePath(path) == false)
	{
		if ((fd = open((path).c_str(), O_RDWR | O_CREAT, 0666)) < 0)
 			return (Server::makeResponseBodyMessage(404, this->m_server_name, makeErrorPage(404), "", req.getAcceptLanguage(), method, getMimeType("html"), req.getReferer()));
		status_code = 201;
	}
	else
	{
		if ((fd = open((path).c_str(), O_RDWR | O_TRUNC, 0666)) < 0)
			return (Server::makeResponseBodyMessage(404, this->m_server_name, makeErrorPage(404), "", req.getAcceptLanguage(), method, getMimeType("html"), req.getReferer()));
		status_code = 200;
	}
	body = req.get_m_body().c_str();
	if (write(fd, req.get_m_body().c_str(), ft::strlen(req.get_m_body().c_str())) < 0)
	{
		//close(fd);
		return (Server::makeResponseBodyMessage(404, this->m_server_name, makeErrorPage(404), "", req.getAcceptLanguage(), method, getMimeType("html"), req.getReferer()));
	}
	else
	{
		//close(fd);
		return (Server::makeResponseMessage(status_code, this->m_server_name, req.get_m_reset_path(), "", req.getAcceptLanguage(), method, "", req.getReferer(), 0, 0, 0, "", req.getReferer()));
	}
	return (Server::makeResponseBodyMessage(404, this->m_server_name, makeErrorPage(404), "", req.getAcceptLanguage(), method, getMimeType("html"), req.getReferer()));
}

/*============================================================================*/
/*********************************  DELETE  ***********************************/
/*============================================================================*/

Response
Server::methodDELETE(int clientfd, std::string method)
{
	std::string path = m_requests[clientfd].get_m_reset_path();

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
	int status_code(this->m_requests[clientfd].get_m_error_code());
	if (status_code == 405)
	{
		std::map<std::string, std::string>::const_iterator it;
		std::string allow_method("");
		it = m_http_config_path_method.find(this->m_requests[clientfd].get_m_uri().get_m_path());
		if (it != m_http_config_path_method.end())
			allow_method = it->second;
		return (Server::makeResponseBodyMessage(status_code, this->m_server_name, makeErrorPage(status_code), "", this->m_requests[clientfd].getAcceptLanguage(), "GET", getMimeType("html"), this->m_requests[clientfd].getReferer(), 0, 0, 0, allow_method));
	}
	return (
		Server::makeResponseBodyMessage(status_code, this->m_server_name, makeErrorPage(status_code), "", this->m_requests[clientfd].getAcceptLanguage(), "GET", getMimeType("html"), this->m_requests[clientfd].getReferer())
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
	// std::cout << "\033[47:30m**** response message ****\033[0m" << std::endl;;
	// std::cout << response.get_m_reponse_message() << std::endl;

	/* 아무것도 전송안할순없으니까 0도 포함..? */
	int ret;
	int buffsize;
	static int pos = 0;
	const std::string &body = m_responses[clientfd].get_m_reponse_message();
	int content_length = body.size();

	buffsize = std::min(content_length - pos, SOCK_BUFF);
	while ((ret = write(clientfd, &(body.c_str()[pos]), buffsize)) > 0)
	{
		pos += ret;
		buffsize = std::min(content_length - pos, SOCK_BUFF);
		std::cout << "OOOOOOO OK " << pos << std::endl;
	}
	if (ret < 0)
	{
		std::cout << "XXXXXXX FAIL" << pos << std::endl;
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
		return (false);
	}
	else if (ret == 0)
	{
		std::cout << "OOOOOOO END " << pos << std::endl;
		pos = 0;
	}
	// if ((ret = send(clientfd, m_responses[clientfd].get_m_reponse_message().c_str(), m_responses[clientfd].get_m_response_size(), 0)) < 0)
	// {
	// 	std::cout << "XXXXXXX" << std::endl;
	// 	return (false);
	// }
	// else if (ret == 0)
	// {
	// 	std::cout << "OOOOOOO" << std::endl;
	// }
	writeLog("response", m_responses[clientfd], Request());
	return (true);
}
