#include "Request.hpp"
#include "Utils.hpp"
#include <string>

/*============================================================================*/
/******************************  Constructor  *********************************/
/*============================================================================*/

Request::Request()
: m_message(""), m_http_version(""), m_check_cgi(false),
m_method(DEFAULT), m_uri(), m_raw_header(""), m_headers(), m_body(""),
m_content_length(-1), m_error_code(0),m_reset_path(""), m_location_block(), m_read_end(false),
m_path_translated(""), m_path_info(""), m_script_name(""),
m_cgi_pid(), m_cgi_stdin(0), m_cgi_stdout(1), m_check_fd(-1),
m_content_type(""), m_referer(""), m_parse_content_length(-1),
m_found_break_line(false), m_chunked(false), m_chunked_finished_read(false),
m_header_bytes(0), m_body_bytes(0), m_cut_bytes(0),
m_should_read(false), m_count_message(0)
{
}

Request::Request(Request const &other)
{
	*this = other;
}

Request& Request::operator=(Request const &rhs)
{
	if (this == &rhs)
		return (*this);
	this->m_message = rhs.get_m_message();
	this->m_http_version = rhs.get_m_http_version();
	this->m_check_cgi = rhs.get_m_check_cgi();
	this->m_method = rhs.get_m_method();
	this->m_uri = rhs.get_m_uri();
	this->m_raw_header = rhs.get_m_raw_header();
	this->m_headers = rhs.get_m_headers();
	this->m_body = rhs.get_m_body();
	this->m_content_length = rhs.get_m_content_length();
	this->m_error_code = rhs.get_m_error_code();
	this->m_reset_path = rhs.get_m_reset_path();
	this->m_location_block = rhs.get_m_location_block();
	this->m_read_end = rhs.get_m_read_end();
	this->m_path_translated = rhs.get_m_path_translated();
	this->m_path_info = rhs.get_m_path_info();
	this->m_script_name = rhs.get_m_script_name();
	this->m_cgi_pid = rhs.get_m_cgi_pid();
	this->m_cgi_stdin = rhs.get_m_cgi_stdin();
	this->m_cgi_stdout = rhs.get_m_cgi_stdout();
	this->m_check_fd = rhs.get_m_check_fd();
	this->m_content_type = rhs.get_m_content_type();
	this->m_referer = rhs.get_m_referer();
	this->m_parse_content_length = rhs.get_m_parse_content_length();
	this->m_found_break_line =  rhs.get_m_found_break_line();
	this->m_chunked =  rhs.get_m_chunked();
	this->m_chunked_finished_read =  rhs.get_m_chunked_finished_read();
	this->m_header_bytes =  rhs.get_m_header_bytes();
	this->m_body_bytes =  rhs.get_m_body_bytes();
	this->m_cut_bytes = rhs.get_m_cut_bytes();
	this->m_should_read =  rhs.get_m_should_read();
	this->m_count_message = rhs.get_m_count_message();
	return (*this);
}



/*============================================================================*/
/******************************  Destructor  **********************************/
/*============================================================================*/

Request::~Request(){}


/*============================================================================*/
/********************************  Getter  ************************************/
/*============================================================================*/

std::string
Request::get_m_message() const
{
	return (this->m_message);
}


std::string
Request::get_m_http_version() const
{
	return (this->m_http_version);
}

bool
Request::get_m_check_cgi() const
{
	return (this->m_check_cgi);
}

Method
Request::get_m_method() const
{
	return (this->m_method);
}

Uri
Request::get_m_uri() const
{
	return (this->m_uri);
}

std::string
Request::get_m_raw_header() const
{
	return (this->m_raw_header);
}

std::map<std::string, std::string>
Request::get_m_headers() const
{
	return (this->m_headers);
}

std::string
Request::get_m_body() const
{
	return (this->m_body);
}

int
Request::get_m_content_length() const
{
	return (this->m_content_length);
}

int
Request::get_m_error_code() const
{
	return (this->m_error_code);
}

std::string
Request::get_m_reset_path() const
{
	return (this->m_reset_path);
}

