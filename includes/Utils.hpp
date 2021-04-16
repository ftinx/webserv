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

# define BUFFER_SIZE 4096
// # define CGI_BUFF 65536
# define CGI_BUFF 4000000
// # define CGI_BUFF 32768
# define SOCK_BUFF 4000000
// # define SOCK_BUFF 680000
# define RESV_SIZE INT_MAX

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

enum FdType
{
	S_SOCKET,
	C_SOCKET,
	CGI_PIPE,
	CLOSED,
};

struct FDT
{
	enum FdType type;
	int sockfd;
	int clientfd;
};

const std::string B64CHRS =  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

namespace ft
{
	/* LIBFT C */
	bool isspace(int c);
	bool isdigit(int c);
	bool isprint(int c);
	int atoi(const char *str);
	size_t strlen(const char *str);
	void putchar_fd(const char c, int fd);
	void putstr_fd(const char *s, int fd);
	void putendl_fd(const char *s, int fd);
	void putnbr_fd(int n, int fd);
	char* strchr(const char *str, int c);
	void* memset(void *str, int c, size_t n);
	void itoa(int value, std::string& buf, int base);
	void doubleFree(char** str);

	/* LIBFT C++ */
	std::string strTolower(const std::string &str);
	int stoi(const std::string &str);
	char* strdup(const std::string &str);
	std::string ltrim(const std::string &str, const std::string &set);
	std::string rtrim(const std::string &str, const std::string &set);
	std::string trim(const std::string &str, const std::string &set);
	std::vector<std::string> split(const std::string &str, const char &set);
	std::vector<std::string> split(const std::string &str, const std::string &set);
	std::vector<std::string> split2(const std::string &str, const std::string &set);

	/* FD SET */
	void fdZero(fd_set *fds);
	void fdSet(int fd, fd_set *fds);
	void fdClr(int fd, fd_set *fds);
	bool fdIsSet(int fd, fd_set *fds);
	FDT makeFDT(enum FdType type, int sockfd, int clientfd);

	/* TCP */
	unsigned short hToNS(unsigned short hostshort);
	unsigned long hToNL(unsigned long hostlong);
	unsigned short nToHS(unsigned short netshort);
	unsigned long nToHL(unsigned long netlong);
	std::string iNetNtoA(unsigned int addr);

	/* TIME */
	std::string getDateTimestamp(int hour, int minute, int second);
	int compareTimestampToCurrent(std::string timestamp);
	std::string compareDetailTimestampToCurrent(std::string timestamp);

	/* BASE64 */
	std::string encode(const std::string &input);
	std::string decode(const std::string &input);

	/* FILE */
	std::string fileToString(const std::string &file_path);
	bool isValidFilePath(const std::string &path);
	bool isValidDirPath(const std::string &path);
	bool checkValidFileExtension(const std::string &file_name, const std::string &ext);
	bool checkValidFileExtension(const std::string &file_name, const std::vector<std::string> &ext_list);

	/* ETC */
	Method getMethodType(const std::string &str);
	std::string getMethodString(const Method &method);
	std::string getErrorMessage(int status_code);
	void console_log(std::string str);

	/* PASRSE CONFIG */
	bool checkBlankStr(std::string str);
	bool checkCommentStr(std::string str);
	bool checkCurlyBracketsDouble(std::string str);
}

#endif
