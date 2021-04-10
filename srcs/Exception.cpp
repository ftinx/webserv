#include "Exception.hpp"

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
	this->error_msg += std::string("[Config] file or folder does not exist -> (line:")
		+ std::to_string(idx + 1)
		+ std::string(") ")
		+ line;
}

PathErrorException::~PathErrorException() throw() {}