HttpConfigLocation
Request::get_m_location_block() const
{
	return (this->m_location_block);
}

bool
Request::get_m_read_end() const
{
	return (this->m_read_end);
}

std::string
Request::get_m_path_translated() const
{
	return (this->m_path_translated);
}

std::string
Request::get_m_path_info() const
{
	return (this->m_path_info);
}

std::string
Request::get_m_script_name() const
{
	return (this->m_script_name);
}

pid_t
Request::get_m_cgi_pid() const
{
	return (this->m_cgi_pid);
}

int
Request::get_m_cgi_stdin() const
{
	return (this->m_cgi_stdin);
}

int
Request::get_m_cgi_stdout() const
{
	return (this->m_cgi_stdout);
}

int
Request::get_m_check_fd() const
{
	return (this->m_check_fd);
}


std::string
Request::get_m_content_type() const
{
	return (this->m_content_type);
}

std::string
Request::get_m_referer() const
{
	return (this->m_referer);
}

long int
Request::get_m_parse_content_length() const
{
	return (this->m_parse_content_length);
}

bool
Request::get_m_found_break_line() const
{
	return (this->m_found_break_line);
}

bool
Request::get_m_chunked() const
{
	return (this->m_chunked);
}

bool
Request::get_m_chunked_finished_read() const
{
	return (this->m_chunked_finished_read);
}

int
Request::get_m_header_bytes() const
{
	return (this->m_header_bytes);
}

int
Request::get_m_body_bytes() const
{
	return (this->m_body_bytes);
}

int
Request::get_m_cut_bytes() const
{
	return (this->m_cut_bytes);
}

bool
Request::get_m_should_read() const
{
	return (this->m_should_read);
}

int
Request::get_m_count_message() const
{
	return (this->m_count_message);
}

/*============================================================================*/
/********************************  Setter  ************************************/
/*============================================================================*/

void
Request::set_m_http_version(std::string http_version)
{
	this->m_http_version = http_version;
}

void
Request::set_m_check_cgi(bool flag)
{
	this->m_check_cgi = flag;
}

void
Request::set_m_method(Method method)
{
	this->m_method = method;
}

void
Request::set_m_body(std::string body)
{
	this->m_body = body;
}

void
Request::set_m_error_code(int error_code)
{
	this->m_error_code = error_code;
}

void
Request::set_m_reset_path(std::string path)
{
	this->m_reset_path = path;
}

void
Request::set_m_location_block(HttpConfigLocation block)
{
	this->m_location_block = block;
}

void
Request::set_m_read_end(bool read_end)
{
	this->m_read_end = read_end;
}

void
Request::set_m_cgi_pid(pid_t cgi_pid)
{
	this->m_cgi_pid = cgi_pid;
}

void
Request::set_m_cgi_stdin(int cgi_stdin)
{
	this->m_cgi_stdin = cgi_stdin;
}

void
Request::set_m_cgi_stdout(int cgi_stdout)
{
	this->m_cgi_stdout = cgi_stdout;
}

void
Request::set_m_check_fd(int check_fd)
{
	this->m_check_fd = check_fd;
}

void
Request::set_m_cut_bytes(int cut_bytes)
{
	this->m_cut_bytes = cut_bytes;
}

void
Request::set_m_count_message(int count_message)
{
	this->m_count_message = count_message;
}

/*============================================================================*/
/*********************************  Util  *************************************/
/*============================================================================*/

std::string
Request::getMethod()
{
	if (this->m_method == GET)
		return ("GET");
	else if (this->m_method == HEAD)
		return ("HEAD");
	else if (this->m_method == POST)
		return ("POST");
	else if (this->m_method == PUT)
		return ("PUT");
	else if (this->m_method == DELETE)
		return ("DELETE");
	else if (this->m_method == OPTIONS)
		return ("OPTIONS");
	else if (this->m_method == TRACE)
		return ("TRACE");
	return ("DEFAULT");
}

