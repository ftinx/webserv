#include "HttpConfig.hpp"

/*============================================================================*/
/******************************  Constructor  *********************************/
/*============================================================================*/

HttpConfig::HttpConfig()
:
	m_name(),
	m_version(),
	m_include(),
	m_root()
{
}

HttpConfig::HttpConfig(std::string file_path)
:
	m_file_path(file_path),
	m_name(),
	m_version(),
	m_include(),
	m_root(),
	m_server_block()
{
	parseHttpBlock();
}

HttpConfig::HttpConfig(HttpConfig const &other):
	m_name(),
	m_version(),
	m_include(),
	m_root(),
	m_server_block()
{
	*this = other;
}

HttpConfig&
HttpConfig::operator=(HttpConfig const &rhs)
{
	m_file_path = rhs.m_file_path;
	m_name = rhs.m_name;
	m_version = rhs.m_version;
	m_include = rhs.m_include;
	m_root = rhs.m_root;
	m_server_block = rhs.m_server_block;
	return (*this);
}

/*============================================================================*/
/******************************  Destructor  **********************************/
/*============================================================================*/

HttpConfig::~HttpConfig()
{
	// std::cout << "HttpConfig Destructor Called" << std::endl;
}

/*============================================================================*/
/********************************  Getter  ************************************/
/*============================================================================*/

std::string
HttpConfig::get_m_name() const
{
	return (this->m_name);
}

std::string
HttpConfig::get_m_version() const
{
	return (this->m_version);
}

std::string
HttpConfig::get_m_include() const
{
	return (this->m_include);
}

std::string
HttpConfig::get_m_root() const
{
	return (this->m_root);
}

std::vector<HttpConfigServer>
HttpConfig::get_m_server_block() const
{
	return (this->m_server_block);
}

/*============================================================================*/
/********************************  Setter  ************************************/
/*============================================================================*/

/*============================================================================*/
/******************************  Exception  ***********************************/
/*============================================================================*/

/*============================================================================*/
/*********************************  Util  *************************************/
/*============================================================================*/

bool
HttpConfig::checkStartHttp()
{
	std::vector<std::string> first_line;

	first_line = ft::split(m_lines[0], " ");
	if (first_line[0].compare("http"))
		return (false);
	return (true);
}

bool
HttpConfig::checkBlankLine(std::string str)
{
    if (str.empty() == false)
        return (false);
    return (true);
}

bool
HttpConfig::checkCurlyBracketsFaired()
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
	if (stack.empty() == false)
		return (false);
	return (true);
}

bool
HttpConfig::checkCurlyBracketsDouble(std::string str)
{
	int cnt = 0;

	for (int i = 0 ; i < str.size() ; i++)
	{
		if (str[i] == '{' || str[i] == '}')
			cnt++;
		if (cnt > 1)
			return (true);
	}
	return (false);
}

void
HttpConfig::setConfigFile()
{
	this->m_config_file = ft::fileToString(m_file_path);
}

void
HttpConfig::setConfigLines()
{
	int idx = 0;

	this->m_lines = ft::split(this->m_config_file, '\n');
	while (idx < this->m_lines.size())
	{
		this->m_lines[idx] = ft::trim(this->m_lines[idx], " ");
		if (checkCurlyBracketsDouble(this->m_lines[idx]))
			throw std::exception();
		if (checkBlankLine(this->m_lines[idx]) ||
			HttpConfigLocation::checkAnnotateLine(this->m_lines[idx]))
		{
			this->m_lines.erase(this->m_lines.begin() + idx);
			continue ;
		}
		idx++;
	}
}

void
HttpConfig::checkValidHttpBlock()
{
	if (checkStartHttp() == false)
		throw std::exception();
	if (checkCurlyBracketsFaired() == false)
		throw std::exception();
}

void
HttpConfig::parseHttpBlock()
{
	int idx = 0;
	bool server_block_exist = false;

	setConfigFile();
	setConfigLines();
	checkValidHttpBlock();
	while (idx < this->m_lines.size())
	{
		std::vector<std::string> line;
		line.clear();
		line = ft::split(m_lines[idx], ' ');
		if (HttpConfigLocation::checkAnnotateLine(line.back()))
			line.pop_back();
		if (line.front().compare("software_name") == 0)
			this->m_name = line.back();
		else if (line.front().compare("software_version") == 0)
			this->m_version = line.back();
		else if (line.front().compare("include") == 0)
			this->m_include = line.back();
		else if (line.front().compare("root") == 0)
			this->m_root = line.back();
		else if (line.front().compare("server") == 0)
		{
			server_block_exist = true;
			HttpConfigServer server = HttpConfigServer();
			this->m_server_block.push_back(server.parseServerBlock(this->m_lines, idx));
			continue ;
		}
		else if (line.front().compare("}") == 0)
			break ;
		idx++;
	}
	if (server_block_exist == false)
		throw std::exception();
}
