#include "HttpConfigServer.hpp"

/*============================================================================*/
/******************************  Constructor  *********************************/
/*============================================================================*/

HttpConfigServer::HttpConfigServer():
	m_server_name(""),
	m_listen(0),
	m_default_error_page(""),
	m_content_length(0),
	m_location_block(),
	m_redirect()
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
	m_redirect = rhs.m_redirect;
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

std::vector<std::string>
HttpConfigServer::get_m_redirect() const
{
	return (this->m_redirect);
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
	std::vector<std::string> line;
	bool location_block_exist = false;
	bool return_exist = false;

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
		if (line.front().compare("server_name") == 0)
			this->m_server_name = line.back();
		else if (line.front().compare("listen") == 0)
			this->m_listen = stoi(line.back());
		else if (line.front().compare("default_error_page") == 0)
		{
			if (ft::isValidFilePath(root + std::string("/") + line.back()) == false)
				throw PathErrorException(lines[idx], idx);
			this->m_default_error_page = line.back();
		}
		else if (line.front().compare("content_length") == 0)
			this->m_content_length = ft::stoi(line.back());
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
			strtol(this->m_redirect[0].c_str(), &tmp, 10);
			if (*tmp || this->m_redirect.size() != 2)
				throw std::exception();
			return_exist = true;
		}
		else if (line.front().compare("location") == 0)
		{
			location_block_exist = true;
			HttpConfigLocation location = HttpConfigLocation();
			this->m_location_block.push_back(location.parseLocationBlock(lines, root, idx));
			continue ;
		}
		else if (line.front().compare("}") == 0)
		{
			idx++;
			break ;
		}
		idx++;
	}
	if (return_exist && location_block_exist) // 리턴도 있고 로케이션도 있으면 에러
		throw std::exception();
	else if (return_exist && location_block_exist == false) // 리턴이 있고 로케이션 블록이 없으면 기본 블록 생성
	{
		location_block_exist = true;
		HttpConfigLocation location = HttpConfigLocation();
		std::vector<Method> limit_except;
		limit_except.push_back(ft::getMethodType("GET"));
		location.set_m_path("/");
		location.set_m_limit_except(limit_except);
		location.set_m_root(root);
		location.set_m_redirect(this->m_redirect);
		this->m_location_block.push_back(location);
	}
	else if (return_exist == false && location_block_exist == false) // 리턴도 없고 로케이션 블록도 없으면 에러
		throw LocationBlockDoesNotExistException();
	return (*this);
}
