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

	/* Request, Response */
	this->m_requests = rhs.m_requests;
	this->m_responses = rhs.m_responses;
	return (*this);
};

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
		m_http_config_file_path_set.push_back(location_iter->get_m_path());
		while (limit != limit_except.end())
		{
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
	m_fd_table.push_back(std::make_pair(C_SOCKET, this->m_client_socket));

	ft::fdSet(this->m_client_socket, this->m_main_fds);
	if (this->m_client_socket > *this->m_maxfd)
		*(this->m_maxfd) = this->m_client_socket;
	std::cerr << "Accept OK" << std::endl;
	return ;
}

void
Server::readProcess()
{
	std::vector< std::pair<FdType, int> >::const_iterator fd_iter;

	for (fd_iter = m_fd_table.begin() ; fd_iter != m_fd_table.end() ; ++fd_iter)
	{
		if (fd_iter->first == CGI_PIPE)
		{

		}
		else if(fd_iter->first == C_SOCKET)
		{
			this->m_sockfd = fd_iter->second;
			if (ft::fdIsSet(this->m_sockfd, this->m_read_fds))
			{
				this->m_requests[this->m_sockfd] = Request();
				if (this->m_requests[this->m_sockfd].getMessage(this->m_sockfd) == false)
				{
					ft::fdClr(this->m_sockfd, this->m_main_fds);
					this->m_fd_table.erase(fd_iter);
					if (this->m_fd_table.size() <= 0)
						break;
				}
				this->resetRequest(&this->m_requests[this->m_sockfd]);
				ft::fdSet(this->m_sockfd, this->m_write_fds);
			}
		}
	}
	return ;
}

void
Server::writeProcess()
{
	std::vector< std::pair<FdType, int> >::const_iterator fd_iter;
	for (fd_iter = m_fd_table.begin() ; fd_iter != m_fd_table.end() ; ++fd_iter)
	{
		if (fd_iter->first == CGI_PIPE)
		{

		}
		else if(fd_iter->first == C_SOCKET)
		{
			int sockfd = fd_iter->second;
			if (ft::fdIsSet(sockfd, this->m_copy_write_fds))
			{
				sendResponse(sockfd);
				FD_CLR(sockfd, this->m_write_fds);
				// FD_CLR(this->sockfd, main_fds);
			}
		}
	}
	return ;
}

void
Server::getRequest(fd_set *main_fds, fd_set *read_fds, fd_set *copy_write_fds, fd_set *write_fds, int *maxfd)
{
	this->m_main_fds = main_fds;
	this->m_read_fds = read_fds;
	this->m_copy_write_fds = copy_write_fds;
	this->m_write_fds = write_fds;
	this->m_maxfd = maxfd;

	writeProcess();
	if (ft::fdIsSet(this->m_server_socket, this->m_read_fds))
		acceptSocket();
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
	writeLog("request", Response());
}

