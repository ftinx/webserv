#include "Server.hpp"

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

void 
Server::setServerAddr()
{
	memset((void *)&this->m_server_addr, 0x00, sizeof(this->m_server_addr));
	this->m_server_addr.sin_family = AF_INET;
	this->m_server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	this->m_server_addr.sin_port = htons(PORT);
}

int 
Server::setServerSocket()
{
	if ((this->m_server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket error");
		return (EXIT_FAILURE);
	}  
	if (bind(
			this->m_server_socket, 
			(struct sockaddr *)&this->m_server_addr, sizeof(this->m_server_addr)
		) == -1)
	{
		perror("bind error");
		return (EXIT_FAILURE);
	}   
	if (listen(this->m_server_socket, 5) == -1)
	{
		perror("listen error");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

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
		this->fd_num = select(
						this->maxfd + 1 , 
						&this->allfds, 
						(fd_set *)0,
						(fd_set *)0, 
						NULL
					);

		if (FD_ISSET(this->m_server_socket, &this->allfds))
		{
			this->addrlen = sizeof(this->m_client_addr);
			this->m_client_socket = accept(
					this->m_server_socket,
					(struct sockaddr *)&this->m_client_addr, 
					&this->addrlen
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
                while ( (this->readn = read(this->sockfd, this->recvline, MAXLINE-1)) > 0)
                {
                    fprintf(stdout, "\n%s\n\n%s", bin2hex(this->recvline, readn), this->recvline);
                    // fprintf(stdout, "%s", this->recvline);

                    //hacky way to detect the end of the message.
                    if (this->recvline[readn-1] == '\n')
                    {
                        break;
                    }
                    memset(this->recvline, 0, MAXLINE);
                }
                snprintf((char *)this->buff, sizeof(this->buff), "HTTP/1.0 200 OK\r\n\r\nHello");
				write(this->sockfd, this->buff, strlen((char *)this->buff));
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