#include "Request.hpp"
#include "Utils.hpp"
#include <string>

/*============================================================================*/
/******************************  Constructor  *********************************/
/*============================================================================*/

Request::Request()
: m_message(""), m_http_version(""), m_cgi_version(""), m_check_cgi(false),
m_method(DEFAULT), m_uri(), m_raw_header(""), m_headers(), m_body(""),
m_content_length(-1), m_written_bytes(0),
m_error_code(0),m_reset_path(""), m_location_block(), m_read_end(false),
m_path_translated(""), m_path_info(""), m_script_name(""),
m_cgi_pid(), m_cgi_stdin(0), m_cgi_stdout(1), m_check_fd(-1),
m_content_type(""), m_referer(""), m_parse_content_length(-1),
m_found_break_line(false), m_chunked(false), m_chunked_finished_read(false),
m_header_bytes(0), m_body_bytes(0), m_cut_bytes(0), m_chunked_bytes(0),
m_should_peek(false),m_should_read(false), m_got_all_msg(false)
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
	this->m_cgi_version = rhs.get_m_cgi_version();
	this->m_check_cgi = rhs.get_m_check_cgi();
	this->m_method = rhs.get_m_method();
	this->m_uri = rhs.get_m_uri();
	this->m_raw_header = rhs.get_m_raw_header();
	this->m_headers = rhs.get_m_headers();
	this->m_body = rhs.get_m_body();
	this->m_content_length = rhs.get_m_content_length();
	this->m_written_bytes = rhs.get_m_written_bytes();
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
	this->m_chunked_bytes =  rhs.get_m_chunked_bytes();
	this->m_should_peek =  rhs.get_m_should_peek();
	this->m_should_read =  rhs.get_m_should_read();
	this->m_got_all_msg = rhs.get_m_got_all_msg();
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

