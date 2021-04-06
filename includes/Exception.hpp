#ifndef EXCEPTION_HPP
# define EXCEPTION_HPP

# include <iostream>

/* parse config */

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

class PathErrorException : public std::exception
{
	public:
		virtual const char* what() const throw();
};


#endif
