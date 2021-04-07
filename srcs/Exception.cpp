#include "Exception.hpp"

/*============================================================================*/
/***********************  PARSE CONFIG EXCEPTION ******************************/
/*============================================================================*/

const char *
HttpBlockDoesNotExistException::what() const throw()
{
	return ("Error: Http block does not exist");
}

const char *
ServerBlockDoesNotExistException::what() const throw()
{
	return ("Error: Server block does not exist");
}

const char *
LocationBlockDoesNotExistException::what() const throw()
{
	return ("Error: Location block does not exist");
}

const char *
BracketErrorException::what() const throw()
{
	return ("Error: Bracket is mismatched or duplicated");
}

const char *
MimeTypeErrorException::what() const throw()
{
	return ("Error: \"mime.types\" file must be included");
}

const char *
PathErrorException::what() const throw()
{
	return ("Error: file or folder does not exist");
}