void
Server::writeLog(std::string type, Response response)
{
	int fd;
	static int start = 1;
	static int cnt_request = 1;
	static int cnt_response = 1;
	std::string path;

	path += this->m_root
		+ std::string("/debug_")
		+ this->m_server_name
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
		ft::putnbr_fd(cnt_request++, fd);
		ft::putstr_fd("] [ ", fd);
		ft::putstr_fd(ft::getDateTimestamp(0, 0, 0).c_str(), fd);
		ft::putendl_fd(" ] ----------", fd);
		ft::putendl_fd(this->m_requests[this->m_sockfd].get_m_message().c_str(), fd);
		ft::putendl_fd("---------------------------------------------------------------------------",fd);
	}
	else if (type.compare("response") == 0)
	{
		ft::putstr_fd("\n########## RESPONSE MESSAGE [", fd);
		ft::putnbr_fd(cnt_response++, fd);
		ft::putstr_fd("] [ ", fd);
		ft::putstr_fd(ft::getDateTimestamp(0, 0, 0).c_str(), fd);
		ft::putendl_fd("] ##########", fd);
		ft::putendl_fd(response.get_m_reponse_message().c_str(), fd);
		ft::putendl_fd("###########################################################################",fd);
	}
	close(fd);
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
		return "none";
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

	for (entry = readdir(dirptr) ; entry ; entry = readdir(dirptr))
	{
		if (std::string(entry->d_name).compare(".") == 0)
			continue ;
		if (ft::isValidDirPath(path + std::string(entry->d_name)))
			entry_dir.push_back(std::string(entry->d_name));
		if (ft::isValidFilePath(path + std::string(entry->d_name)))
			entry_file.push_back(std::string(entry->d_name));
	}
	std::sort(entry_dir.begin(), entry_dir.end());
	std::sort(entry_file.begin(), entry_file.end());
	page += std::string("<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n<title>Index of /")
			+ std::string(dir_name)
			+ std::string("</title>\n</head>\n<body>\n<h2>Directory Listing</h2>\n<h3>Index of /")
			+ std::string(dir_name)
			+ std::string("</h3>\n");
	if (dir_name[0] != '/')
		dir_name = std::string("/") + dir_name;
	page += std::string("<hr><p>name</p><hr><table><p>\n");
	for (std::vector<std::string>::const_iterator it = entry_dir.begin() ; it != entry_dir.end() ; ++it)
	{
		page += std::string("<a href=\"")
				+ std::string(dir_name + *it)
				+ std::string("\">")
				+ std::string(*it)
				+ std::string("</a></br>\n");
	}
	for (std::vector<std::string>::const_iterator it = entry_file.begin() ; it != entry_file.end() ; ++it)
	{
		page += std::string("<a href=\"")
				+ std::string(dir_name + *it)
				+ std::string("\">")
				+ std::string(*it)
				+ std::string("</a></br>\n");
	}
	page += std::string("</p><hr>\n<i>")
			+ std::string(this->m_server_name)
			+ std::string(" by ftinx 0.1 port ")
			+ std::string(std::to_string(this->m_port))
			+ std::string("</i>\n</body>\n</html>\n");
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
	Response response;
	std::string absolute_path(this->m_requests[clientfd].get_m_reset_path());
	HttpConfigLocation location_block(this->m_requests[clientfd].get_m_location_block());
	std::string type;
	std::string extension;

	if (ft::isValidDirPath(absolute_path)) // 폴더라면
	{
		if (absolute_path.find("/", absolute_path.length() - 1) == std::string::npos)
			absolute_path = absolute_path + std::string("/");
		std::vector<std::string> index = location_block.get_m_index();
		std::vector<std::string>::const_iterator index_it;
		for (index_it = index.begin() ; index_it != index.end() ; ++index_it) // index 벡터 순회
		{
			if (ft::isValidFilePath(absolute_path + *index_it)) // 만약 유효한 파일이면
			{
				extension = (*index_it).substr((*index_it).find_last_of(".") + 1, std::string::npos);
				type = getMimeType(extension);
				return (Server::makeResponseMessage(200, absolute_path + *index_it, "", method, type)); // 200 응답과 반환
			}
		}
		std::map<std::string, bool>::const_iterator autoindex_it;
		for (autoindex_it = this->m_get_location_auto_index.begin() ; autoindex_it != this->m_get_location_auto_index.end() ; ++autoindex_it)
		{
			if (location_block.get_m_path().compare((*autoindex_it).first) == 0 && (*autoindex_it).second == true)
				return (Server::makeResponseBodyMessage(200, makeAutoindexPage(location_block.get_m_root(), absolute_path), "", method));
		}
	}
	else // 폴더가 아니라면
	{
		if (ft::isValidFilePath(absolute_path))
		{
			extension = absolute_path.substr(absolute_path.find_last_of(".") + 1, std::string::npos);
			type = getMimeType(extension);
			if (type.compare(0, 5, "image") == 0)
			{
				return (Server::makeResponseBodyMessage(200, std::string("data:image/png;base64,") + ft::encode(ft::fileToString(absolute_path)), method, type));
			}
			return (Server::makeResponseMessage(200, absolute_path, "", method, type));
			// if (type == "image/jpeg")
			// 	return Server::makeResponseBodyMessage(200, "iVBORw0KGgoAAAANSUhEUgAAAZAAAAGQCAIAAAAP3aGbAAAJsklEQVR4nOzdvY8cZwHH8Wdfbs+Jz5zPCcZBjiWwiQMkcYFAAiFEoCA0FCQUVDQUiCZSFNHQIF46/gKEaAIFdEhRIiS6AEICOaJwiIXiS5zEjl8Sv5zvzfsyyCYKkbK7vpvbje/n+3zkwtrVzY7Xc999duaZmfaTx9cKQILm7V4BgI0SLCCGYAExBAuIIVhADMECYggWEEOwgBiCBcQQLCCGYAExBAuIIVhADMECYggWEEOwgBiCBcQQLCCGYAExBAuIIVhADMECYggWEEOwgBiCBcQQLCCGYAExBAuIIVhADMECYggWEEOwgBiCBcQQLCCGYAExBAuIIVhADMECYggWEEOwgBiCBcQQLCCGYAExBAuIIVhADMECYggWEEOwgBiCBcQQLCCGYAExBAuIIVhADMECYggWEEOwgBiCBcQQLCCGYAExBAuIIVhADMECYggWEEOwgBiCBcQQLCCGYAExBAuIIVhADMECYggWEEOwgBiCBcQQLCCGYAExBAuIIVhADMECYggWEEOwgBiCBcQQLCCGYAExBAuIIVhADMECYggWEEOwgBiCBcQQLCCGYAExBAuIIVhADMECYggWEEOwgBiCBcQQLCCGYAExBAuIIVhADMECYggWEEOwgBiCBcQQLCCGYAExBAuIIVhADMECYggWEEOwgBiCBcQQLCCGYAExBAuIIVhADMECYggWEEOwgBiCBcQQLCCGYAExBAuIIVhADMECYggWEEOwgBiCBcQQLCCGYAExBAuIIVhADMECYggWEEOwgBiCBcQQLCCGYAExBAuIIVhADMECYggWEEOwgBiCBcQQLCCGYAExBAuIIVhADMECYggWEEOwgBiCBcQQLCCGYAExBAuIIVhADMECYggWEEOwgBiCBcQQLCCGYAExBAuIIVhADMECYggWEEOwgBiCBcQQLCCGYAExBAuIIVhADMECYggWEEOwgBiCBcQQLCCGYAExBAuIIVhADMECYggWEEOwgBiCBcQQLCCGYAExBAuIIVhADMECYggWEEOwgBiCBcQQLCCGYAExBAuIIVhADMECYggWEEOwgBiCBcQQLCCGYAExBAuIIVhADMECYggWEEOwgBiCBcQQLCCGYAExBAuIIVhADMECYggWEEOwgBiCBcQQLCCG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", "image/jpeg", method, type);
			return (Server::makeResponseMessage(200, absolute_path, "", method, type));
		}
	}
	return (Server::makeResponseBodyMessage(404, makeErrorPage(404), "", method));
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

	// map["SERVER_SOFTWARE"] = std::string("ftinx/1.0");
	// map["SERVER_NAME"] = res.get_m_cgi_server_name();
	// map["GATEWAY_INTERFACE"] = "Cgi/1.1";
	// map["SERVER_PORT"] = std::to_string(res.get_m_cgi_port());
	// map["PATH_INFO"] = req.get_m_path_info();
	// map["PATH_TRANSLATED"] = req.get_m_path_translated() + "/abc";

	// map["REDIRECT_STATUS"] = "";
	// map["SCRIPT_FILENAME"] = "";
	// map["SCRIPT_NAME"] = uri.get_m_path();

	// map["SCRIPT_NAME"] = "/cgi-bin/cgi_tester";
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
Server::executeCgi(Request req, Response res, std::string method)
{
	pid_t pid;
	char** envp = Server::makeCgiEnvp(req, res);
	Response response(res);
	int pipe1[2];
	int pipe2[2];
	int ret;
	//char buff[MAXLINE];
	std::string body;

	std::cout << "Execute Cgi >0<" << std::endl;
	if (pipe(pipe1) < 0 || pipe(pipe2) < 0)
		return (Server::makeResponseBodyMessage(404, makeErrorPage(404), "", method));

	int cgi_read= pipe1[0];
	int cgi_write = pipe2[1];
	int parent_read = pipe2[0];
	int parent_write = pipe1[1];

	fcntl(cgi_read, F_SETFL, O_NONBLOCK);
	fcntl(cgi_write, F_SETFL, O_NONBLOCK);
	fcntl(parent_read, F_SETFL, O_NONBLOCK);
	fcntl(parent_write, F_SETFL, O_NONBLOCK);

	printf("\n=========== cgi envp ============\n");
	for (int i = 0; i<3; i++)
	{
		printf("%s\n", envp[i]);
	}
	printf("===================================\n");

	if ((pid = fork()) < 0)
		return (Server::makeResponseBodyMessage(404, makeErrorPage(404), "", method));
	if (pid == 0)
	{
		close(parent_read);
		close(parent_write);
		// dup2 실패에 대한 에러처리를 어떤 방식으로 해줘야 할지?
		dup2(cgi_write, STDOUT_FILENO);
		dup2(cgi_read, STDIN_FILENO);
		ret = execve(req.get_m_path_translated().c_str(), 0, envp);
		exit(ret);
	}
	else
	{
		close(cgi_read);
		close(cgi_write);
		ft::fdSet(parent_write, this->m_write_fds);
		// while ((ret = recv(parent_read, buff, MAXLINE - 1, 0)) > 0)
		// {
		// 	buff[ret]= '\0';
		// 	body +=  std::string(buff);
		// }
		// std::cout << body.substr(0, 100) << std::endl;
 		response = makeResponseBodyMessage(404);
	}
	return (response);
}

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
				return (Server::makeResponseMessage(200, "./www/srcs/login.html", ""));
			else
				return (Server::makeResponseMessage(200, "./www/index.html", ""));
		}
	}
	return (res);
}

