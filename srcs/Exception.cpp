#include "Exception.hpp"

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
BracketErrorException::what() const throw()
{
	return ("Error: Bracket is mismatched or duplicated");
}

const char *
MimeTypeErrorException::what() const throw()
{
	return ("Error: \"mime.types\" file must be included");
}
