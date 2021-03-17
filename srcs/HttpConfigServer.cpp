#include "HttpConfigServer.hpp"

/*============================================================================*/
/******************************  Constructor  *********************************/
/*============================================================================*/

HttpConfigServer::HttpConfigServer():
	m_server_name(""),
	m_listen(0),
	m_default_error_page(""),
	m_content_length(0),
	m_location_block()
{

}

HttpConfigServer::HttpConfigServer(HttpConfigServer const &other)
{
	*this = other;
}

HttpConfigServer&
HttpConfigServer::operator=(HttpConfigServer const &rhs)
{
	m_server_name = rhs.m_server_name;
	m_listen = rhs.m_listen;
	m_default_error_page = rhs.m_default_error_page;
	m_content_length = rhs.m_content_length;
	m_location_block = rhs.m_location_block;
	return (*this);
}

/*============================================================================*/
/******************************  Destructor  **********************************/
/*============================================================================*/

HttpConfigServer::~HttpConfigServer()
{
}

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

const std::vector<HttpConfigLocation>
HttpConfigServer::get_m_location_block() const
{
	return (this->m_location_block);
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

HttpConfigServer&
HttpConfigServer::parseServerBlock(std::vector<std::string> lines, std::string root, size_t &idx)
{
	bool location_block_exist = false;

	while (42)
	{
		std::vector<std::string> line;
		line.clear();
		line = ft::split(lines[idx], ' ');
		if (HttpConfigLocation::checkCommentLine(line.back()))
			line.pop_back();
		if (line.front().compare("server_name") == 0)
			this->m_server_name = line.back();
		else if (line.front().compare("listen") == 0)
			this->m_listen = stoi(line.back());
		else if (line.front().compare("default_error_page") == 0)
		{
			// HttpConfigLocation::checkFileExist(root, line.back()); // 유효성 체크, 유연한 테스트를 위해 주석처리
			this->m_default_error_page = line.back();
		}
		else if (line.front().compare("content_length") == 0)
			this->m_content_length = stoi(line.back());
		else if (line.front().compare("location") == 0)
		{
			location_block_exist = true;
			HttpConfigLocation location = HttpConfigLocation();
			this->m_location_block.push_back(location.parseLocationBlock(lines, idx));
			continue ;
		}
		else if (line.front().compare("}") == 0)
		{
			idx++;
			break ;
		}
		idx++;
	}
	if (location_block_exist == false)
		throw std::exception();
	return (*this);
}