std::string
Request::getContentLength()
{
	std::map<std::string, std::string>::const_iterator it;

	it = this->m_headers.find("Content-Length");
	if (it == this->m_headers.end())
		return ("");
	return (it->second);
}

std::string
Request::getContentType()
{
	std::map<std::string, std::string>::const_iterator it;

	it = this->m_headers.find("Content-Type");
	if (it == this->m_headers.end())
		return ("text/html");
	return (it->second);
}

std::string
Request::getReferer()
{
	/* m_referer exist */
	if (this->m_referer != "")
		return (this->m_referer);
	std::map<std::string, std::string>::const_iterator it;
	it = this->m_headers.find("referer");
	/* `Referer` header not exist */
	if (it == this->m_headers.end())
		return ("");
	return (this->m_referer = it->second);
}

std::string
Request::getAcceptLanguage()
{
	if (this->m_content_type != "")
		return (this->m_content_type);

	std::map<std::string, std::string>::const_iterator it;
	it = this->m_headers.find("accept-language");

	/* `Accept-Language Header` not exist */
	if (it == this->m_headers.end())
		return (this->m_content_type = "ko");
	/* Quality not exist */
	if ((*it).second.find(";") == std::string::npos)
	{
		if ((*it).second.find("en") == std::string::npos)
			return (this->m_content_type = "ko");
		return (this->m_content_type = "en");
	}
	/* Quality exist */
	std::vector<std::string> line;
	std::string content_type = "";
	float quality = 0;
	line = ft::split((*it).second, ',');
	for (std::vector<std::string>::const_iterator line_iter = line.begin() ; line_iter != line.end() ; ++line_iter)
	{
		if ((*line_iter).find(";") != std::string::npos)
		{
			std::vector<std::string> content_type_line;
			content_type_line = ft::split(*line_iter, ';');
			/* Compare Quality */
			if (std::stof(content_type_line[1].substr(2)) > quality)
			{
				quality = std::stof(content_type_line[1].substr(2));
				content_type = content_type_line[0];
			}
		}
	}
	return (this->m_content_type = content_type);
}

// bool
// Request::isBreakCondition(bool *chunked, int buff_bytes, std::string buff)
// {
// 	size_t pos;
// 	std::string tmp;

// 	/* Transfered-encoding: chunked 에서 메세지의 끝 탐지하기*/
// 	if ((pos = this->m_message.find("Transfer-Encoding: chunked")) != std::string::npos)
// 		*chunked = true;
// 	else if ((pos = this->m_message.find("transfer-encoding: chunked")) != std::string::npos)
// 		*chunked = true;
// 	if (*chunked == true && (pos = this->m_message.find("0\r\n\r\n")) == std::string::npos)
// 	{
// 		this->m_cut_bytes = buff_bytes;
// 		return (false);
// 	}
// 	if (*chunked == true && (pos = this->m_message.find("0\r\n\r\n")) != std::string::npos)
// 	{
// 		this->m_message = this->m_message.substr(0, pos + 5);
// 		this->m_body_bytes = this->m_message.size() - m_header_bytes;
// 		this->m_cut_bytes = buff.find("0\r\n\r\n") + 5;
// 		this->m_chunked_finished_read = true;
// 		return (true);
// 	}
// 	/* content-length 또는 헤더만 온 경우 메세지 끝 탐지하기 */
// 	/* -- content-length 파싱하기 */
// 	int content_length = 0;
// 	if ((pos = this->m_message.find("Content-Length:")) != std::string::npos)
// 	{
// 		tmp = m_message.substr(pos + strlen("Content-Length:"), std::string::npos);
// 		if ((pos = tmp.find_first_of("\n")) != std::string::npos)
// 		{
// 			tmp = ft::trim(tmp.substr(0, pos), " \r");
// 			content_length = ft::stoi(tmp);
// 			m_content_length = content_length;
// 			m_body_bytes = m_content_length;
// 		}
// 	}
// 	else if ((pos = this->m_message.find("content-length:")) != std::string::npos)
// 	{
// 		tmp = m_message.substr(pos + strlen("content-length:"), std::string::npos);
// 		if ((pos = tmp.find_first_of("\n")) != std::string::npos)
// 		{
// 			tmp = ft::trim(tmp.substr(0, pos), " \r");
// 			content_length = ft::stoi(tmp);
// 			m_content_length = content_length;
// 			m_body_bytes = m_content_length;
// 		}
// 	}
// 	/* 받아온 메세지가 content_length / header_bytes 보다 길때 읽을 위치(m_cut_bytes) 설정*/
// 	if (*chunked == false && m_content_length >= 0 && (size_t)(m_content_length + m_header_bytes) <= m_message.size())
// 	{
// 		this->m_message = this->m_message.substr(0, m_header_bytes + m_content_length);
// 		m_cut_bytes = m_header_bytes + m_content_length;
// 		return (true);
// 	}
// 	return (false);
// }

