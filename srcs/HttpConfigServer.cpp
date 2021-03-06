#include "HttpConfigServer.hpp"

/*============================================================================*/
/******************************  Constructor  *********************************/
/*============================================================================*/

HttpConfigServer::HttpConfigServer():
	m_server_name(),
	m_listen(),
	m_default_error_page(),
	m_content_length()
{
}

HttpConfigServer::HttpConfigServer(HttpConfigServer const &other):
	m_server_name(),
	m_listen(),
	m_default_error_page(),
	m_content_length()
{
}

HttpConfigServer&
HttpConfigServer::operator=(HttpConfigServer const &rhs)
{
	m_server_name = rhs.m_server_name;
	m_listen = rhs.m_listen;
	m_default_error_page = rhs.m_default_error_page;
	m_content_length = rhs.m_content_length;
}

/*============================================================================*/
/******************************  Destructor  **********************************/
/*============================================================================*/

HttpConfigServer::~HttpConfigServer(){}

/*============================================================================*/
/********************************  Getter  ************************************/
/*============================================================================*/

std::string
HttpConfigServer::get_m_server_name() const
{
	return (this->m_server_name);
}

int
HttpConfigServer::get_m_listen() const
{
	return (this->m_listen);
}

std::string
HttpConfigServer::get_m_default_error_page() const
{
	return (this->m_default_error_page);
}

int
HttpConfigServer::get_m_content_length() const
{
	return (this->m_content_length);
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

int
HttpConfigServer::parseServerBlock(std::vector<std::string> lines, int idx)
{
	while (42)
	{
		std::vector<std::string> line;
		line.clear();
		line = ft::split(ft::trim(lines[idx]," "), ' ');
		if (ft::checkAnnotateLine(line[0]))
			continue ;
		if (line.front().compare("server_name") == 0)
			this->m_server_name = line.back();
		else if (line.front().compare("listen") == 0)
			this->m_listen = stoi(line.back());
		else if (line.front().compare("default_error_page") == 0)
			this->m_default_error_page = line.back();
		else if (line.front().compare("content_length") == 0)
			this->m_content_length = stoi(line.back());
		else if (line.front().compare("location") == 0)
		{
			HttpConfigLocation location = HttpConfigLocation();
			idx = location.parseLocationBlock(lines, idx);
			m_location_block.push_back(location);
			continue ;
		}
		else if (line.front().compare("}") == 0)
			return (idx + 1);
		idx++;
	}
	return (idx);
}
