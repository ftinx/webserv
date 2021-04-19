#include "Exception.hpp"

/*============================================================================*/
/************************  GET REQUEST EXCEPTION ******************************/
/*============================================================================*/

const char *
HeaderIsTooLargeException::what() const throw()
{
	return ("[Request] Header is too large");
}


/*============================================================================*/
/***********************  PARSE CONFIG EXCEPTION ******************************/
/*============================================================================*/

const char *
HttpBlockDoesNotExistException::what() const throw()
{
	return ("[Config] Http block does not exist");
}

const char *
ServerBlockDoesNotExistException::what() const throw()
{
	return ("[Config] Server block does not exist");
}

const char *
LocationBlockDoesNotExistException::what() const throw()
{
	return ("[Config] Location block does not exist");
}

const char *
MimeTypeErrorException::what() const throw()
{
	return ("[Config] \"mime.types\" file must be included");
}

const char *
BracketPairErrorException::what() const throw()
{
	return ("[Config] Bracket must be paired");
}

const char *
BracketDoubleErrorException::what() const throw()
{
	return (this->error_msg.c_str());
}

BracketDoubleErrorException::BracketDoubleErrorException(std::string line, size_t idx)
{
	this->error_msg += std::string("[Config] Bracket is duplicated -> (line:")
		+ std::to_string(idx + 1)
		+ std::string(") ")
		+ line;
}

BracketDoubleErrorException::~BracketDoubleErrorException() throw() {}

const char *
PathErrorException::what() const throw()
{
	return (this->error_msg.c_str());
}

PathErrorException::PathErrorException(std::string line, size_t idx)
{
	this->error_msg += std::string("[Config] File or folder does not exist -> (line:")
		+ std::to_string(idx + 1)
		+ std::string(") ")
		+ line;
}

PathErrorException::~PathErrorException() throw() {}

const char *
ListenValueErrorException::what() const throw()
{
	return (this->error_msg.c_str());
}

ListenValueErrorException::ListenValueErrorException(std::string line, size_t idx)
{
	this->error_msg += std::string("[Config] Listen(port) value must be a number (1024 ~ 49151) -> (line:")
		+ std::to_string(idx + 1)
		+ std::string(") ")
		+ line;
}

ListenValueErrorException::~ListenValueErrorException() throw() {}

const char *
ReturnMethodErrorException::what() const throw()
{
	return (this->error_msg.c_str());
}

ReturnMethodErrorException::ReturnMethodErrorException(std::string line, size_t idx)
{
	this->error_msg += std::string("[Config] Return method must be a number (301, 302, 307) -> (line:")
		+ std::to_string(idx + 1)
		+ std::string(") ")
		+ line;
}

ReturnMethodErrorException::~ReturnMethodErrorException() throw() {}


const char *
LimitBodySizeErrorException::what() const throw()
{
	return (this->error_msg.c_str());
}

LimitBodySizeErrorException::LimitBodySizeErrorException(std::string line, size_t idx)
{
	this->error_msg += std::string("[Config] Limit body size value must be a number (0 ~ INT_MAX) -> (line:")
		+ std::to_string(idx + 1)
		+ std::string(") ")
		+ line;
}

LimitBodySizeErrorException::~LimitBodySizeErrorException() throw() {}