int
Request::getHeader(int fd)
{
	int ret = 0;
	char buff[HEADER_BUFF];

	ft::memset(buff, 0, HEADER_BUFF);
	if (m_should_read == false && this->m_raw_header == "")
	{
		if ((ret = recv(fd, buff, HEADER_BUFF - 1, MSG_PEEK)) > 0)
		{
			std::string str(buff);
			std::string header;
			size_t tmp;
			size_t last_pos;
			size_t first_pos;

			/* header 위치 찾아내서 m_cut_bytes로 설정 */
			if ((first_pos = str.find("\r\n\r\n")) != std::string::npos)
			{
				m_count_message = 1;
				first_pos += 4;
				header = str.substr(0, first_pos);
				last_pos = first_pos;
				/* 헤더만 들어오는 메세지가 동일하게 여러번 들어왔을 때 개수를 세줌 */
				while (last_pos < static_cast<size_t>(ret))
				{
					if ((tmp = str.find("\r\n\r\n", last_pos + 1)) != std::string::npos)
					{
						if (str.compare(last_pos, first_pos, header) == 0)
						{
							m_count_message += 1;
						}
						else
						{
							break;
						}
						last_pos = tmp + 4;
					}
					else
						break;
				}
				this->set_m_cut_bytes(last_pos);
				m_should_read = true;
				m_header_bytes = first_pos;
				return (CONTINUE);
			}
			/* header 위치 찾아낼 수 없으면 더 읽어야 */
			else
			{
				m_should_read = false;
				/* 읽어낼 수 있는 헤더 크기를 넘었을 때 오류 처리 */
				if (ret == HEADER_BUFF - 1)
					throw (HeaderIsTooLargeException());
				return (CONTINUE);
			}
		}
		else if (ret <= 0)
		{
			return (FAIL);
		}
	}
	else if (m_should_read && m_raw_header == "")
	{
		ret = read(fd, buff, m_cut_bytes);
		/* 헤더만 온전히 읽어왔을 때 */
		if (ret == m_cut_bytes)
		{
			if (m_count_message != 1)
			{
				buff[m_header_bytes] = '\0';
			}
			m_raw_header.append(buff);
			m_should_read = false;
			parseRawHeader();
			return (SUCCESS);
		}
		/* read 실패 */
		else if (ret <= 0)
		{
			m_should_read = false;
			return (FAIL);
		}
		/*  헤더 덜 받아서 또 read 해야 */
		else if (ret < m_cut_bytes)
		{
			m_raw_header.append(buff);
			m_cut_bytes -= ret;
		}
	}
	ft::console_log(":::::::::::::::::::::::::::::::::::::::::::::::meme::", 1);
	/* 헤더 덜 받았을 때, 아니면 이미 이전에 헤더 다 받고 파싱 끝냈을 때 */
	return (CONTINUE);
}

