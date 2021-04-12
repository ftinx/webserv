#ifndef EXCEPTION_HPP
# define EXCEPTION_HPP

# include <iostream>

/* parse config exception */

class HttpBlockDoesNotExistException : public std::exception
{
	public:
		virtual const char* what() const throw();
};

class ServerBlockDoesNotExistException : public std::exception
{
	public:
		virtual const char* what() const throw();
};

class LocationBlockDoesNotExistException : public std::exception
{
	public:
		virtual const char* what() const throw();
};

class MimeTypeErrorException : public std::exception
{
	public:
		virtual const char* what() const throw();
};

class BracketPairErrorException : public std::exception
{
	public:
		virtual const char* what() const throw();
};

class BracketDoubleErrorException : public std::exception
{
	private:
		std::string error_msg;
		BracketDoubleErrorException();
	public:
		BracketDoubleErrorException(std::string line, size_t idx);
		virtual ~BracketDoubleErrorException() throw();
		virtual const char* what() const throw();
};

class PathErrorException : public std::exception
{
	private:
		std::string error_msg;
		PathErrorException();
	public:
		PathErrorException(std::string line, size_t idx);
		virtual ~PathErrorException() throw();
		virtual const char* what() const throw();
};

#endif
