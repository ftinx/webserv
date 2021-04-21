#include "ServerManager.hpp"

/*============================================================================*/
/****************************  Static variables  ******************************/
/*============================================================================*/

/*============================================================================*/
/******************************  Constructor  *********************************/
/*============================================================================*/


ServerManager::ServerManager()
: m_err_message("")
{};

ServerManager::ServerManager(ServerManager const &other)
{
	*this = other;
};
ServerManager& ServerManager::operator=(ServerManager const &rhs)
{
	this->m_httpConfig = rhs.m_httpConfig;

	this->m_mime_types = rhs.m_mime_types;
	this->m_server_block = rhs.m_server_block;
	this->m_server = rhs.m_server;
	this->m_server_size = rhs.m_server_size;

	this->m_software_name = rhs.m_software_name;
	this->m_software_version = rhs.m_software_version;
	this->m_mime_include = rhs.m_mime_include;
	this->m_root = rhs.m_root;
	this->m_default_type = rhs.m_default_type;

	/* Socket */
	this->m_maxfd = rhs.m_maxfd;
	this->m_main_fds = rhs.m_main_fds;
	this->m_copy_fds = rhs.m_copy_fds;
	this->m_copy_write_fds = rhs.m_copy_write_fds;

	/* Flag */
	this->m_err_message = rhs.m_err_message;
	return (*this);
};

/*============================================================================*/
/******************************  Destructor  **********************************/
/*============================================================================*/

ServerManager::~ServerManager(){};

/*============================================================================*/
/*******************************  Overload  ***********************************/
/*============================================================================*/

/*============================================================================*/
/********************************  Getter  ************************************/
/*============================================================================*/

/*============================================================================*/
/********************************  Setter  ************************************/
/*============================================================================*/

/*============================================================================*/
/******************************  Exception  ***********************************/
/*============================================================================*/

/*============================================================================*/
/*********************************  Util  *************************************/
/*============================================================================*/

void
ServerManager::storeParseValue()
{
	this->m_software_name = this->m_httpConfig.get_m_name();
	this->m_software_version = this->m_httpConfig.get_m_version();
	this->m_mime_include = this->m_httpConfig.get_m_include();
	this->m_root = this->m_httpConfig.get_m_root();
	this->m_server_size = this->m_httpConfig.get_m_server_block().size();
	this->m_server_block = this->m_httpConfig.get_m_server_block();
	this->m_mime_types = this->m_httpConfig.get_m_mime_types();
	this->m_default_type = this->m_httpConfig.get_m_default_type();
	return ;
}

void
ServerManager::parseHttpConfig(std::string config_path)
{
	this->m_httpConfig.parseConfigFile(config_path);
	storeParseValue();
	return ;
}

Server
ServerManager::generateServer(
	HttpConfigServer server_block,
	std::string server_name, int port, int content_length, size_t location_size, std::string root,
	std::map<std::string, std::string> mime_types, std::string default_type,
	int maxfd, fd_set main_fds, fd_set read_fds, fd_set write_fds, fd_set copy_write_fds
	)
{
	Server server;
	server.init(
		server_block, server_name, port, content_length,location_size, root, mime_types, default_type,
		&maxfd, &main_fds, &read_fds, &write_fds, &copy_write_fds
		);
	server.setServerAddr(port);
	this->m_err_message = server.setServerSocket();
	server.noteHttpConfigLocation();
	return (server);
}

std::string
ServerManager::initServers()
{
	for (size_t i = 0; i < this->m_server_size; i++) {
		this->m_server.push_back(
			new Server(generateServer(
				this->m_server_block[i],
				this->m_software_name + '/' + this->m_software_version + ": " + this->m_server_block[i].get_m_server_name(),
				this->m_server_block[i].get_m_listen(),
				this->m_server_block[i].get_m_content_length(),
				this->m_server_block[i].get_m_location_block().size(),
				this->m_root,
				this->m_mime_types,
				this->m_default_type,
				this->m_maxfd,
				this->m_main_fds,
				this->m_read_fds,
				this->m_write_fds,
				this->m_copy_write_fds
			))
		);
	}
	return (this->m_err_message);
}