Response
Server::methodPOST(int clientfd, std::string method)
{
	Response response;
	Request request(m_requests[clientfd]);

	std::cout << "::1::"<< std::endl;
	if (request.checkCGI() == true)
	{
		std::cout << "::2::"<< std::endl;
		std::cout << "##### START CGI PART #####" << std::endl;
		executeCgi(request, response, method);
	}
	std::cout << "::3::"<< std::endl;
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
 			return (Server::makeResponseBodyMessage(404, makeErrorPage(404), "", method));
		status_code = 201;
	}
	else
	{
		if ((fd = open((path).c_str(), O_RDWR | O_TRUNC, 0666)) < 0)
			return (Server::makeResponseBodyMessage(404, makeErrorPage(404), "", method));
		status_code = 200;
	}
	body = req.get_m_body().c_str();
	if (write(fd, req.get_m_body().c_str(), ft::strlen(req.get_m_body().c_str())) < 0)
	{
		close(fd);
		return (Server::makeResponseBodyMessage(404, makeErrorPage(404), "", method));
	}
	else
	{
		close(fd);
		return (Server::makeResponseMessage(status_code, req.get_m_reset_path(), "", method));
	}
	return (Server::makeResponseBodyMessage(404, makeErrorPage(404), "", method));
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
			return (Server::makeResponseMessage(200, "./www/index.html", "", method));
	}
	return (Server::makeResponseBodyMessage(404, makeErrorPage(404), "", method));
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
		return (Server::makeResponseBodyMessage(404, makeErrorPage(404), "", method));
	allow_method = makeAllowMethod(location.get_m_limit_except());
	return (Server::makeResponseBodyMessage(204, "",  "", method, "text/html; charset=UTF-8", "", 0, 0, 0, allow_method));
}


