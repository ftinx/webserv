#include "Str.hpp"

Str::Str()
{
	p = new std::string;
}

Str::Str(Str const &other)
{
    *this = other;
}

Str& Str::operator=(Str const &rhs)
{
    if (this == &rhs)
        return (*this);
    return (*this);
}


/*============================================================================*/
/******************************  Destructor  **********************************/
/*============================================================================*/

Str::~Str()
{
	delete p;
}