void
ServerManager::runServers()
{
	int fd_num = 0;

	struct timeval timeout;
	timeout.tv_sec = 1;
	timeout.tv_usec = 2;

	signal(SIGINT, exitServers);
	signal(SIGPIPE, SIG_IGN);

	this->m_maxfd = 0;
	FD_ZERO(&this->m_main_fds);
	FD_ZERO(&this->m_write_fds);

	for (size_t i = 0; i < this->m_server_size; i++) {
		FD_SET(this->m_server[i]->get_m_server_socket(), &this->m_main_fds);
		if (this->m_maxfd < this->m_server[i]->get_m_server_socket())
			this->m_maxfd = this->m_server[i]->get_m_server_socket();
	}

	while (42)
	{
		// printf("---Select Wait %d---\n", this->m_maxfd);
		this->m_read_fds = this->m_main_fds;
		this->m_copy_write_fds = this->m_write_fds;
		// for (int i=0; i<1; i++) {
		// 	std::cout << "-----" << "BEFORE SELECT" << "-----" << std::endl;
		// 	std::cout << std::bitset<32>(this->m_copy_write_fds.fds_bits[i]) << std::endl;
		// }

		fd_num = select(
			this->m_maxfd + 1 ,
			&this->m_read_fds,
			&this->m_copy_write_fds,
			reinterpret_cast<fd_set *>(0),
			&timeout
		);
		// for (int i=0; i<1; i++) {
		// 	std::cout << "-----" << "AFTER SELECT" << "-----" << std::endl;
		// 	std::cout << std::bitset<32>(this->m_write_fds.fds_bits[i]) << std::endl << std::endl;;
		// }

		switch (fd_num)
		{
			case -1:
				std::cerr << "select error" << std::endl;
				std::cout << EBADF << std::endl;
				std::cout << EINTR << std::endl;
				std::cout << EINVAL << std::endl;
				std::cout << ENOMEM << std::endl;
				std::cout << errno << std::endl;
				return ;
			case 0:
				// ft::console_log("Timeout Reset");
				std::cerr << "Timeout Reset" << std::endl;
				int max_server_socket;
				for (size_t i = 0; i < this->m_server_size; i++) {
					if (max_server_socket < this->m_server[i]->get_m_server_socket())
						max_server_socket = this->m_server[i]->get_m_server_socket();
				}
				for (int i=max_server_socket + 1; i<=this->m_maxfd; i++)
				{
					if (ft::fdIsSet(i, &this->m_main_fds) || ft::fdIsSet(i, &this->m_write_fds))
					{
						close(i);
					}
				}
				FD_ZERO(&this->m_main_fds);
				FD_ZERO(&this->m_write_fds);
				this->m_maxfd = 0;
				for (size_t i = 0; i < this->m_server_size; i++) {
					FD_SET(this->m_server[i]->get_m_server_socket(), &this->m_main_fds);
					if (this->m_maxfd < this->m_server[i]->get_m_server_socket())
						this->m_maxfd = this->m_server[i]->get_m_server_socket();
				}
			default:
				for (size_t i = 0; i < this->m_server_size; i++) {
					this->m_server[i]->getRequest(&this->m_main_fds, &this->m_read_fds,
					&this->m_copy_write_fds, &this->m_write_fds, &this->m_maxfd);
				}
		}
	}
	return ;
}

void
ServerManager::exitServers(int signo)
{
	if (signo == SIGINT)
	{
		/* clean child process */
		// kill(childfd, SIGTERM);
		// waitpid(childfd, &status, WNOHANG);
		exit(EXIT_SUCCESS);
	}
	return ;
}