std::string
Request::get_m_cgi_version() const
{
	return (this->m_cgi_version);
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
Request::get_m_written_bytes() const
{
	return (this->m_written_bytes);
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

int
Request::get_m_chunked_bytes() const
{
	return (this->m_chunked_bytes);
}

bool
Request::get_m_should_peek() const
{
	return (this->m_should_peek);
}

bool
Request::get_m_should_read() const
{
	return (this->m_should_read);
}

bool
Request::get_m_got_all_msg() const
{
	return (this->m_got_all_msg);
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
Request::set_m_cgi_version(std::string cgi_version)
{
	this->m_cgi_version = cgi_version;
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
Request::set_m_written_bytes(int written_bytes)
{
	this->m_written_bytes = written_bytes;
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
	it = this->m_headers.find("Referer");
	/* `Referer` header not exist */
	if (it == this->m_headers.end())
		return ("");
	std::cout << it->second << std::endl;
	return (this->m_referer = it->second);
}

std::string
Request::getAcceptLanguage()
{
	if (this->m_content_type != "")
		return (this->m_content_type);

	std::map<std::string, std::string>::const_iterator it;
	it = this->m_headers.find("Accept-Language");

	// `Accept-Language Header` not exist
	if (it == this->m_headers.end())
		return (this->m_content_type = "ko");
	// Quality not exist
	if ((*it).second.find(";") == std::string::npos)
	{
		if ((*it).second.find("en") == std::string::npos)
			return (this->m_content_type = "ko");
		return (this->m_content_type = "en");
	}
	// Quality exist
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
			// Compare Quality
			if (std::stof(content_type_line[1].substr(2)) > quality)
			{
				quality = std::stof(content_type_line[1].substr(2));
				content_type = content_type_line[0];
			}
		}
	}
	// std::cout << content_type << std::endl;
	return (this->m_content_type = content_type);
}

bool
Request::isBreakCondition(bool *chunked, int buff_bytes, std::string buff)
{
	size_t pos;
	std::string tmp;

	/* chunked case */
	if ((pos = this->m_message.find("Transfer-Encoding: chunked")) != std::string::npos)
		*chunked = true;
	else if ((pos = this->m_message.find("transfer-encoding: chunked")) != std::string::npos)
		*chunked = true;
	if (*chunked == true && (pos = this->m_message.find("0\r\n\r\n")) == std::string::npos)
	{
		this->m_cut_bytes = buff_bytes;
		return (false);
	}
	if (*chunked == true && (pos = this->m_message.find("0\r\n\r\n")) != std::string::npos)
	{
		this->m_message = this->m_message.substr(0, pos + 5);
		this->m_body_bytes = this->m_message.size() - m_header_bytes;
		this->m_cut_bytes = buff.find("0\r\n\r\n") + 5;
		this->m_chunked_finished_read = true;
		std::cout << "CASE 1" << std::endl;
		return (true);
	}

	/* content-length case */
	int content_length = 0;
	if ((pos = this->m_message.find("Content-Length:")) != std::string::npos)
	{
		tmp = m_message.substr(pos + strlen("Content-Length:"), std::string::npos);
		if ((pos = tmp.find_first_of("\n")) != std::string::npos)
		{
			tmp = ft::trim(tmp.substr(0, pos), " \r");
			content_length = ft::stoi(tmp);
			m_content_length = content_length;
			m_body_bytes = m_content_length;
		}
	}
	else if ((pos = this->m_message.find("content-length:")) != std::string::npos)
	{
		tmp = m_message.substr(pos + strlen("content-length:"), std::string::npos);
		if ((pos = tmp.find_first_of("\n")) != std::string::npos)
		{
			tmp = ft::trim(tmp.substr(0, pos), " \r");
			content_length = ft::stoi(tmp);
			m_content_length = content_length;
			m_body_bytes = m_content_length;
		}
	}
	if (*chunked == false && m_content_length >= 0 && (size_t)(m_content_length + m_header_bytes) <= m_message.size())
	{
		/* 받아온 메세지가 content_length보다 길때 */
		this->m_message = this->m_message.substr(0, m_header_bytes + m_content_length);
		m_cut_bytes = m_header_bytes + m_content_length;
		std::cout << "CASE 2" << std::endl;
		return (true);
	}
	else
	{
		/* 받아온 메세지가 content_length보다 짧으면 더 읽어야 */
		return (false);
	}


	/* no body case */
	if ((pos = this->m_message.find("\r\n\r\n")) != std::string::npos && *chunked == false)
	{
		std::cout << "CASE 3" << std::endl;
		this->m_message = this->m_message.substr(0, pos + 4);
		m_cut_bytes = m_header_bytes;
		return (true);
	}
	return (false);
}

int
Request::getHeader(int fd)
{
	int ret;
	char *buff = (char*)malloc(sizeof(char) * SOCK_BUFF);

	ft::memset(buff, 0, SOCK_BUFF);
	if (m_should_read == false && this->m_raw_header == "")
	{
		if ((ret = recv(fd, buff, SOCK_BUFF - 1, MSG_PEEK)) > 0)
		{
			std::string str(buff);
			size_t pos;
			if ((pos = str.find("\r\n\r\n")) != std::string::npos)
			{
				this->set_m_cut_bytes(pos + 4);
				m_should_read = true;
				free(buff);
				return (CONTINUE);
			}
			else
			{
				m_should_read = false;
				if (ret == SOCK_BUFF - 1)
					throw (HeaderIsTooLargeException());
				free(buff);
				return (CONTINUE);
			}
		}
		/* ret <= 0 아직 고려 안함 */
	}
	else if (m_should_read && m_raw_header == "")
	{
		ret = read(fd, buff, m_cut_bytes);
		if (ret == m_cut_bytes) // 헤더 다 받았을 때
		{
			m_raw_header.append(buff);
			m_should_read = false;
			parseRawHeader();
			free(buff);
			ft::console_log("GETHEADER | return SUCCESS");
			return (SUCCESS);
		}
		else if (ret <= 0)
		{
			free(buff);
			m_should_read = false;
			ft::console_log("GETHEADER | return FAIL");
			return (FAIL);
		}
		else if (ret < m_cut_bytes) // 헤더 덜 받아서 또 read 해야
		{
			m_raw_header.append(buff);
			m_cut_bytes -= ret;
		}
	}
	free(buff);
	ft::console_log("GETHEADER | return CONTINUE");
	return (CONTINUE); // 헤더 덜 받았을 때, 아니면 이미 이전에 헤더 다 받고 파싱 끝냈을 때
}

int
Request::getBody(int fd)
{
	int ret;
	char *buff = (char*)malloc(sizeof(char) * SOCK_BUFF);

	ft::memset(buff, 0, SOCK_BUFF);
	if (m_content_length >= 0)
	{
		ret = read(fd, buff, m_content_length);
		if (ret == m_content_length)
		{
			ft::console_log("++++++ READ PROCESS(SOCK): " + std::to_string(ret));
			m_body.append(buff);
			m_read_end = true;
			free(buff);
			return (SUCCESS);
		}
		else if (ret <= 0 && m_content_length != 0)
		{
			free(buff);
			return (FAIL);
		}
		else // body 읽어올 게 더 남았을 때
		{
			m_body.append(buff);
			m_content_length -= ret;
			free(buff);
			return (CONTINUE);
		}
	}
	else if (m_chunked == true)
	{
		if (m_should_read == false && m_raw_header != "") // 얼마나 읽을지 훔쳐보기
		{
			ft::console_log("------ chunked peek ------");
			ret = recv(fd, buff, SOCK_BUFF - 1, MSG_PEEK);
			if (ret <= 0)
			{
				free(buff);
				return (FAIL);
			}

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
				m_cut_bytes += num + numlen + 2;
			}
			ft::console_log("m_cut_bytes: "+ std::to_string(m_cut_bytes));
			ft::console_log("m_chunked_bytes: "+ std::to_string(num));
			m_should_read = true;
			free(buff);
			return (CONTINUE);
		}
		else if (m_should_read == true &&  m_raw_header != "")// 위에서 설정한 만큼 읽어오기
		{
			/*
			** CGI 의 SUCCESS 는 chunked 를 다 읽었을 때
			** 일반 요청의 SUCCESS는 0/r/n/r/n 까지 다 읽었을 때
			*/
			static int myname = 0;
			ft::console_log("------ chunked read ------");
			ret = read(fd, buff, m_cut_bytes);
			myname += ret;
			ft::console_log("=============================================="+std::to_string(myname));
			// if (ret == m_cut_bytes && m_check_cgi == true)
			// {
			// 	m_should_read = false;
			// 	buff[m_cut_bytes - 2] ='\0';
			// 	ft::console_log("CHHNKED BODY(success): ");
			// 	free(buff);
			// 	return (SUCCESS);
			// }
			// else if (ret == m_cut_bytes && m_check_cgi == false)
			if (ret == m_cut_bytes)
			{
				printf("------m_cut_bytes %d -----\n", m_cut_bytes);
				buff[m_cut_bytes - 2] ='\0';
				if (strlen(buff) >= 3 && strncmp(buff + strlen(buff) - 3, "0\r\n", 3) == 0) /* chunked 메세지의 마지막을 탐지 0\r\n\r\n */
				{
					ft::console_log("CHUNKED BODY(success): ");
					m_should_read = false;
					free(buff);
					m_read_end = true;
					return (SUCCESS);
				}
				ft::console_log("CHUNKED BODY(continue): ");
				m_should_read = false;
				free(buff);
				return (CONTINUE);
			}
			else if (ret > 0)
			{
				buff[m_cut_bytes - 2] ='\0';
				m_body += std::string(&(buff[m_cut_bytes - m_chunked_bytes - 2]));
				m_should_read = true;
				m_cut_bytes -= ret;
				ft::console_log("CHHNKED BODY(continue): ");
				free(buff);
				return (CONTINUE);
			}
			else if (ret <= 0)
			{
				free(buff);
				return (FAIL);
			}
			return (FAIL);
		}
	}
	// content length 없고, chunked도 아닌 오로지 헤더만 들어온 요청
	ft::console_log("only header");
	return (CONTINUE);
}

bool
Request::parseMessage(bool chunked)
{
	size_t i;
	std::vector<std::string> lines = ft::split(this->m_message, "\n");

	if (lines.size() == 0)
		return (false);
	if (parseRequestLine(ft::rtrim(lines[0], "\r")) == false)
	{
		return (false);
	}
	for (i = 1; i < lines.size(); i++)
	{
		if (checkBlankLine(ft::rtrim(lines[i], "\r")))
			break;
		if (parseHeader(ft::rtrim(lines[i], "\r")) == false)
			return (false);
	}
	i++;
	while (i < lines.size())
	{
		if (parseBody(lines[i], i, lines.size(), chunked) == false)
			return (false);
		i++;
	}
	return (true);
}

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
	ft::console_log("PARSE REQUEST LINE | method: "+ std::to_string(m_method));

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
	{
		this->m_error_code = 400;
		return (false);
	}
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
	// size_t pos;
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
	ft::console_log("PARSE HEADER | path: " + m_uri.get_m_path());
	ft::console_log("PARSE HEADER | version: " + m_http_version);
	ft::console_log("PARSE HEADER | raw header: \n" + m_raw_header);
	return (false);
}

bool
Request::parseBody(std::string& line, int i, int size, bool chunked)
{
	long int num;
	std::string newline;

	if (chunked == false)
	{
		newline = line;
		if (i != size - 1)
			newline += "\n";
		this->m_body += newline;
		return (true);
	}
	/* else chunked == true */
	newline = ft::rtrim(line, "\r");
	num = std::strtol(newline.c_str(), 0, 16);
	if (m_parse_content_length != -1 && newline != "0")
	{
		this->m_body += newline.substr(0, m_parse_content_length);
		m_parse_content_length = -1;
	}
	else if (num != 0 && m_parse_content_length == -1)
	{
		m_parse_content_length = num;
		m_content_length += m_parse_content_length;
	}
	return (true);
}

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
			// return (m_reset_path.substr(pos + (*it).size(), std::string::npos));
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
			// std::cout << "***** PATH INFO ******" << m_path_info << std::endl;
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
