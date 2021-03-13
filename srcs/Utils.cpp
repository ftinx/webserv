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

std::string
getDateTimestamp(int hour, int minute, int second)
{
	struct timeval currentTime;
	struct tm *tm;
	char buf[64];

	gettimeofday(&currentTime, NULL);
	currentTime.tv_sec += 3600 * hour + 60 * minute + second;
	tm = localtime(&currentTime.tv_sec);
	strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S %Z", tm);
	free(tm);
	return (buf);
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

std::string
fileToString(std::string file_path)
{
	int fd;
	int bytes;
	char buffer[BUFFER_SIZE];
	std::string ret;

	if ((fd = open(file_path.c_str(), O_RDONLY)) < 0)
		throw std::exception();
	ft::memset(buffer, 0, BUFFER_SIZE);
	while ((bytes = read(fd, buffer, BUFFER_SIZE) > 0))
		ret += std::string(buffer);
	if (bytes < 0)
		throw std::exception();
	return (ret);
}

std::string
publicFileToString(std::string file_path)
{
	int fd;
	int bytes;
	char buffer[BUFFER_SIZE];
	std::string ret;

	if ((fd = open(("./www/" + file_path).c_str(), O_RDONLY)) < 0)
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

bool
checkValidFileExtension(std::string file_name, std::vector<std::string> ext_list)
{
	std::vector<std::string> tmp;
	std::string extension;

	if (file_name.find(".") == std::string::npos)
		return (false);
	tmp = ft::split(file_name, '.');
	extension = tmp.back();
	for (size_t i = 0 ; i < ext_list.size() ; i++)
	{
		if (ext_list[i].compare(extension) == 0)
			return (true);
	}
	return (false);
}

}
