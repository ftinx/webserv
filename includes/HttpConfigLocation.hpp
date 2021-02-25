#ifndef HTTPCONFIGLOCATION_HPP
# define HTTPCONFIGLOCATION_HPP

# include <string>

enum Method
{
	DEFAULT,
	GET,
	HEAD,
	POST,
	PUT,
	DELETE,
	OPTIONS,
	TRACE
}

class HttpConfigLocation
{
	private:
		std::string m_path;
		Method m_limit_except;
		std::string m_root;
		std::string m_index;
		std::string m_cgi;
		std::string m_cgi_path;
	private:
		HttpConfigLocation();
		virtual ~HttpConfigLocation();
		HttpConfigLocation(HttpConfigLocation const &other);
		HttpConfigLocation& operator=(HttpConfigLocation const &rhs);
	public:
		bool	checkStartLocation(string);
		bool	checkValidHttpConfigLocation();
};

#endif
