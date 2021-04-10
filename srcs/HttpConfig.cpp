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
	// 파싱 잘 되었는지 출력 테스트
	// std::cout << "-----------mime_types_map----------" << std::endl;
	// std::map<std::string, std::string>::const_iterator test;
	// for (test = this->m_mime_types.begin() ; test != this->m_mime_types.end() ; ++test)
	// 	std::cout << test->first << " " << test->second << std::endl;
	// std::cout << "-----------------------------------" << std::endl;
}

void
HttpConfig::setDefaultRootPath()
{
	char path[1024];

	getcwd(path, 1024);
	this->m_root = std::string(path) + std::string("/www-default");
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

void
HttpConfig::printConfigFileInfo()
{
	std::string dash(80, '-');

	std::cout << dash << std::endl
		<< "config file infomation" << std::endl
		// << "path(relative) : " << this->m_file_path << std::endl
		<< "size: " << this->m_config_file.size() << " bytes, "
		// << this->m_lines.size() + this->m_cnt_trash_lines << " lines" << std::endl
		<< dash << std::endl
		<< std::endl;

	std::cout << dash << std::endl
		<< "http block infomation" << std::endl
		<< "software_name : " << this->m_name << std::endl
		<< "software_version : " << this->m_version << std::endl
		<< "include : " << this->m_include << std::endl
		<< "root : " << this->m_root << std::endl
		<< "size of server block : " << this->m_server_block.size() << std::endl
		<< dash << std::endl
		<< std::endl;

	for (size_t i = 0 ; i < this->m_server_block.size() ; i++)
	{
		std::cout << dash << std::endl
			<< "[" << i + 1 << "] server block infomation" << std::endl
			<< "server name : " << this->m_server_block[i].get_m_server_name() << std::endl
			<< "listen : " << this->m_server_block[i].get_m_listen() << std::endl
			<< "default error page : " << this->m_server_block[i].get_m_default_error_page() << std::endl
			<< "content length : " << this->m_server_block[i].get_m_content_length() << std::endl
			<< "size of location block : " << this->m_server_block[i].get_m_location_block().size() << std::endl;
		for (size_t j = 0 ; j < this->m_server_block[i].get_m_location_block().size() ; j++)
		{
			std::cout << "[" << j + 1 << "] location block infomation" << std::endl
				<< "path : " << this->m_server_block[i].get_m_location_block()[j].get_m_path() << std::endl
				<< "root : " << this->m_server_block[i].get_m_location_block()[j].get_m_root() << std::endl
				<< "index : ";
			for (size_t k = 0 ; k < this->m_server_block[i].get_m_location_block()[j].get_m_index().size() ; k++)
				std::cout << this->m_server_block[i].get_m_location_block()[j].get_m_index()[k] << " ";
			std::cout << std::endl;
			std::cout << "cgi : ";
			for (size_t k = 0 ; k < this->m_server_block[i].get_m_location_block()[j].get_m_cgi().size() ; k++)
				std::cout << this->m_server_block[i].get_m_location_block()[j].get_m_cgi()[k] << " ";
			std::cout << std::endl
				<< "cgi path : " << this->m_server_block[i].get_m_location_block()[j].get_m_cgi_path() << std::endl
				<< "autoindex : ";
			if (this->m_server_block[i].get_m_location_block()[j].get_m_autoindex())
				std::cout << "on" << std::endl;
			else
				std::cout << "off" << std::endl;
			std::cout << "auth basic : ";
			for (size_t k = 0 ; k < this->m_server_block[i].get_m_location_block()[j].get_m_auth_basic().size() ; k++)
				std::cout << this->m_server_block[i].get_m_location_block()[j].get_m_auth_basic()[k] << " ";
			std::cout << std::endl
				<< "auth basic user file : " << this->m_server_block[i].get_m_location_block()[j].get_m_auth_basic_user_file() << std::endl;

		}
		std::cout << dash << std::endl
			<< std::endl;
	}
}