/*============================================================================*/
/**********************************  TRACE  ***********************************/
/*============================================================================*/

Response
Server::methodTRACE(int clientfd, std::string method)
{
	Response response = Response();

	return (
		Server::makeResponseBodyMessage(200, this->m_requests[clientfd].get_m_message(), "", method, "message/http")
	);
}

/*============================================================================*/
/*****************************  RESPONSE MESSAGE ******************************/
/*============================================================================*/

Response
Server::makeResponseMessage(
	int status_code, std::string path, std::string transfer_encoding,
	std::string method, std::string content_type,
	std::string referer, int date_hour, int date_minute, int date_second,
	std::string allow_method, std::string content_location, std::string location,
	std::string content_language, std::string server
)
{
	(void) transfer_encoding;
	Response response = Response();

	if (status_code == 301 || status_code == 503)
		response.setHttpResponseHeader("retry-after", "10");
	if ((300 <= status_code && status_code < 400) || status_code == 201)
		response.setHttpResponseHeader("location", location);

	if (content_type != "image/gif" || content_type != "image/jpeg"
	|| content_type != "image/png" || content_type != "image/x-icon")
		response.setHttpResponseHeader("referer", referer);

	if (method == "GET")
		response.setHttpResponseHeader("last-modified", response.get_m_date());
	else if (method == "POST")
		response.setHttpResponseHeader("content-location", content_location);
	else if (method == "PUT")
		response.setHttpResponseHeader("content-location", content_location);
	else if (method == "OPTIONS")
		response.setHttpResponseHeader("allow", allow_method);

	return (
		response
			.setStatusCode(status_code)
			.setCurrentDate(date_hour, date_minute, date_second)
			.setFileDocument(path)
			.setContentLanguage(content_language)
			.setContentType(content_type)
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
	int status_code, std::string body, std::string transfer_encoding,
	std::string method, std::string content_type,
	std::string referer, int date_hour, int date_minute, int date_second,
	std::string allow_method, std::string content_location, std::string location,
	std::string content_language, std::string server
)
{
	(void) transfer_encoding;
	Response response = Response();

	if (status_code == 301 || status_code == 503)
		response.setHttpResponseHeader("retry-after", "10");
	if ((300 <= status_code && status_code < 400) || status_code == 201)
		response.setHttpResponseHeader("location", location);

	if (content_type != "image/gif" || content_type != "image/jpeg"
	|| content_type != "image/png" || content_type != "image/x-icon")
		response.setHttpResponseHeader("referer", referer);

	if (method == "GET")
		response.setHttpResponseHeader("last-modified", response.get_m_date());
	else if (method == "POST")
		response.setHttpResponseHeader("content-location", content_location);
	else if (method == "PUT")
		response.setHttpResponseHeader("content-location", content_location);
	else if (method == "OPTIONS")
		response.setHttpResponseHeader("allow", allow_method);

	return (
		response
			.setStatusCode(status_code)
			.setCurrentDate(date_hour, date_minute, date_second)
			.setContentLanguage(content_language)
			.setContentType(content_type)
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
			+ std::string(" by ftinx 0.1 port ")
			+ std::string(std::to_string(m_port))
			+ std::string("</i>\n</center>\n</body>\n</html>\n");
	return (page);
}

/*============================================================================*/
/***************************  SERVER METHOD UTIL  *****************************/
/*============================================================================*/

Response
Server::parseErrorResponse(int clientfd)
{
	int status_code(this->m_requests[clientfd].get_m_error_code());
	return (
		Server::makeResponseBodyMessage(status_code, makeErrorPage(status_code), "")
	);
}

Response
Server::checkValidRequestHeader(int clientfd)
{
	/* If request has no Host Header */
	if (this->m_requests[clientfd].get_m_uri().get_m_host() == "")
		return (Server::makeResponseBodyMessage(400, makeErrorPage(400), ""));
	return (
		Server::makeResponseBodyMessage(400, makeErrorPage(400), "")
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
	Response response = Response();
	Method method = this->m_requests[clientfd].get_m_method();

	/* make Response for Parse Error */
	if (this->m_requests[clientfd].get_m_error_code())
		response = this->parseErrorResponse(clientfd);
	else
	{
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
			response = Server::makeResponseBodyMessage(405, makeErrorPage(405), "");
			break;
		}
	}

	// std::cout << "\033[47:30m**** response message ****\033[0m" << std::endl;;
	// std::cout << response.get_m_reponse_message() << std::endl;

	/* 아무것도 전송안할순없으니까 0도 포함..? */
	if (send(clientfd, response.get_m_reponse_message().c_str(), response.get_m_response_size(), 0) <= 0)
	{
		close(clientfd);
		return (false);
	}
	writeLog("response", response);
	return (true);
}
