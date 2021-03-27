#include "Utils.hpp"

namespace ft {

/*============================================================================*/
/*******************************  Libft C  ************************************/
/*============================================================================*/

bool
isspace(int c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (true);
	return (false);
}

bool
isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (true);
	return (false);
}

bool
isprint(int c)
{
	if (c >= 32 && c <= 126)
		return (true);
	return (false);
}

int
atoi(const char *str)
{
	int sign;
	int value;

	sign = 1;
	value = 0;
	while (ft::isspace(*str))
		str++;
	if (*str == '-')
		sign = -1;
	if (*str == '-' || *str == '+')
		++str;
	while (ft::isdigit(*str))
	{
		value = value * 10 + (*str - '0');
		str++;
	}
	value = sign == 1 ? value : -value;
	return (value);
}

size_t
strlen(const char *str)
{
	size_t	idx;

	idx = 0;
	while (str[idx])
		idx++;
	return (idx);
}

char *
ft_strchr(const char *str, int c)
{
	while (*str)
	{
		if (*str == c)
			return ((char *)str);
		str++;
	}
	if (c == '\0')
		return ((char *)str);
	return (0);
}

void *
memset(void *str, int c, size_t n)
{
	size_t	idx;

	idx = 0;
	while (idx < n)
	{
		*((unsigned char *)str + idx) = c;
		idx++;
	}
	return (str);
}

/*============================================================================*/
/*******************************  Libft C++  **********************************/
/*============================================================================*/

int
stoi(const std::string &str)
{
	int value;

	value = ft::atoi(str.c_str());
	return (value);
}

char *
strdup(std::string str)
{
	size_t	idx;
	char	*ret;

	if (!(ret = (char *)malloc(sizeof(char) * (str.length() + 1))))
		return (NULL);
	idx = 0;
	while (idx < str.length())
	{
		ret[idx] = str[idx];
		idx++;
	}
	ret[idx] = '\0';
	return (ret);
}

std::string
ltrim(std::string str, std::string set)
{
	size_t n;

	n = str.find_first_not_of(set);
	if (n == std::string::npos)
		return (str);
	else
		return (str.substr(n, str.length()));
}

std::string
rtrim(std::string str, std::string set)
{
	size_t n;

	n = str.find_last_not_of(set);
	if (n == std::string::npos)
		return (str);
	else
		return (str.substr(0, n + 1));
}

std::string
trim(std::string str, std::string set)
{
	return (rtrim(ltrim(str, set), set));
}

std::vector<std::string>
split(std::string const &str, char set)
{
	std::size_t start = 0;
	std::size_t pos_set = 0;
	std::vector<std::string> ret;

	while (start < str.length())
	{
		if (start == std::string::npos)
			break ;
		pos_set = str.find_first_of(set, start);
		if (pos_set == std::string::npos)
			pos_set = str.length();
		ret.push_back(str.substr(start, (pos_set - start)));
		start = pos_set + 1;
	}
	return (ret);
}

std::vector<std::string>
split(std::string const &str, std::string set)
{
	std::size_t start = 0;
	std::size_t pos_set = 0;
	std::vector<std::string> ret;

	while (start < str.length())
	{
		if (start == std::string::npos)
			break ;
		pos_set = str.find(set, start);
		if (pos_set == std::string::npos)
			pos_set = str.length();
		ret.push_back(str.substr(start, (pos_set - start)));
		start = str.find_first_not_of(set, pos_set);
	}
	return (ret);
}

/*============================================================================*/
/*******************************  FD SET  *************************************/
/*============================================================================*/

void
fdZero(fd_set *fds)
{
	for (int i = 0 ; i < 32 ; i++)
		fds->fds_bits[i] = 0;
}

void
fdSet(int fd, fd_set *fds)
{
	int mask;

	if (fd < 0 || fd >= 1024)
		return ;
	mask = 1 << ((unsigned long)fd % (sizeof(__int32_t) * 8));
	fds->fds_bits[(unsigned long)fd / (sizeof(__int32_t) * 8)] |= mask;
}

void
fdClr(int fd, fd_set *fds)
{
	int mask;

	if (fd < 0 || fd >= 1024)
		return ;
	mask = ~(1 << ((unsigned long)fd % (sizeof(__int32_t) * 8)));
	fds->fds_bits[(unsigned long)fd / (sizeof(__int32_t) * 8)] &= mask;
}

bool
fdIsSet(int fd, fd_set *fds)
{
	int mask;

	if (fd < 0 || fd >= 1024)
		return (false);
	mask = 1 << ((unsigned long)fd % (sizeof(__int32_t) * 8));
	if (fds->fds_bits[(unsigned long)fd / (sizeof(__int32_t) * 8)] & mask)
		return (true);
	return (false);
}

