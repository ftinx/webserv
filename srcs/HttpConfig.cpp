#include "HttpConfig.hpp"

/*============================================================================*/
/******************************  Constructor  *********************************/
/*============================================================================*/

HttpConfig::HttpConfig()
:
	m_name("ftinx"),
	m_version("0.1"),
	m_include(""),
	m_default_type("application/octet-stream"),
	m_root(""),
	m_tmp_path(""),
	m_server_block(),
	m_mime_types()
{
}

HttpConfig::HttpConfig(HttpConfig const &other)
{
	*this = other;
}

HttpConfig&
HttpConfig::operator=(HttpConfig const &rhs)
{
	m_name = rhs.m_name;
	m_version = rhs.m_version;
	m_include = rhs.m_include;
	m_default_type = rhs.m_default_type;
	m_root = rhs.m_root;
	m_server_block = rhs.m_server_block;
	m_mime_types = rhs.m_mime_types;
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
HttpConfig::get_m_default_type() const
{
	return (this->m_default_type);
}

std::string
HttpConfig::get_m_root() const
{
	return (this->m_root);
}

std::string
HttpConfig::get_m_tmp_path() const
{
	return (this->m_tmp_path);
}

std::vector<HttpConfigServer>
HttpConfig::get_m_server_block() const
{
	return (this->m_server_block);
}

std::map<std::string, std::string>
HttpConfig::get_m_mime_types() const
{
	return (this->m_mime_types);
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

void
HttpConfig::parseMimeTypes()
{
	std::string file = ft::fileToString(std::string("./") + this->m_include);
	std::vector<std::string> lines = ft::split(file, '\n');

	for (std::vector<std::string>::const_iterator it = lines.begin() ; it != lines.end() ; ++it)
	{
		if (ft::checkBlankStr(*it) || it->find("{") != std::string::npos || it->find("}") != std::string::npos)
			continue ;
		std::vector<std::string> tmp = ft::split(ft::rtrim(ft::ltrim(*it, " "), ";"), " ");
		std::string value = tmp.front();
		tmp.erase(tmp.begin());
		for (std::vector<std::string>::const_iterator i = tmp.begin() ; i != tmp.end() ; ++i)
			this->m_mime_types.insert(make_pair(*i, value));
	}
}

void
HttpConfig::setDefaultRootPath()
{
	char path[1024];

	getcwd(path, 1024);
	this->m_root = std::string(path) + std::string("/www-default");
}

void
HttpConfig::setDefaultTmpPath()
{
	char path[1024];

	getcwd(path, 1024);
	this->m_tmp_path = std::string(path) + std::string("/.tmp");
}

void
HttpConfig::parseConfigFile(std::string &file_path)
{
	size_t idx = 0;
	std::vector<std::string> line;
	bool http_block_exist = false;
	bool server_block_exist = false;
	this->m_config_file = ft::fileToString(file_path);
	this->m_lines = ft::split(this->m_config_file, '\n');

	setDefaultRootPath();
	setDefaultTmpPath();
	if (HttpConfig::checkCurlyBracketsFaired() == false)
		throw BracketPairErrorException();
	while (idx < this->m_lines.size())
	{
		this->m_lines[idx] = ft::trim(this->m_lines[idx], " ");
		if (ft::checkBlankStr(this->m_lines[idx]) || ft::checkCommentStr(this->m_lines[idx]))
		{
			idx++;
			continue ;
		}
		if (this->m_lines[idx].find_first_of("#") != std::string::npos)
		{
			this->m_lines[idx].erase(this->m_lines[idx].find_first_of("#"));
			this->m_lines[idx] = ft::rtrim(this->m_lines[idx], " ");
		}
		if (ft::checkCurlyBracketsDouble(this->m_lines[idx]))
			throw BracketDoubleErrorException(m_lines[idx], idx);
		line = ft::split(m_lines[idx], ' ');
		if (http_block_exist == false && line.front().compare("http") == 0)
			http_block_exist = true;
		else
		{
			if (http_block_exist == false)
				throw HttpBlockDoesNotExistException();
			if (line.front().compare("software_name") == 0)
				this->m_name = line.back();
			else if (line.front().compare("software_version") == 0)
				this->m_version = line.back();
			else if (line.front().compare("include") == 0)
			{
				this->m_include = line.back();
				if (ft::checkValidFileExtension(this->m_include, "types"))
					parseMimeTypes();
			}
			else if (line.front().compare("default_type") == 0)
				this->m_default_type =  line.back();
			else if (line.front().compare("root") == 0)
			{
				if (ft::isValidDirPath(line.back()) == false)
					throw PathErrorException(m_lines[idx], idx);
				this->m_root = line.back();
			}
			else if (line.front().compare("tmp_path") == 0)
			{
				if (ft::isValidDirPath(line.back()) == false)
					throw PathErrorException(m_lines[idx], idx);
				this->m_tmp_path = line.back();
			}
			else if (line.front().compare("server") == 0)
			{
				server_block_exist = true;
				HttpConfigServer server = HttpConfigServer();
				this->m_server_block.push_back(server.parseServerBlock(this->m_lines, this->m_root, idx));
				continue ;
			}
			else if (line.front().compare("}") == 0)
				break ;
		}
		idx++;
	}
	if (server_block_exist == false)
		throw ServerBlockDoesNotExistException();
	if (this->m_mime_types.empty())
		throw MimeTypeErrorException();
}