int
Request::getBody(int fd)
{
	int ret = 0;
	char *buff = (char*)malloc(sizeof(char) * SOCK_BUFF);

	ft::memset(buff, 0, SOCK_BUFF);
	std::cout << m_content_length << std::endl;
	if (m_chunked == false && m_content_length == -1)
	{
		m_read_end = true;
		m_should_read = false;
		free(buff);
		return (SUCCESS);
	}
	/* m_content_length가 있는 경우, 그만큼만 body 읽어오기 */
	if (m_chunked == false && m_content_length >= 0)
	{
		ret = read(fd, buff, m_content_length);
		/* 읽어오려고 했던 만큼 온전히 다 읽었을 때 */
		if (ret == m_content_length)
		{
			std::cout << "hi1"<< std::endl;
			m_body.append(buff);
			m_read_end = true;
			free(buff);
			return (SUCCESS);
		}
		/* read 실패 */
		else if (ret <= 0 && m_content_length != 0)
		{
			std::cout << "hi2"<< std::endl;
			free(buff);
			return (FAIL);
		}
		/* 읽어올 만큼 다 못읽어서 더 읽어야 할 때 */
		else
		{
			std::cout << "hi3"<< std::endl;
			m_body.append(buff);
			m_content_length -= ret;
			free(buff);
			return (CONTINUE);
		}
	}
	/* chunked의 경우, 훔쳐보면서 파싱과 동시에 읽을 양을 결정 */
	else if (m_chunked == true)
	{
		/* 최대 버퍼만큼 일단 훔쳐봄 */
		if (m_should_read == false && m_raw_header != "")
		{
			ret = recv(fd, buff, SOCK_BUFF - 1, MSG_PEEK);
			if (ret <= 0)
			{
				free(buff);
				return (FAIL);
			}
			/* 훔쳐본 메세지 파싱 및 읽을 메세지 길이 설정 */
			std::string str(buff);
			std::string crlf("\r\n");
			std::vector<std::string> lines = ft::split2(str, crlf);
			std::vector<std::string>::const_iterator it;
			size_t it_size = lines.size();
			long num;
			char *temp;
			int numlen;

			size_t i = it_size % 2;
			m_cut_bytes = 0;
			if (m_content_length == -1)
				m_content_length = 0;
			for (it = lines.begin(); it != lines.end() - i; ++it)
			{
				num = std::strtol((*it).c_str(), &temp, 16);
				if (num == 0)
				{
					m_read_end = true;
				}
				if (*temp)
				{
					free(buff);
					return (FAIL);
				}
				numlen = (*it).size() + 2;
				it++;
				if ((*it).size() != static_cast<size_t>(num) && it != lines.end() - 1) // 숫자에 안맞게 메세지 온 비정상적인 경우
				{
					free(buff);
					return (FAIL);
				}
				else if ((*it).size() != static_cast<size_t>(num) && it == lines.end() - i - 1) // 마지막 메세지의 문자가 짤려서 왔을 때
				{
					free(buff);
					m_should_read = true;
					return (CONTINUE);
				}
				m_body += *it;
				m_content_length += num;
				m_cut_bytes += num + numlen + 2;
			}
			/* 읽어온 바디가 conf에서 설정한 limit_body_size 보다 크면 에러처리 */
			if (m_location_block.get_m_limit_body_size() < m_content_length)
			{
				m_error_code = 413;
			}
			m_should_read = true;
			free(buff);
			return (CONTINUE);
		}
		/* 위에서 설정한 만큼만 읽어서 버퍼 비워주기 */
		else if (m_should_read == true &&  m_raw_header != "")
		{
			/*
			** CGI 의 SUCCESS 는 chunked 를 다 읽었을 때
			** 일반 요청의 SUCCESS는 0/r/n/r/n 까지 다 읽었을 때
			*/
			ret = read(fd, buff, m_cut_bytes);
			/* 목표한 만큼 다 읽었을 때 */
			if (ret == m_cut_bytes)
			{
				buff[m_cut_bytes - 2] ='\0';
				/* chunked 메세지의 마지막을 탐지 0\r\n\r\n */
				if (strlen(buff) >= 3 && strncmp(buff + strlen(buff) - 3, "0\r\n", 3) == 0)
				{
					m_should_read = false;
					free(buff);
					m_read_end = true;
					return (SUCCESS);
				}
				m_should_read = false;
				free(buff);
				return (CONTINUE);
			}
			/* 목표한 만큼 못읽었을 때 다시 읽어올 길이를 설정 */
			else if (ret > 0)
			{
				buff[m_cut_bytes - 2] ='\0';
				m_body += std::string(&(buff[m_cut_bytes - 2]));
				m_should_read = true;
				m_cut_bytes -= ret;
				free(buff);
				return (CONTINUE);
			}
			/* read 실패 */
			else if (ret <= 0)
			{
				free(buff);
				return (FAIL);
			}
			return (FAIL);
		}
	}
	return (CONTINUE);
}