/*============================================================================*/
/*******************************  TCP  ****************************************/
/*============================================================================*/

unsigned short
hToNS(unsigned short hostshort)
{
	hostshort = (hostshort & 0x00ffU) >> 8 |
				(hostshort & 0xff00U) << 8;
	return (hostshort);
}

unsigned long
hToNL(unsigned long hostlong)
{
	hostlong = (hostlong & 0x000000ffU) << 24 |
				(hostlong & 0x0000ff00U) << 8 |
				(hostlong & 0x00ff0000U) >> 8 |
				(hostlong & 0xff000000U) >> 24;
	return (hostlong);
}

unsigned short
nToHS(unsigned short netshort)
{
	if (BYTE_ORDER == BIG_ENDIAN)
		return (netshort);
	else if (BYTE_ORDER == LITTLE_ENDIAN)
		return (ft::hToNS(netshort));
	return (0);
}

unsigned long
nToHL(unsigned long netlong)
{
	if (BYTE_ORDER == BIG_ENDIAN)
		return (netlong);
	else if (BYTE_ORDER == LITTLE_ENDIAN)
		return (ft::hToNL(netlong));
	return (0);
}

std::string
iNetNtoA(unsigned int addr)
{
	std::string ret;

	ret = std::to_string(addr % 256)
		+ "."
		+ std::to_string((addr / 256) % 256)
		+ "."
		+ std::to_string((addr / 256 / 256) % 256)
		+ "."
		+ std::to_string((addr / 256 / 256 / 256));
	return (ret);
}

/*============================================================================*/
/*******************************  TIME  ***************************************/
/*============================================================================*/

void
doubleFree(char **str)
{
	int i(0);

	while(str[i] == 0)
	{
		free(str[i]);
	}
	free(str);
}

std::string
getDateTimestamp(int hour, int minute, int second)
{
	struct timeval currentTime;
	struct tm *tm;
	char buf[64];
	int ret;

	gettimeofday(&currentTime, NULL);
	currentTime.tv_sec += 3600 * hour + 60 * minute + second;
	tm = localtime(&currentTime.tv_sec);
	ret = strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S %Z", tm);
	buf[ret] = '\0';
	return (buf);
}

int
compareTimestampToCurrent(std::string timestamp)
{
	struct timeval current_timeval;
	struct timeval timestamp_timeval;
	struct tm timestamp_tm;

	gettimeofday(&current_timeval, NULL);
	strptime(timestamp.c_str(), "%a, %d %b %Y %H:%M:%S %Z", &timestamp_tm);
	timestamp_timeval.tv_sec = mktime(&timestamp_tm);
	return (current_timeval.tv_sec - timestamp_timeval.tv_sec);
}

/*============================================================================*/
/*******************************  ETC  ****************************************/
/*============================================================================*/

bool
isValidFilePath(std::string path)
{
	struct stat buffer;

	if ((stat(path.c_str(), &buffer) == 0) && // stat 함수의 반환값이 0이면 정상적으로 파일 정보 조회된 것
		isValidDirPath(path) == false) // 그리고 폴더가 아니라면
		return (true); // 파일이 맞음
	return (false); // 파일이 아님
}

bool
isValidDirPath(std::string path)
{
	DIR *dirptr;

	if ((dirptr = opendir(path.c_str())) != NULL) // opendir 함수는 폴더 경로를 입력받아 성공하면 포인터 반환, 실패(존재하지 않거나 퍼미션 등의 이유로 실패)하면 NULL 반환
	{
		closedir(dirptr); // 열었으면 닫고
		return (true); // 폴더가 맞음
	}
	return (false); // 폴더가 아님
}

// std::string
// fileToString(std::string file_path)
// {
// 	int fd;
// 	int bytes;
// 	char buffer[BUFFER_SIZE];
// 	std::string ret;

// 	if ((fd = open(file_path.c_str(), O_RDONLY)) < 0)
// 		throw std::exception();
// 	ft::memset(buffer, 0, BUFFER_SIZE);
// 	while ((bytes = read(fd, buffer, BUFFER_SIZE) > 0))
// 		ret += std::string(buffer);
// 	if (bytes < 0)
// 		throw std::exception();
// 	return (ret);
// }

std::string
fileToString(std::string file_path)
{
	int fd;
	int bytes;
	char buffer[BUFFER_SIZE];
	std::string ret;

	if ((fd = open(file_path.c_str(), O_RDONLY)) < 0)
		throw std::exception();
	ft::memset(reinterpret_cast<void *>(buffer), 0, BUFFER_SIZE);
	while ((bytes = read(fd, reinterpret_cast<void *>(buffer), BUFFER_SIZE) > 0))
	{
		if (bytes < 0)
			throw std::exception();
		ret += std::string(buffer);
	}
	return (ret);
}

