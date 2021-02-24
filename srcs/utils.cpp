#include "utils.hpp"

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

std::vector<std::string>
split(std::string const &str, char const c)
{

}

std::string
getCurrentDateTime()
{

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
	fds->fds_bits[(unsigned long)fd/(sizeof(__int32_t) * 8)] |= mask;
}

void
fdClr(int fd, fd_set *fds)
{
	int mask;

	if (fd < 0 || fd >= 1024)
		return ;
	mask = ~(1 << ((unsigned long)fd % (sizeof(__int32_t) * 8)));
	fds->fds_bits[(unsigned long)fd/(sizeof(__int32_t) * 8)] &= mask;
}

int
fdIsSet(int fd, fd_set *fds)
{

}

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

}