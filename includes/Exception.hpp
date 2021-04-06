#ifndef EXCEPTION_HPP
# define EXCEPTION_HPP

# include <iostream>

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

class BracketErrorException : public std::exception
{
	public:
		virtual const char* what() const throw();
};

class MimeTypeErrorException : public std::exception
{
	public:
		virtual const char* what() const throw();
};

#endif
