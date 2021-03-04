#include "Utils.hpp"

namespace ft {

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

bool
checkBlankLine(std::string str)
{
	if (str.empty())
		return (true);
	return (false);
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

// int
// fdIsSet(int fd, fd_set *fds)
// {

// }

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

// std::string
// getDateTimestamp()
// {

// }

std::string
getDateTimestamp()
{
	struct timeval currentTime;
	struct tm *tm;
	char buf[64];

	gettimeofday(&currentTime, NULL);
	tm = localtime(&currentTime.tv_sec);
	strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S %Z", tm);
	free(tm);
	return (buf);
}

std::string
fileToString(std::string file_path)
{
	int fd;
	int bytes;
	char buffer[BUFFER_SIZE];
	std::string ret;

	if ((fd == open(file_path.c_str(), O_RDONLY)) < 0)
		throw std::exception();
	ft::memset(buffer, 0, BUFFER_SIZE);
	while ((bytes = read(fd, buffer, BUFFER_SIZE) > 0))
		ret += std::string(buffer);
	if (bytes < 0)
		throw std::exception();
	return (ret);
}


}