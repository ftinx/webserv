#include "HttpConfigLocation.hpp"

/*============================================================================*/
/******************************  Constructor  *********************************/
/*============================================================================*/

HttpConfigLocation::HttpConfigLocation():
	m_path(""),
	m_limit_except(),
	m_root(""),
	m_index(),
	m_cgi(),
	m_cgi_path(""),
	m_autoindex(false),
	m_auth_basic(""),
	m_auth_basic_user_file(""),
	m_limit_body_size(1048567)
{
}

HttpConfigLocation::HttpConfigLocation(HttpConfigLocation const &other)
{
	*this = other;
}

HttpConfigLocation&
HttpConfigLocation::operator=(HttpConfigLocation const &rhs)
{
	m_path = rhs.m_path;
	m_limit_except = rhs.m_limit_except;
	m_root = rhs.m_root;
	m_index = rhs.m_index;
	m_cgi = rhs.m_cgi;
	m_cgi_path = rhs.m_cgi_path;
	m_autoindex = rhs.m_autoindex;
	m_auth_basic = rhs.m_auth_basic;
	m_auth_basic_user_file = rhs.m_auth_basic_user_file;
	m_limit_body_size = rhs.m_limit_body_size;
	return (*this);
}

/*============================================================================*/
/******************************  Destructor  **********************************/
/*============================================================================*/

HttpConfigLocation::~HttpConfigLocation(){}

/*============================================================================*/
/********************************  Getter  ************************************/
/*============================================================================*/

std::string
HttpConfigLocation::get_m_path() const
{
	return (this->m_path);
}

std::vector<Method>
HttpConfigLocation::get_m_limit_except() const
{
	return (this->m_limit_except);
}

std::string
HttpConfigLocation::get_m_root() const
{
	return (this->m_root);
}

std::vector<std::string>
HttpConfigLocation::get_m_index() const
{
	return (this->m_index);
}

std::vector<std::string>
HttpConfigLocation::get_m_cgi() const
{
	return (this->m_cgi);
}

std::string
HttpConfigLocation::get_m_cgi_path() const
{
	return (this->m_cgi_path);
}

bool
HttpConfigLocation::get_m_autoindex() const
{
	return (this->m_autoindex);
}

std::string
HttpConfigLocation::get_m_auth_basic() const
{
	return (this->m_auth_basic);
}

std::string
HttpConfigLocation::get_m_auth_basic_user_file() const
{
	return (this->m_auth_basic_user_file);
}

int
HttpConfigLocation::get_m_limit_body_size() const
{
	return (this->m_limit_body_size);
}

/*============================================================================*/
/********************************  Setter  ************************************/
/*============================================================================*/

void
HttpConfigLocation::set_m_root(std::string root)
{
	this->m_root = root;
}

/*============================================================================*/
/******************************  Exception  ***********************************/
/*============================================================================*/

/*============================================================================*/
/*********************************  Util  *************************************/
/*============================================================================*/

bool
HttpConfigLocation::checkCommentLine(std::string str)
{
	if (str.find("#") != 0)
		return (false);
	return (true);
}

void
HttpConfigLocation::checkDirExist(std::string path)
{
	if (ft::isValidDirPath(path) == false)
		throw HttpConfigLocation::parseErrorException();
}

void
HttpConfigLocation::checkFileExist(std::string root, std::string path)
{
	if (root != "") // 상대경로라면 root와 path 결합 후 유효성체크
	{
		if (ft::isValidFilePath(root + path.substr(2, path.length())) == false)
			throw HttpConfigLocation::parseErrorException();
	}
	else // 절대경로라면 path 유효성 체크
	{
		if (ft::isValidFilePath(path) == false)
			throw HttpConfigLocation::parseErrorException();
	}
}

const char* HttpConfigLocation::parseErrorException::what() const throw()
{
	return ("Error: Failed to parse the config file.");
}

HttpConfigLocation&
HttpConfigLocation::parseLocationBlock(std::vector<std::string> lines, std::string root, size_t &idx)
{
	while (42)
	{
		std::vector<std::string> line;
		line.clear();
		line = ft::split(lines[idx], ' ');
		if (checkCommentLine(line.back()))
			line.pop_back();
		if (line.front().compare("location") == 0)
		{
			for (size_t i = 1 ; i < line.size() ; i++)
			{
				if (line.empty())
					continue ;
				this->m_path = line[i];
				break ;
			}
		}
		else if (line.front().compare("limit_except") == 0)
		{
			for (size_t i = 1 ; i < line.size() ; i++)
			{
				if (line[i].empty())
					continue ;
				this->m_limit_except.push_back(ft::getMethodType(line[i]));
			}
		}
		else if (line.front().compare("root") == 0)
		{
			// checkDirExist(line.back()); // 유효성 체크, 유연한 테스트를 위해 주석처리
			this->m_root = line.back();
		}
		else if (line.front().compare("index") == 0)
		{
			for (size_t i = 1 ; i < line.size() ; i++)
			{
				if (line[i].empty())
					continue ;
				this->m_index.push_back(line[i]);
			}
		}
		else if (line.front().compare("cgi") == 0)
		{
			for (size_t i = 1 ; i < line.size() ; i++)
			{
				if (line[i].empty())
					continue ;
				// if (checkCgiFormat(line[i])) // cgi 포맷 체크
				this->m_cgi.push_back(line[i]);
			}
		}
		else if (line.front().compare("cgi_path") == 0)
		{
			// checkFileExist("", line.back()); // 유효성 체크, 유연한 테스트를 위해 주석처리
			this->m_cgi_path = line.back();
		}
		else if (line.front().compare("autoindex") == 0)
		{
			if (line.back().compare("on") == 0)
				this->m_autoindex = true;
			else
				this->m_autoindex = false;
		}
		else if (line.front().compare("auth_basic") == 0)
		{
			this->m_auth_basic = line.back();
		}
		else if (line.front().compare("auth_basic_user_file") == 0)
		{
			this->m_auth_basic_user_file = line.back();
		}
		else if (line.front().compare("limit_body_size") == 0)
		{
			this->m_limit_body_size = ft::stoi(line.back());
			if (this->m_limit_body_size == 0)
				this->m_limit_body_size = INT_MAX;
		}
		else if (line.front().compare("}") == 0)
		{
			idx++;
			break ;
		}
		idx++;
	}
	if (this->m_root.compare("") == 0)
		this->m_root = root;
	return (*this);
}
