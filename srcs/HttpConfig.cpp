#include "HttpConfig.hpp"

/*============================================================================*/
/******************************  Constructor  *********************************/
/*============================================================================*/

HttpConfig::HttpConfig():
	m_name(),
	m_version(),
	m_include(),
	m_root()
{
}

HttpConfig::HttpConfig(std::string file_path):
	m_file_path(file_path),
	m_name(),
	m_version(),
	m_include(),
	m_root()
{
	setConfigFile();
	setConfigLines();
	if (checkStartHttp() == false)
		throw std::exception();
	if (checkCurlyBracketsFaired() == false)
		throw std::exception();
	parseHttpBlock();
}

HttpConfig::HttpConfig(HttpConfig const &other):
	m_name(),
	m_version(),
	m_include(),
	m_root()
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
	return (*this);
}

/*============================================================================*/
/******************************  Destructor  **********************************/
/*============================================================================*/

HttpConfig::~HttpConfig(){}

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
HttpConfig::checkCurlyBracketsDouble(std::string line)
{
	int cnt = 0;

	for (int i = 0 ; i < line.length() ; i++)
	{
		if (line[i] == '{' || line[i] == '}')
			cnt++;
		if (cnt > 1)
			return (false);
	}
	return (true);
}

void
HttpConfig::setConfigFile()
{
	this->m_config_file = ft::fileToString(m_file_path);
}

void
HttpConfig::setConfigLines()
{
	this->m_lines = ft::split(ft::trim(this->m_config_file, " "), '\n');
}

void
HttpConfig::parseHttpBlock()
{
	int idx = 0;
	bool server_block_exist = false;

	while (idx < this->m_lines.size())
	{
		std::vector<std::string> line;
		line.clear();
		line = ft::split(ft::trim(ft::rtrim(this->m_lines[idx], ";")," "), ' ');
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
			idx = server.parseServerBlock(this->m_lines, idx);
			this->m_server_block.push_back(server);
			continue ;
		}
		else if (line.front().compare("}") == 0)
			break ;
		idx++;
	}
	if (server_block_exist == false)
		throw std::exception();
}
