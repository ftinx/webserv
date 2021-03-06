#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <string>
# include <algorithm>
# include <vector>
# include <sys/socket.h>
# include <sys/time.h>
# include <fcntl.h>
# include <unistd.h>

# define BUFFER_SIZE 4096

enum Method
{
	DEFAULT,
	GET,
	HEAD,
	POST,
	PUT,
	DELETE,
	OPTIONS,
	TRACE,
};

namespace ft
{
	/* LIBFT C */
	bool isspace(int c);
	bool isdigit(int c);
	bool isprint(int c);
	size_t strlen(const char *str);
	char* strchr(const char *str, int c);
	void* memset(void *str, int c, size_t n);

	/* LIBFT C++ */
	std::string ltrim(std::string str, std::string set);
	std::string rtrim(std::string str, std::string set);
	std::string trim(std::string str, std::string set);
	std::vector<std::string> split(std::string const &str, char set);
	std::vector<std::string> split(std::string const &str, std::string set);

	/* FD SET */
	void fdZero(fd_set *fds);
	void fdSet(int fd, fd_set *fds);
	void fdClr(int fd, fd_set *fds);
	int fdIsSet(int fd, fd_set *fds); // not yet

	/* TCP */
	unsigned short hToNS(unsigned short hostshort);
	unsigned long hToNL(unsigned long hostlong);
	unsigned short nToHS(unsigned short netshort);
	unsigned long nToHL(unsigned long netlong);
	std::string iNetNtoA(unsigned int addr);

	/* TIME */
	std::string getDateTimestamp();

	/* ETC */
	bool isFileExists(std::string path); // not yet
	bool isDirExists(std::string path); // not yet
	bool checkBlankLine(std::string str);
	bool checkAnnotateLine(std::string str);
	std::string fileToString(std::string file_path);

}

#endif