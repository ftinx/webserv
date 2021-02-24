#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <string>
# include <algorithm>
# include <vector>
# include <sys/socket.h>
# include <time.h>

namespace ft
{
	bool isspace(int c);
	bool isdigit(int c);
	bool isprint(int c);

	size_t strlen(const char *str);
	void *memset(void *str, int c, size_t n);
	std::string tolower(std::string str);
	std::string toupper(std::string str);
	std::vector<std::string> split(std::string const &str, char const c);

	std::string getCurrentDateTime(void);

	void fdZero(fd_set *fds);
	void fdSet(int fd, fd_set *fds);
	void fdClr(int fd, fd_set *fds);
	int fdIsSet(int fd, fd_set *fds);

	unsigned long hToNL(unsigned long hostlong);
	unsigned short hToNS(unsigned short hostshort);
	unsigned long nToHL(unsigned long netlong);
	unsigned short nToHS(unsigned short netshort);

	char *iNetNtoA(struct in_addr in);

}

#endif;