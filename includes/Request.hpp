#ifndef REQUEST_HPP
# define REQUEST_HPP

/*
** rfc 7230 3.1.1
*/

#include <string>
#include <map>
#include "Uri.hpp"
#include "Utils.hpp"
#include "HttpConfigLocation.hpp"

#define SOCK_SETSIZE 1021

enum REQUEST_STATUS
{
	FAIL,
	CONTINUE,
	SUCCESS,
};

class Request
{
	private:
		std::string m_message;
		std::string m_http_version;
		bool m_check_cgi;
		Method m_method;
		Uri m_uri;
		std::string m_raw_header;
		std::map<std::string, std::string> m_headers;
		std::string m_body;
		int m_content_length;
		int m_error_code;
		std::string m_reset_path;
		HttpConfigLocation m_location_block;
		bool m_read_end;

		/* cgi envp */
		std::string m_path_translated;
		std::string m_path_info;
		std::string m_script_name;
		pid_t m_cgi_pid;
		int m_cgi_stdin;
		int m_cgi_stdout;

		/* HTTP Header */
		std::string m_content_type;
		std::string m_referer;

		/* pipeline */
		long int m_parse_content_length;
		bool m_found_break_line;
		bool m_chunked;
		bool m_chunked_finished_read;
		int m_header_bytes;
		int m_body_bytes;
		int m_cut_bytes;
		bool m_should_read;
		int m_count_message;
		int m_pos;

	public:
		Request();
		Request(Request const &other);
		Request& operator=(Request const &rhs);
		~Request();

		/* getter */
		std::string get_m_message() const;
		std::string get_m_http_version() const;
		bool get_m_check_cgi() const;
		Method get_m_method() const;
		Uri get_m_uri() const;
		std::string get_m_raw_header() const;
		std::map<std::string, std::string> get_m_headers() const ;
		std::string get_m_body() const;
		int get_m_content_length() const;
		int	get_m_error_code() const;
		std::string get_m_reset_path() const;
		HttpConfigLocation get_m_location_block() const;
		bool get_m_read_end() const;
		std::string get_m_path_translated() const;
		std::string get_m_path_info() const;
		std::string get_m_script_name() const;
		pid_t get_m_cgi_pid() const;
		int get_m_cgi_stdin() const;
		int get_m_cgi_stdout() const;
		std::string get_m_content_type() const;
		std::string get_m_referer() const;
		long int get_m_parse_content_length() const;
		bool get_m_found_break_line() const;
		bool get_m_chunked() const;
		bool get_m_chunked_finished_read() const;
		int get_m_header_bytes() const;
		int get_m_body_bytes() const;
		int get_m_cut_bytes() const;
		bool get_m_should_read() const;
		int get_m_count_message() const;
		int get_m_pos() const;

		/* setter */
		void set_m_http_version(std::string);
		void set_m_check_cgi(bool);
		void set_m_method(Method);
		void set_m_cgi_pid(pid_t);
		void set_m_cgi_stdin(int);
		void set_m_cgi_stdout(int);
		void set_m_body(std::string);
		void set_m_error_code(int);
		void set_m_reset_path(std::string);
		void set_m_location_block(HttpConfigLocation);
		void set_m_read_end(bool);
		void set_m_cut_bytes(int);
		void set_m_count_message(int);
		void set_m_pos(int);

		std::string getMethod();
		std::string getContentLength();
		std::string getContentType();
		std::string getAcceptLanguage();
		std::string getReferer();
		int getHeader(int);
		int getBody(int);
		bool parseRequestLine(std::string);
		bool checkMethod();
		bool parseHeader(std::string);
		bool checkBlankLine(std::string);
		bool parseRawHeader();
		std::string getRestPath();
		std::string getScriptName(std::string path);
		bool checkCGI();
		void printHeaders() const;
		void clearBody();
		bool isHost();
};

std::ostream& operator<<(std::ostream &os, const Request &ref);

#endif
