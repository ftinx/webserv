#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <string>
# include <algorithm>
# include <vector>
# include <map>
# include <sys/socket.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <dirent.h>
# include <fcntl.h>
# include <unistd.h>

# define BUFFER_SIZE 20000// 4096

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
	int atoi(const char *str);
	size_t strlen(const char *str);
	char* strchr(const char *str, int c);
	void* memset(void *str, int c, size_t n);

	/* LIBFT C++ */
	int stoi(const std::string &str);
	char* strdup(std::string str);
	std::string ltrim(std::string str, std::string set);
	std::string rtrim(std::string str, std::string set);
	std::string trim(std::string str, std::string set);
	std::vector<std::string> split(std::string const &str, char set);
	std::vector<std::string> split(std::string const &str, std::string set);

	/* FD SET */
	void fdZero(fd_set *fds);
	void fdSet(int fd, fd_set *fds);
	void fdClr(int fd, fd_set *fds);
	bool fdIsSet(int fd, fd_set *fds);

	/* TCP */
	unsigned short hToNS(unsigned short hostshort);
	unsigned long hToNL(unsigned long hostlong);
	unsigned short nToHS(unsigned short netshort);
	unsigned long nToHL(unsigned long netlong);
	std::string iNetNtoA(unsigned int addr);

	/* TIME */
	std::string getDateTimestamp(int hour, int minute, int second);
	int compareTimestampToCurrent(std::string timestamp);

	/* ETC */
	bool isValidFilePath(std::string path);
	bool isValidDirPath(std::string path);
	std::string fileToString(std::string file_path);
	std::string publicFileToString(std::string file_path);
	Method getMethodType(std::string str);
	std::string getMethodString(Method method);
	bool checkValidFileExtension(std::string file_name, std::string ext);
	bool checkValidFileExtension(std::string file_name, std::vector<std::string> ext_list);
	std::string getErrorMessage(int status_code);

	std::string encode(const std::string &input);
	std::string decode(const std::string &input);


}

#endif
