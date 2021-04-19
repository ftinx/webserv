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
	m_auth_basic(),
	m_auth_basic_user_file(""),
	m_limit_body_size(INT_MAX),
	m_redirect()
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
	m_redirect = rhs.m_redirect;
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

std::vector<std::string>
HttpConfigLocation::get_m_auth_basic() const
{
	return (this->m_auth_basic);
}

std::string
HttpConfigLocation::get_m_auth_basic_user_file() const
{
	return (this->m_auth_basic_user_file);
}

long
HttpConfigLocation::get_m_limit_body_size() const
{
	return (this->m_limit_body_size);
}

std::vector<std::string>
HttpConfigLocation::get_m_redirect() const
{
	return (this->m_redirect);
}

/*============================================================================*/
/********************************  Setter  ************************************/
/*============================================================================*/

void
HttpConfigLocation::set_m_path(std::string path)
{
	this->m_path = path;
}

void
HttpConfigLocation::set_m_root(std::string root)
{
	this->m_root = root;
}

void
HttpConfigLocation::set_m_limit_except(std::vector<Method> limit_except)
{
	this->m_limit_except = limit_except;
}

void
HttpConfigLocation::set_m_redirect(std::vector<std::string> redirect)
{
	this->m_redirect = redirect;
}


/*============================================================================*/
/******************************  Exception  ***********************************/
/*============================================================================*/

/*============================================================================*/
/*********************************  Util  *************************************/
/*============================================================================*/

HttpConfigLocation&
HttpConfigLocation::parseLocationBlock(std::vector<std::string> lines, std::string root, size_t &idx)
{
	std::vector<std::string> line;

	while (42)
	{
		lines[idx] = ft::trim(lines[idx], " ");
		if (ft::checkBlankStr(lines[idx]) || ft::checkCommentStr(lines[idx]))
		{
			idx++;
			continue ;
		}
		if (lines[idx].find_first_of("#") != std::string::npos)
		{
			lines[idx].erase(lines[idx].find_first_of("#"));
			lines[idx] = ft::rtrim(lines[idx], " ");
		}
		if (ft::checkCurlyBracketsDouble(lines[idx]))
			throw BracketDoubleErrorException(lines[idx], idx);
		line.clear();
		line = ft::split(lines[idx], ' ');
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
			if (ft::isValidDirPath(line.back()) == false)
				throw PathErrorException(lines[idx], idx);
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
				this->m_cgi.push_back(line[i]);
			}
		}
		else if (line.front().compare("cgi_path") == 0)
		{
			if (ft::isValidFilePath(line.back()) == false)
				throw PathErrorException(lines[idx], idx);
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
			for (size_t i = 1 ; i < line.size() ; i++)
			{
				if (line[i].empty())
					continue ;
				this->m_auth_basic.push_back(line[i]);
			}
		}
		else if (line.front().compare("auth_basic_user_file") == 0)
		{
			if (ft::isValidFilePath(root + std::string("/") + line.back()) == false)
				throw PathErrorException(lines[idx], idx);
			this->m_auth_basic_user_file = line.back();
		}
		else if (line.front().compare("limit_body_size") == 0)
		{
			char *tmp;
			std::strtol(line.back().c_str(), &tmp, 10);
			if (*tmp)
				throw std::exception();
			this->m_limit_body_size = ft::stol(line.back());
			if (this->m_limit_body_size == 0)
				this->m_limit_body_size = INT_MAX;
			else if (this->m_limit_body_size < 0 || this->m_limit_body_size > INT_MAX)
				throw std::exception();
		}
		else if (line.front().compare("return") == 0)
		{
			if (this->m_redirect.empty() == false)
			{
				idx++;
				continue ;
			}
			for (size_t i = 1 ; i < line.size() ; i++)
			{
				if (line[i].empty())
					continue ;
				this->m_redirect.push_back(line[i]);
			}
			char *tmp;
			std::strtol(this->m_redirect.front().c_str(), &tmp, 10);
			if (*tmp || this->m_redirect.size() != 2)
				throw std::exception();
			if (this->m_redirect[0].compare("301") != 0 &&
				this->m_redirect[0].compare("302") != 0 &&
				this->m_redirect[0].compare("307") != 0) // 301 302 307 확인 후 아니면 에러
				throw std::exception();
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
