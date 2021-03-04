#include "HttpConfigLocation.hpp"

/*============================================================================*/
/******************************  Constructor  *********************************/
/*============================================================================*/

HttpConfigLocation::HttpConfigLocation():
	m_path(),
	m_root(),
	m_cgi_path()
{
}

HttpConfigLocation::HttpConfigLocation(HttpConfigLocation const &other):
	m_path(),
	m_root(),
	m_cgi_path()
{
}

HttpConfigLocation&
HttpConfigLocation::operator=(HttpConfigLocation const &rhs)
{
	m_path = rhs.m_path;
	m_root = rhs.m_root;
	m_cgi_path = rhs.m_cgi_path;
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

std::string
HttpConfigLocation::get_m_root() const
{
	return (this->m_root);
}

std::string
HttpConfigLocation::get_m_cgi_path() const
{
	return (this->m_cgi_path);
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

Method
HttpConfigLocation::convertStringToMethod(std::string str)
{
	if (str == "GET")
		return (GET);
	else if (str == "HEAD")
		return (HEAD);
	else if (str == "POST")
		return (POST);
	else if (str == "PUT")
		return (PUT);
	else if (str == "DELETE")
		return (DELETE);
	else if (str == "OPTIONS")
		return (OPTIONS);
	else if (str == "TRACE")
		return (TRACE);
	else
		return (ERROR);
}

int
HttpConfigLocation::parseLocationBlock(std::vector<std::string> lines, int idx)
{
	while (42)
	{
		std::vector<std::string> line;
		line.clear();
		line = ft::split(ft::ltrim(ft::rtrim(lines[idx], ";")," "), ' ');
		if (line.front().compare("location") == 0)
		{
			for (int i = 1 ; i < line.size() ; i++)
			{
				if (line.empty())
					continue ;
				this->m_path = line[i];
				break ;
			}
		}
		else if (line.front().compare("limit_except") == 0)
		{
			for (int i = 1 ; i < line.size() ; i++)
			{
				if (line[i].empty())
					continue ;
				this->m_limit_except.push_back(convertStringToMethod(line[i]));
			}
		}
		else if (line.front().compare("root") == 0)
			this->m_root = line.back();
		else if (line.front().compare("index") == 0)
		{
			for (int i = 1 ; i < line.size() ; i++)
			{
				if (line[i].empty())
					continue ;
				// if (checkFileExtention(line[i])) // 파일 확장자 체크
				this->m_index.push_back(line[i]);
			}
		}
		else if (line.front().compare("cgi") == 0)
		{
			for (int i = 1 ; i < line.size() ; i++)
			{
				if (line[i].empty())
					continue ;
				// if (checkCgiFormat(line[i])) // cgi 포맷 체크
				this->m_cgi.push_back(line[i]);
			}
		}
		else if (line.front().compare("cgi_path") == 0)
			this->m_cgi_path = line.back();
		else if (line.front().compare("}") == 0)
			return (idx + 1);
		idx++;
	}
	return (idx);
}