// bool
// Request::parseMessage(bool chunked)
// {
// 	size_t i;
// 	std::vector<std::string> lines = ft::split(this->m_message, "\n");

// 	if (lines.size() == 0)
// 		return (false);
// 	if (parseRequestLine(ft::rtrim(lines[0], "\r")) == false)
// 	{
// 		return (false);
// 	}
// 	for (i = 1; i < lines.size(); i++)
// 	{
// 		if (checkBlankLine(ft::rtrim(lines[i], "\r")))
// 			break;
// 		if (parseHeader(ft::rtrim(lines[i], "\r")) == false)
// 			return (false);
// 	}
// 	i++;
// 	while (i < lines.size())
// 	{
// 		if (parseBody(lines[i], i, lines.size(), chunked) == false)
// 			return (false);
// 		i++;
// 	}
// 	return (true);
// }

bool
Request::parseRequestLine(std::string request_line)
{
	int error_code;
	std::vector<std::string> pieces = ft::split(request_line, ' ');

	if (pieces.size() != 3)
	{
		this->m_error_code = 400;
		return (false);
	}
	this->m_method = ft::getMethodType(pieces[0]);
	this->m_uri.set_m_uri(pieces[1]);
	this->m_http_version = pieces[2];

	if (this->m_uri.get_m_uri().size() > 8000)
	{
		this->m_error_code = 414;
		return (false);
	}
	if (this->m_http_version != "HTTP/1.1")
	{
		this->m_error_code = 505;
		return (false);
	}
	if (checkMethod() == false)
	{
		this->m_error_code = 501;
		return (false);
	}
	if ((error_code = m_uri.parseUri()))
	{
		this->m_error_code = error_code;
		return(false);
	}
	return (true);
}

bool
Request::parseHeader(std::string line)
{
	std::vector<std::string> key_value;

	size_t pos = line.find_first_of(":");

	if (pos == std::string::npos)
	{
		this->m_error_code = 400;
		return (false);
	}
	key_value.push_back(line.substr(0, pos));
	key_value.push_back(line.substr(pos + 1, std::string::npos));
	if (key_value.size() < 2)
	{
		this->m_error_code = 400;
		return (false);
	}
	int key_len = key_value[0].length();
	if (key_len == 0 || key_value[0][key_len - 1] == ' ')
	{
		this->m_error_code = 400;
		return (false);
	}
	if (ft::strTolower(key_value[0]) == "host")
		this->m_uri.set_m_host(ft::trim(key_value[1], " "));
	else if (ft::strTolower(key_value[0]) == "port")
		this->m_uri.set_m_port(ft::trim(key_value[1], " "));
	if (ft::strTolower(key_value[0]) == "content-length")
		this->m_content_length = ft::stoi(ft::trim(key_value[1], " "));
	if ((ft::strTolower(key_value[0]) == "transfer-encoding") && (ft::trim(key_value[1], " ") == "chunked"))
		this->m_chunked = true;
	if (this->m_headers.insert(make_pair(ft::strTolower(key_value[0]), ft::trim(key_value[1], " "))).second == false
	&& ft::strTolower(key_value[0]) == "host")
		this->m_error_code = 400;
	return (true);
}

bool
Request::checkBlankLine(std::string str)
{
	if (str[0] == '\r')
		return (true);
	return (false);
}

