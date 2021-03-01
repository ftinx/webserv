#include "HttpConfig.hpp"

HttpConfig::HttpConfig(std::string file_path)
{
	saveConfigFileToString(file_path);
	if (checkStartHttp() == false)
		throw std::exception();
	if (checkCurlyBrackets() == false)
		throw std::exception();

	std::vector<std::string> lines;
	lines = ft::split(this->m_config_file, "\n");
	for (int i = 0; i < lines.size(); ++i)
	{
		std::string tmp;
		lines[i] = ft::trim(lines[i], " ");
		if (!lines[i].compare("http {"))
			continue ;
		else if (!lines[i].compare("server {"))
			continue ;
		else if (!lines[i].compare("}"))
			continue ;
		else if (lines[i].length() > 9)
		{
			tmp = lines[i].substr(0, 9);
			if (!tmp.compare("location "))
				continue ;
		}

	}

}

void
HttpConfig::saveConfigFileToString(std::string file_path)
{
	int fd;
	int bytes;
	char buffer[BUFFER_SIZE];

	if ((fd = open(file_path.c_str(), O_RDONLY)) < 0)
		throw std::exception();
	ft::memset(buffer, 0, BUFFER_SIZE);
	while ((bytes = read(fd, buffer, BUFFER_SIZE) > 0))
		this->m_config_file += std::string(buffer);
	if (bytes < 0)
		throw std::exception();
	ft::trim(this->m_config_file, " ");
}

bool
HttpConfig::checkStartHttp()
{
	std::string tmp;

	tmp = this->m_config_file.substr(0, 4);
	if (tmp.compare("http") == 0)
		return (true);
	return (false);
}

bool
HttpConfig::checkCurlyBrackets()
{
	std::stack<char> stack;

	for (int i = 0 ; this->m_config_file[i] ; i++)
	{
		if (this->m_config_file[i] == '{')
			stack.push(this->m_config_file[i]);
		if (this->m_config_file[i] == '}')
		{
			if (stack.empty())
				return (false);
			else if (stack.top() == '{')
				stack.pop();
			else
				return (false);
		}
	}
	if (stack.empty())
		return (true);
	else
		return (false);
}