std::string
publicFileToString(std::string file_path)
{
	int fd;
	int bytes;
	char buffer[BUFFER_SIZE];
	std::string ret;

	if ((fd = open((file_path).c_str(), O_RDONLY)) < 0)
		throw std::exception();
	ft::memset(buffer, 0, BUFFER_SIZE);
	while ((bytes = read(fd, buffer, BUFFER_SIZE) > 0))
		ret += std::string(buffer);
	if (bytes < 0)
		throw std::exception();
	return (ret);
}

Method
getMethodType(std::string str)
{
	if (str == "GET")
		return (GET);
	else if (str == "HEAD")
		return (HEAD);
	else if (str == "POST")
		return (POST);
	else if (str == "PUT")
		return (PUT);
	else if (str == "DELETE")
		return (DELETE);
	else if (str == "OPTIONS")
		return (OPTIONS);
	else if (str == "TRACE")
		return (TRACE);
	return (DEFAULT);
}

std::string
getMethodString(Method method)
{
	if (method == GET)
		return ("GET");
	else if (method == HEAD)
		return ("HEAD");
	else if (method == POST)
		return ("POST");
	else if (method == PUT)
		return ("PUT");
	else if (method == DELETE)
		return ("DELETE");
	else if (method == OPTIONS)
		return ("OPTIONS");
	else if (method == TRACE)
		return ("TRACE");
	return ("DEFAULT");
}


bool
checkValidFileExtension(std::string file_name, std::string ext)
{
	std::vector<std::string> tmp;
	std::string extension;

	if (file_name.find_last_of(".") == std::string::npos)
		return (false);
	tmp = ft::split(file_name, '.');
	extension = tmp.back();
	if (ext.compare(extension) == 0)
		return (true);
	return (false);
}

bool
checkValidFileExtension(std::string file_name, std::vector<std::string> ext_list)
{
	std::vector<std::string>::const_iterator it;
	std::string extension;
	size_t pos;

	if ((pos = file_name.find_last_of(".")) == std::string::npos)
		return (false);
	extension = file_name.substr(pos, std::string::npos);
	for (it = ext_list.begin() ; it != ext_list.end() ; it++)
	{
		if (*it == extension)
			return (true);
	}
	return (false);
}


std::string
getErrorMessage(int status_code)
{
	switch (status_code)
	{
		case 100:
			return (std::string("Continue"));
		case 101:
			return (std::string("Switching Protocols"));
		case 200:
			return (std::string("OK"));
		case 201:
			return (std::string("Created"));
		case 202:
			return (std::string("Accepted"));
		case 203:
			return (std::string("Non-Authoritative Information"));
		case 204:
			return (std::string("No Content"));
		case 205:
			return (std::string("Reset Content"));
		case 206:
			return (std::string("Partial Content"));
		case 300:
			return (std::string("Multiple Choices"));
		case 301:
			return (std::string("Moved Permanently"));
		case 302:
			return (std::string("Found"));
		case 303:
			return (std::string("See Other"));
		case 304:
			return (std::string("Not Modified"));
		case 305:
			return (std::string("Use Proxy"));
		case 307:
			return (std::string("Temporary Redirect"));
		case 400:
			return (std::string("Bad Request"));
		case 401:
			return (std::string("Unauthorized"));
		case 402:
			return (std::string("Payment Requied"));
		case 403:
			return (std::string("Forbidden"));
		case 404:
			return (std::string("Not Found"));
		case 405:
			return (std::string("Method Not Allowed"));
		case 406:
			return (std::string("Not Acceptable"));
		case 407:
			return (std::string("Proxy Authentication Required"));
		case 408:
			return (std::string("Request Timeout"));
		case 409:
			return (std::string("Conflict"));
		case 410:
			return (std::string("Gone"));
		case 411:
			return (std::string("Length Required"));
		case 412:
			return (std::string("Precondition Failed"));
		case 413:
			return (std::string("Payload Too Large"));
		case 414:
			return (std::string("URI Too Long"));
		case 415:
			return (std::string("Unsupported Media Type"));
		case 416:
			return (std::string("Range Not Satisfiable"));
		case 417:
			return (std::string("Expectation Failed"));
		case 426:
			return (std::string("Upgrade Required"));
		case 500:
			return (std::string("Internal Server Error"));
		case 501:
			return (std::string("Not Implemented"));
		case 502:
			return (std::string("Bad Gateway"));
		case 503:
			return (std::string("Service Unavailable"));
		case 504:
			return (std::string("Gateway Timeout"));
		case 505:
			return (std::string("HTTP Version Not Supported"));
		default:
			return (std::string("Undefined Status Code"));
	}
}

}