bool
Request::parseRawHeader()
{
	std::vector<std::string> lines = ft::split(m_raw_header, std::string("\r\n"));
	std::vector<std::string>::const_iterator it;

	if (parseRequestLine(lines[0]) == false)
		return (false);
	for (it = lines.begin() + 1; it != lines.end(); ++it)
	{
		if (parseHeader(*it) == false)
		{
			std::cout << "Error" << m_error_code << std::endl;
			return(false);
		}
	}
	return (false);
}

// bool
// Request::parseBody(std::string& line, int i, int size, bool chunked)
// {
// 	long int num;
// 	std::string newline;

// 	if (chunked == false)
// 	{
// 		newline = line;
// 		if (i != size - 1)
// 			newline += "\n";
// 		this->m_body += newline;
// 		return (true);
// 	}
// 	/* else chunked == true */
// 	newline = ft::rtrim(line, "\r");
// 	num = std::strtol(newline.c_str(), 0, 16);
// 	if (m_parse_content_length != -1 && newline != "0")
// 	{
// 		this->m_body += newline.substr(0, m_parse_content_length);
// 		m_parse_content_length = -1;
// 	}
// 	else if (num != 0 && m_parse_content_length == -1)
// 	{
// 		m_parse_content_length = num;
// 		m_content_length += m_parse_content_length;
// 	}
// 	return (true);
// }

bool
Request::checkMethod()
{
	if (this->m_method == DEFAULT)
		return (false);
	return (true);
}

std::string
Request::getRestPath()
{
	std::vector<std::string> v(m_location_block.get_m_cgi());
	std::vector<std::string>::const_iterator it;
	size_t pos;

	for (it = v.begin(); it != v.end(); it++)
	{
		if ((pos = m_reset_path.find(*it)) != std::string::npos)
		{
			std::string path = m_reset_path.substr(pos + (*it).size(), std::string::npos);
			if (path == "")
				path = "/";
			return (path);
		}
	}
	return ("/");
}

std::string
Request::getScriptName(std::string path)
{
	size_t pos = path.find_last_of("");

	if (pos == std::string::npos)
	{
		return (path);
	}
	return (path.substr(pos + 1, std::string::npos));
}

bool
Request::checkCGI()
{
	std::string path(m_reset_path);
	HttpConfigLocation loc(m_location_block);

	if (ft::checkValidFileExtension(path, loc.get_m_cgi()))
	{
		/* valid cgi file */
		if (ft::isValidFilePath(path))
		{
			this->m_path_translated = path;
			this->m_path_info = "/";
			this->m_script_name = getScriptName(path);
		}
		/* invalid cgi file, using default cgi */
		else
		{
			this->m_path_translated = loc.get_m_cgi_path();
			this->m_script_name = getScriptName(m_path_translated);
			this->m_path_info = getRestPath();
		}
		return (true);
	}
	return (false);
}

void
Request::printHeaders() const
{
	std::map<std::string, std::string> m = this->m_headers;
	std::map<std::string, std::string>::iterator i;
	std::vector<std::string> ret;

	std::cout << "#### print headers #### " << std::endl;
	for (i = m.begin(); i != m.end(); i++)
	{
		std::cout << i->first << ": " << i->second << std::endl;
	}
}

std::ostream& operator<<(std::ostream &os, Request const& ref)
{
	Uri uri;

	uri = ref.get_m_uri();
	return (os << "error code: " << ref.get_m_error_code() << std::endl
	<< "http version: "<< ref.get_m_http_version() << std::endl
	<< "method: " << ref.get_m_method() << std::endl
	<< "uri: " << uri.get_m_uri() << std::endl
	<< "> scheme: " << uri.get_m_scheme() << std::endl
	<< "> host: " << uri.get_m_host() << std::endl
	<< "> port: " << uri.get_m_port() << std::endl
	<< "> path: " << uri.get_m_path() << std::endl
	<< "> body: " << ref.get_m_body() << std::endl);
}

void
Request::clearBody()
{
	this->m_body.clear();
	return ;
}

bool
Request::isHost()
{
	std::map<std::string, std::string>::const_iterator it;
	it = this->m_headers.find("host");
	if (it == this->m_headers.end())
		return (false);
	return (true);
}
