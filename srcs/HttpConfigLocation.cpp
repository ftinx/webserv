#include "HttpConfigLocation.hpp"

/*============================================================================*/
/******************************  Constructor  *********************************/
/*============================================================================*/

HttpConfigLocation::HttpConfigLocation():
	m_path(),
	m_limit_except(),
	m_root(),
	m_index(),
	m_cgi(),
	m_cgi_path(),
	m_autoindex()
{
}

HttpConfigLocation::HttpConfigLocation(HttpConfigLocation const &other):
	m_path(),
	m_limit_except(),
	m_root(),
	m_index(),
	m_cgi(),
	m_cgi_path(),
	m_autoindex()
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
HttpConfigLocation::checkCommentLine(std::string str)
{
	if (str.find("#") != 0)
		return (false);
	return (true);
}

HttpConfigLocation&
HttpConfigLocation::parseLocationBlock(std::vector<std::string> lines, size_t &idx)
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
			this->m_root = line.back();
		else if (line.front().compare("index") == 0)
		{

			for (size_t i = 1 ; i < line.size() ; i++)
			{
				if (line[i].empty())
					continue ;
				// if (checkFileExtention(line[i])) // 파일 확장자 체크
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
			this->m_cgi_path = line.back();
		else if (line.front().compare("autoindex") == 0)
		{
			if (line.back().compare("on") == 0)
				this->m_autoindex = true;
			else
				this->m_autoindex = false;
		}
		else if (line.front().compare("}") == 0)
		{
			idx++;
			break ;
		}
		idx++;
	}
	return (*this);
}
